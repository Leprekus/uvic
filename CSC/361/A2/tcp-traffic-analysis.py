import sys
import struct
import itertools
from uuid import uuid4
from ipaddress import IPv4Address
from typing import Callable, Dict, List, Literal, Tuple, TypedDict, cast
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
@dataclass
class Packet:
    conn_id:   str
    pkt_id :   str
    src_addr:  int
    dest_addr: int
    src_port:  int
    dest_port: int 
    flags:     int
    pkt_size:  int # holds the size of the payload excluding headers
    ts_sec:    int
    ts_usec:   int
    ws:        int

''' using first seen rule to determine src and dest addresses '''
@dataclass
class Connection:
    conn_id:     str
    conn_number: int
    src_addr:    int
    dest_addr:   int
    src_port:    int
    dest_port:   int
    status:      Tuple[int, int]
    is_reset:    bool
    is_complete: bool
    packets:     Dict[str, List[Packet]]

@dataclass
class SummaryPartial:
    conn_id: str # contains (src_ip, src_port, dest_addr, dest_port)
    connection: int
    src_addr: IPv4Address
    dest_addr: IPv4Address
    src_port: int
    dest_port: int
    state: str

@dataclass
class Summary(SummaryPartial):
    start: Tuple[int, int]
    end: Tuple[int, int]
    duration: Tuple[int, int] 
    packets_sent: int
    packets_recv: int
    packets_total: int
    bytes_sent: int
    bytes_recv: int
    bytes_total: int

@dataclass
class Stats:
    duration_min: Tuple[int, int] 
    duration_max: Tuple[int, int] 
    duration_avg: Tuple[int, int] 

    rtt_min: int
    rtt_max: int
    rtt_avg: float

    pkt_min: int
    pkt_max: int
    pkt_avg: float

    ws_min: int
    ws_max: int
    ws_avg: float

conn_cnt = 0
'''
open are actively managed connections.
Which can then move to:
    1. reset - where we only track the count
    2. complete - where we keep the details to perform analysis later

Finally, established connections is a simple counter.
'''
class Report(TypedDict):
    preestablished_cnt: int
    conn_cnt: int
    total_pkt_cnt: int

    open:     Callable [[], List[Connection]]
    reset:    Callable [[], List[Connection]]
    complete: Callable [[], List[Connection]]


