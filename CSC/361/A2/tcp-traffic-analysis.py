import sys
import struct
from uuid import uuid4
from ipaddress import IPv4Address
from typing import Dict, List, Literal, Tuple, TypedDict
from dataclasses import dataclass

MAGIC_NATIVE = 0xa1b2c3d4
MAGIC_SWAPPED = 0xd4c3b2a1

#  Flags
URG = 1 << 5
ACK = 1 << 4
PSH = 1 << 3
RST = 1 << 2
SYN = 1 << 1
FIN = 1 << 0

type State = Literal['S0F0', 'S1F0', 'S2F0', 'S1F1', 'S2F2', 'R']
type Endian = Literal['>', '<']

# TODO: remove total_cnt and pkt_cnt
@dataclass(frozen=True)
class Packet:
    conn_id:   str
    pkt_id :   str
    src_addr:  int
    dest_addr: int
    src_port:  int
    dest_port: int 
    flags:     int
    pkt_size:  int # holds the size of the payload excluding headers

''' using first seen rule to determine src and dest addresses '''
@dataclass
class Connection:
    conn_id:   str
    src_addr:  int
    dest_addr: int
    src_port:  int
    dest_port: int
    status:    Tuple[int, int]
    packets:   Dict[str, List[Packet]]



@dataclass(frozen=True)
class Summary:
    conn_id: Tuple[int, int, int, int] # contains (src_ip, src_port, dest_addr, dest_port)
    connection: int
    src_addr: int
    dest_addr: int
    src_port: int
    dest_port: int
    state: List[int]
    start: int
    end: int
    duration: int
    packets_sent: int
    packets_recv: int
    packets_total: int
    bytes_sent: int
    bytes_recv: int
    bytes_total: int

conn_cnt = 0
'''
open are actively managed connections.
Which can then move to:
    1. reset - where we only track the count
    2. complete - where we keep the details to perform analysis later

Finally, established connections is a simple counter.
'''
class Report(TypedDict):
    open:  Dict[str, Connection]
    reset: Dict[str, Connection]
    complete: Dict[str, Connection]
    preestablished_cnt: int

report: Report = {
        'open': {},
        'reset': {},
        'complete' : {},
        'preestablished_cnt': 0,

    }
def main():
    if len(sys.argv) != 2:
        raise Exception('Usage ./<filename> <path-to-cap-file>')

    with open(sys.argv[1], 'rb') as f:
        '''
        parse global header in the cap file located in the first 24 bytes
        handle big and little endian encodings.
        '''
        global_header = f.read(24)
        magic, version_major, version_minor, \
        thiszone, sigfigs, snaplen, network = \
                struct.unpack('<IHHIIII', global_header) # little-endian

        if magic == MAGIC_SWAPPED:
            magic, version_major, version_minor, \
            thiszone, sigfigs, snaplen, network = \
            struct.unpack('>IHHIIII', global_header)
            endian = '>'
        elif magic == MAGIC_NATIVE:
            endian = '<'
        else:
            raise ValueError('Invalid PCAP file')

        total_cnt = 0
        while True:
            pkt_header = f.read(16)
            pkt_data   = b''
            if not pkt_header: break
            if len(pkt_header) != 16:
                raise Exception(f'Truncated packet header: {len(pkt_header)} bytes')

            ''' packet header '''
            ts_sec, ts_usec, incl_len, orig_len = \
                    struct.unpack(f'{endian}4I', pkt_header)  

            ''' packet data ''' 
            if incl_len:
               pkt_data = f.read(incl_len)
               total_cnt += 1

            if len(pkt_data) != incl_len:
                raise Exception(f'Packet data bytes mismatch, expected {incl_len}, got: {len(pkt_data)}')

            if incl_len > 0:
                pkt = pkt_data_parse(incl_len, pkt_data, endian)
                create_or_update_connection(pkt)
        print(total_cnt)

