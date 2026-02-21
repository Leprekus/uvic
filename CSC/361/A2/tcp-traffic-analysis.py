import sys
import struct
from typing import Literal, Tuple
from enum import Enum
from dataclasses import dataclass

LITTLE_ENDIAN = 0xa1b2c3d4
BIG_ENDIAN = 0xd4c3b2a1

class Segment(Enum):
    URG = 1 << 5
    ACK = 1 << 4
    PSH = 1 << 3
    RST = 1 << 2
    SYN = 1 << 1
    FIN = 1 << 0

@dataclass(frozen=True)
class Summary:
    conn_id: Tuple[int, int, int] # contains (src_ip, src_port, dest_addr)
    connection: int
    src_addr: int
    dest_addr: int
    src_port: int
    dest_port: int
    status: Literal['S0F0', 'S2F2', 'R']
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
summaries = {}
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

        if magic == BIG_ENDIAN:
            magic, version_major, version_minor, \
            thiszone, sigfigs, snaplen, network = \
            struct.unpack('>IHHIIII', global_header)
            endian = '>'
        else:
            endian = '<'

        while True:
            pkt_header = f.read(16)
            pkt_data   = b''
            if not pkt_header: break
            if len(pkt_header) != 16:
                raise Exception(f'Truncated packet header: {len(pkt_header)} bytes')

            ''' packet header '''
            ts_sec, ts_usec, incl_len, orig_len = \
                    struct.unpack(f'{endian}IIII', pkt_header)  

            ''' packet data ''' 
            if incl_len:
               pkt_data = f.read(incl_len)

            if len(pkt_data) != incl_len:
                raise Exception(f'Packet data bytes mismatch, expected {incl_len}, got: {len(pkt_data)}')

            if incl_len > 0:
                summary = pkt_data_parse(incl_len, pkt_data, endian)
                summaries[summary.conn_id] = summary

            record = pkt_header, pkt_data


''' parse ethernet, IPv4, and TCP/UDP headers '''
def pkt_data_parse(size: int, data: bytes, 
                   endian: Literal['>', '<']) -> Summary:

    ''' sizes in bytes '''
    eth_header = 14
    ipv4_header = 20
    tcp_header = 20
    assert len(data) >= eth_header + ipv4_header
    data_bytes = \
            struct.unpack(f'{endian} 6s 6s H 2B 3H 2B H 2I', data)
    dest_mac, src_mac, eth_type, \
    ip_ver_ihl, service_type, total_len, \
    identification, flags_and_offset, ttl, \
    protocol, checksum, src_ip, dest_ip = data_bytes

    flags = 0

    conn_id = (src_ip, 0, dest_ip)
    if conn_id in summaries:
        # TODO: handle updates
        summary = summaries[conn_id]
    else: 
        summary = Summary(
            conn_id = (0, 0, 0),
            connection    = 0,
            src_addr      = 0,
            dest_addr     = 0,
            src_port      = 0,
            dest_port     = 0,
            status        = 'R',
            start         = 0,
            end           = 0,
            duration      = 0,
            packets_sent  = 0,
            packets_recv  = 0,
            packets_total = 0,
            bytes_sent    = 0,
            bytes_recv    = 0,
            bytes_total   = 0,
            )
    return summary

if __name__ == '__main__':
    main()