total_cnt = 0
connections: Dict[str, Connection] = {}
report: Report = {
    'preestablished_cnt': 0,
    'conn_cnt': 0,
    'total_pkt_cnt': 0,
    
    'reset': lambda: [c for c in connections.values() if c.is_reset],
    'open': lambda: [c for c in connections.values() \
            if (not c.is_reset) and (not c.is_complete)],
    'complete': lambda: [c for c in connections.values() if c.is_complete]
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
               report['total_pkt_cnt'] += 1

            if len(pkt_data) != incl_len:
                raise Exception(f'Packet data bytes mismatch, expected {incl_len}, got: {len(pkt_data)}')

            if incl_len > 0:
                pkt = pkt_data_parse(incl_len, pkt_data, endian)
                pkt.ts_sec = ts_sec
                pkt.ts_usec = ts_usec # append timestamp
                create_or_update_connection(pkt)
        generate_report()


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
        pkt_size      = total_len - ihl - thl,
        ts_sec        = 0,
        ts_usec       = 0, # this gets updated inside the main while loop 
        ws            = ws
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
    conn_seen = connections.get(pkt.conn_id)

    if not conn_seen and (not pkt.flags & SYN):
        report['preestablished_cnt'] += 1

    if conn_seen:
        update_connection(pkt)

    else: 
        conn = new_connection(pkt)
        connections[conn.conn_id] = conn


def new_connection(pkt: Packet) -> Connection:
    src = pkt.src_addr
    dest = pkt.dest_addr
    report['conn_cnt'] += 1

    return Connection(
        conn_id   = pkt.conn_id,
        conn_number= report['conn_cnt'],
        src_addr  = pkt.src_addr,
        dest_addr = pkt.dest_addr,
        src_port  = pkt.src_port,
        dest_port = pkt.dest_port,
        status    = (pkt.flags & SYN, pkt.flags & FIN),
        is_reset   = bool(pkt.flags & RST),
        is_complete = bool(pkt.flags & FIN and pkt.pkt_size == 0),
        packets   =    {
            f'{src}': [pkt],
            f'{dest}': []
        }
    )

''' 
    update the SkFk status and 
    track whether we have seen a reset
    append packet to the connection history 
'''
def update_connection(pkt: Packet) -> None: 
    conn = connections[pkt.conn_id]
    s, f = conn.status
    conn.status = (s + pkt.flags & SYN, f + pkt.flags & FIN)
    src = str(pkt.src_addr)
    conn.is_reset |= bool(pkt.flags & RST)
    conn.is_complete |= bool(pkt.flags & FIN and pkt.pkt_size == 0)
    conn.packets[src].append(pkt)

def _generate_report_for(
        key: Literal['reset', 'open', 'complete']
        ) -> List[Summary | SummaryPartial]:


    summaries = []
    for conn in report[key]():

        summaryP = SummaryPartial(
            conn_id = conn.conn_id,
            connection = conn.conn_number,
            src_addr = IPv4Address(conn.src_addr),
            dest_addr = IPv4Address(conn.dest_addr),
            src_port = conn.src_port,
            dest_port = conn.dest_port,
            state = ''
            )

        if key == 'reset':
            summaryP.state = 'R'
        else:
            summaryP.state = f'S{conn.status[0]}F{conn.status[1]}'

        print('\n'.join([
            f'Connection {summaryP.connection}:',
            f'Source Address: {summaryP.src_addr}',
            f'Destination Address: {summaryP.dest_addr}',
            f'Source Port: {summaryP.src_port}',
            f'Destination Port: {summaryP.dest_port}',
            f'Status: {summaryP.state}',
        ]))

        if key == 'complete':
            src, dest = conn.packets.values()
            start = min(itertools.chain(src, dest), 
                        key=lambda p: (p.ts_sec, p.ts_usec))
            end   = max(itertools.chain(src, dest), 
                        key=lambda p: (p.ts_sec, p.ts_usec))

            pkts_sent = len(conn.packets[str(conn.src_addr)])
            pkts_recv = len(conn.packets[str(conn.dest_addr)])
            bytes_sent = sum(p.pkt_size for p in 
                             conn.packets[str(conn.src_addr)])
            bytes_recv = sum(p.pkt_size for p in 
                             conn.packets[str(conn.dest_addr)])

            dur_ms, dur_ns = (
                    end.ts_sec - start.ts_sec,
                    end.ts_usec - start.ts_usec
                )
            
            
            summaryP = Summary(
                *summaryP.__dict__.values(),
                start = (start.ts_sec, start.ts_usec), 
                end = (end.ts_sec, end.ts_usec),
                duration = (dur_ms, dur_ns) if dur_ns >= 0 
                    else (dur_ms - 1,  dur_ns + 1_000_000),
                packets_sent = pkts_sent,
                packets_recv = pkts_recv,
                packets_total = pkts_sent + pkts_recv,
                bytes_sent = bytes_sent,
                bytes_recv = bytes_recv,
                bytes_total = bytes_sent + bytes_recv
                )
            print(
                '\n'.join([
                    f'Start Time: {summaryP.start} seconds',
                    f'End Time: {summaryP.end} seconds',
                    f'Duration: {summaryP.duration} seconds',
                    f'Number of packets send from Source to Destination: {
                        len(
                            connections[summaryP.conn_id]
                            .packets[str(int(summaryP.src_addr))])
                    }',
                    f'Number of packets sent from Destination to Source: {
                        len(
                            connections[summaryP.conn_id]
                            .packets[str(int(summaryP.dest_addr))])

                    }',
                    f'Total number of packets: {
                        len(
                            connections[summaryP.conn_id]
                            .packets[str(int(summaryP.src_addr))])
                       + len(
                            connections[summaryP.conn_id]
                            .packets[str(int(summaryP.dest_addr))])
                    }',
                    f'Numer of data bytes sent from Source to Destination: {
                        summaryP.bytes_sent
                    }',
                    f'Number of data bytes sent from Destination to Source: {
                        summaryP.bytes_recv
                    }',
                    f'Total number of data bytes: {
                        summaryP.bytes_sent + summaryP.bytes_recv
                    }'
                ])
            )
        print('\n'.join([
        'END',
        '++++++++++++++++++++++++++++++++'
        ]))
        summaries.append(summaryP)
    return summaries

        