''' parse ethernet, IPv4, and TCP/UDP headers '''
def pkt_data_parse(size: int, data: bytes, 
                   endian: Endian) -> Packet:

    ''' sizes in bytes '''
    eth_header = 14
    ipv4_header = 20
    tcp_header = 20

    ''' eth header '''
    eth_bytes = data[:eth_header]

    dest_mac, src_mac, eth_type = \
            struct.unpack(f'> 6s 6s H', eth_bytes)

    
    ''' ipv4 header '''
    ipv4_bytes = data[eth_header: eth_header + ipv4_header]

    ipver_ihl, tos, total_len, \
    identification, flags_and_offset, ttl, \
    protocol, ipv4_checksum, src_ip, dest_ip = \
        struct.unpack(f'> 2B 3H 2B H 2I', ipv4_bytes)

    ''' 
        tcp header 

    IHL indicates number of 4 byte blocks,
    1. we first extract lower nibble from ipver_ihl
    2. then we multiply x4 to get size in bytes
    3. we use eth_header + ihl as an offset to skip
        eth and ipv4 and begin parsing the tcp header
    '''
    # IHL is long enough to encapsulate options if any
    ihl = (ipver_ihl & 0x0F) * 4 

    tcp_bytes = data[eth_header + ihl: eth_header + ihl + tcp_header]
    src_port, dest_port, seq_num, \
    ack_num, offset_reserved, tcp_flags, \
    ws, tcp_checksum, urgent_ptr = \
        struct.unpack(f'> 2H 2I 2B H 2H', tcp_bytes)

    pkt_id = (src_ip, src_port, dest_ip, dest_port)
    thl = ((offset_reserved >> 4) & 0x0F) * 4 # tcp header length in bytes
    return Packet(
        # sort ids so sender and receiver map to the same connection
        conn_id       = str(tuple(sorted(pkt_id))),
        pkt_id        = str(pkt_id),
        src_addr      = src_ip,
        dest_addr     = dest_ip,
        src_port      = src_port,
        dest_port     = dest_port,
        flags         = tcp_flags,
        pkt_size      = total_len - ihl - thl
        )
'''
The entry-point is the open field, we will always 
report = {
        'open': {},
        'reset': 0,
        'complete' : {},
        'preestablished_cnt': 0,

    }

'reset' and 'complete' states get assigned a uuid4() instead of conn_id
as multiple connection instances between the same (client, server)
may exist.
'''
def create_or_update_connection(pkt: Packet):
    # TODO: implement line 39 & upwards on A2 writeup
    conn_seen = report['open'].get(pkt.conn_id)

    if not conn_seen:
        if not pkt.flags & SYN: report['preestablished_cnt'] += 1
        ''' 
        1. move pre established rst connections to 'reset' 
        2. move pre established closed connections to 'complete'
        3. move only currently open conns to 'open' state (no FIN or RST) 
        '''

        if pkt.flags & RST:
            report['reset'][uuid4().hex] = new_connection(pkt)

        elif pkt.flags & FIN and pkt.pkt_size == 0:
            report['complete'][uuid4().hex] = new_connection(pkt)
        else: 
            report['open'][pkt.conn_id] = new_connection(pkt)

    if conn_seen:
        ''' 
        Update connection to include latest packet sent
        1. move from 'open' to 'reset' on rst 
        2. move from 'open' to 'complete' on FIN & no data
        3. keep in 'open' otherwise
        '''
        update_connection(pkt)
        if pkt.flags & RST: 
            report['reset'][uuid4().hex] = report['open'].pop(pkt.conn_id)
        elif pkt.flags & FIN and pkt.pkt_size == 0:
            report['complete'][uuid4().hex] = report['open'].pop(pkt.conn_id)


def new_connection(pkt: Packet) -> Connection:
    src = pkt.src_addr
    dest = pkt.dest_addr
    return Connection(
        conn_id   = pkt.conn_id,
        src_addr  = pkt.src_addr,
        dest_addr = pkt.dest_addr,
        src_port  = pkt.src_port,
        dest_port = pkt.dest_port,
        status    = (pkt.flags & SYN, pkt.flags & FIN),
        packets   =    {
            f'{src}': [pkt],
            f'{dest}': []
        }
    )
def update_connection(pkt: Packet) -> None: 
    conn = report['open'][pkt.conn_id]
    s, f = conn.status
    conn.status = (s + pkt.flags & SYN, f + pkt.flags & FIN)
    src = str(pkt.src_addr)
    conn.packets[src].append(pkt)

if __name__ == '__main__':
    main()
    pkt_cnt = 0
    for conn in report['complete'].values():
        for pkts in conn.packets.values():
            pkt_cnt += len(pkts) 

    for conn in report['open'].values():
        for pkts in conn.packets.values():
            pkt_cnt += len(pkts)

    for conn in report['reset'].values():
        for pkts in conn.packets.values():
            pkt_cnt += len(pkts)
    print(pkt_cnt)
