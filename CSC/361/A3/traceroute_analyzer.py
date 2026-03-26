import platform
import struct
import sys
from typing import Dict, List, Literal, cast
'''
Send kth datagram with TTL = k
Each router decrements the TTL by at least 1
When the kth TTL = 0 the kth router returns ICMP
We use this to determine the path of routers from req to dest
'''

'''

source node: node executing traceroute
intermediate destination node: router that is not the ultimate destination
ultimate destination node: destination 

Analyze IP datagrams created by traceroute and
determine:
    - IP address of source
    - IP address of destination
    - IP address of intermediates ordered by hop count
    - Set(for each datagram: list values in protocol field (IP headers))
    - Number of fragments from the original datagram
    - Offset in bytes of last fragment

    calculated over all fragments:
        - average and std. dev. of RTT between source and intermediates
        - average RTT between source and destination
'''


''' parse ethernet, IPv4, and TCP/UDP headers '''

endian = "<"
def tcp_parse(pkts: List[Dict[str, float | int | bytes]]):
    ''' 
        tcp header 

    IHL indicates number of 4 byte blocks,
    1. we first extract lower nibble from ipver_ihl
    2. then we multiply x4 to get size in bytes
    3. we use eth_header + ihl as an offset to skip
        eth and ipv4 and begin parsing the tcp header
    '''
    packets = []
    TCP_PROTOCOL = 6
    for pkt in pkts:
        if pkt["protocol"] != TCP_PROTOCOL: continue

        data = cast(bytes, pkt["data"])
        assert len(data) >= 20
        tcp_bytes = data[:20]

        src_port, dest_port, seq_num, \
        ack_num, thl_resbits_flags, \
        ws, checksum, urgent_ptr = \
            struct.unpack(f'{endian} 2H 2I 2H 2H', tcp_bytes)

        pkt_id = (pkt["source"], src_port, pkt["destination"], dest_port)
        thl = ((thl_resbits_flags >> 12) & 0x0F) * 4 # tcp header length in bytes
        res_bits = (thl_resbits_flags >> 6) & 0x3F
        flags = thl_resbits_flags & 0x3F
        # 0011 1111
        packets.append({
            "source_port": src_port, "destination_port": dest_port,
            "seq_num": seq_num,
            "ack_num": ack_num,
            "thl": thl, "res_bits": res_bits, "flags": flags, "win_size": ws,
            "checksum": checksum, "urgent_ptr": urgent_ptr,
            "options": data[20:thl],
            "data": data[thl:]
        })
    return packets

def icmp_parse(pkts: List[Dict[str, float | int | bytes]]):
    packets = []
    ICMP_PROTOCOL = 1
    for pkt in pkts:
        if pkt["protocol"] != ICMP_PROTOCOL: continue

        data = cast(bytes, pkt["data"])
        assert len(data) >= 4
        icmp_bytes = data[:4]



     
def ipv4_parse(data: bytes):
    ''' sizes in bytes '''
    eth_header = 14
    ipv4_header = 20
    tcp_header = 20

    ''' eth header '''
    eth_bytes = data[:eth_header]
    dest_mac, src_mac, eth_type = \
            struct.unpack(f'{endian} 6s 6s H', eth_bytes)

    
    ''' ipv4 header '''
    ipv4_bytes = data[eth_header: eth_header + ipv4_header]

    ipver_ihl, tos, total_len, \
    identification, flags_and_offset, ttl, \
    protocol, checksum, source, destination = \
        struct.unpack(f'{endian} 2B 3H 2B H 2I', ipv4_bytes)

    version = (ipver_ihl & 0xF0) >> 4
    # IHL is long enough to encapsulate options if any
    ihl = (ipver_ihl & 0x0F) * 4
    flags = (flags_and_offset & 0xE000) >> 13
    offset = flags_and_offset & 0x1FFF
    
    # 1110 0000 0000 0000
    return {
        "version": version, "ihl": ihl, "tos": tos, "total_len": total_len,
        "identification": identification, "flags": flags, "offset": offset,
        "ttl": ttl, "protocol": protocol, "checksum": checksum,
        "source": source,
        "destination": destination,
        "data": data[eth_header + ihl:]
        }
    



def read_cap_file():
    LITTLE_ENDIAN = 0xa1b2c3d4
    BIG_ENDIAN = 0xd4c3b2a1

    packets = []
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
                ipv4_pkt = ipv4_parse(pkt_data)
                packets.append(ipv4_pkt)
    return packets

def main():
    ipv4_pkts = read_cap_file()
    tcp_pkts  = tcp_parse(ipv4_pkts)
    icmp_pkts = icmp_parse(ipv4_pkts)
    
    print(len(ipv4_pkts), ipv4_pkts[0])
if __name__ == "__main__":
    main()