def _compute_stats_for_complete(data: List[Summary]):

    conns = report['complete']()
    if len(conns) <= 0: return

    all_pkts = [
        pkt
        for conn in list(conns)
        for pkt_list in conn.packets.values()
        for pkt in pkt_list
    ]

    ''' compute the rtt stats '''
    rrts = []
    for summary in data:
        # cast addresses back to int to use as keys in 'all_pkts'
        src = int(summary.src_addr)
        dest = int(summary.dest_addr)

    
    stats = Stats(
        duration_min = min(data, key= lambda s: s.duration).duration,
        duration_max = max(data, key= lambda s: s.duration).duration,
        duration_avg = (
            sum(s.duration[0] for s in data),
            sum(s.duration[1] for s in data)
            ),
        rtt_min = 0,
        rtt_max = 0,
        rtt_avg = 0,

        pkt_min = min(data, key=lambda s: s.packets_total).packets_total,
        pkt_max = max(data, key=lambda s: s.packets_total).packets_total,
        pkt_avg = sum(s.packets_total for s in data) / len(data),

        ws_min = min(all_pkts, key=lambda pkt: pkt.ws).ws,
        ws_max = max(all_pkts, key=lambda pkt: pkt.ws).ws,
        ws_avg = sum(pkt.ws for pkt in all_pkts) / len(all_pkts)
        )
    print('\n'.join([
            f'Minimum time duration: {stats.duration_min} seconds',
            f'Mean time duration: {stats.duration_avg} seconds',
            f'Maximum time duration: {stats.duration_max} seconds',
            '\n',
            f'Minimum RTT value: {stats.rtt_min}',
            f'Mean RTT value: {stats.rtt_avg}',
            f'Maximum RTT value: {stats.rtt_max}',
            '\n',
            f'Minimum number of packets including both send/received: {stats.pkt_min}',
            f'Mean number of packets including both send/received: {stats.pkt_avg}',
            f'Maximum number of packets including both send/received: {stats.pkt_max}'
            '\n',
            f'Minimum receive window size including both send/received: {stats.ws_min} bytes',
            f'Mean receive window size including both send/received: {stats.ws_avg} bytes',
            f'Maximum receive window size including both send/received: {stats.ws_max} bytes'
        ])
    )


def generate_report():
    print(f'A) Total number of connections: {report["conn_cnt"]}')
    print('________________________________________________')
    print('B) Connection\'s Details')
    _generate_report_for('open')
    _generate_report_for('reset')
    summaries = cast(List[Summary], _generate_report_for('complete'))
    print('________________________________________________')
    print('C) General')
    print('\n'.join([
            f'Total number of complete TCP connections: {
                len(report["complete"]())
            }',
            f'Number of reset TCP connections: {len(report["reset"]())}',
            f'Number of TCP connections that were still open when the trace capture ended: {
                len(report["open"]())
            }'
        ])
    )
    print('________________________________________________')
    print('D) Complete TCP connections')
    _compute_stats_for_complete(summaries)


if __name__ == '__main__':
    main()
