from os import wait
import platform
import struct
import sys
from ipaddress import ip_address 
from typing import Dict, List, Literal, cast
from collections import defaultdict
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

'''

''' parse ethernet, IPv4, and TCP/UDP headers '''

endian = ">"
ipv4_protocols = {
    0: "HOPOPT",
    1: "ICMP",
    2: "IGMP",
    3: "GGP",
    4: "IPv4",
    5: "ST",
    6: "TCP",
    7: "CBT",
    8: "EGP",
    9: "IGP",
    10: "BBN-RCC-MON",
    11: "NVP-II",
    12: "PUP",
    17: "UDP",
    22: "IDP",
    41: "IPv6",
    43: "IPv6-Route",
    44: "IPv6-Frag",
    46: "RSVP",
    47: "GRE",
    50: "ESP",
    51: "AH",
    58: "IPv6-ICMP",
    59: "IPv6-NoNxt",
    60: "IPv6-Opts",
    88: "EIGRP",
    89: "OSPF",
    103: "PIM",
    115: "L2TP",
    132: "SCTP"
}
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

ICMP_PROTOCOL = [1, 17]
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

ECHO_REQ = 8
TIMEOUT = 11
ECHO_REPLY = 0
UNREACHABLE = 3
def icmp_parse(pkts):
    packets = []
    for pkt in pkts:
        data = cast(bytes, pkt["data"])
        if len(data) < 8: continue
        icmp_header = data[:8]
        icmp_type, code, checksum, rest_of_header =  \
            struct.unpack(f'{endian} 2B H I', icmp_header)
        # probe
        if icmp_type == ECHO_REQ:
            seq_num = rest_of_header & 0xFFFF
            packets.append({
                "role": "PROBE", 
                "seq_num": seq_num
                })
        # intermediate link
        elif icmp_type == TIMEOUT:
            seq_num = struct.unpack(f'{endian}H', data[34:36])[0]
            packets.append({
                "role": "HOP_REPLY", 
                "type": "TIMEOUT", 
                "seq_num": seq_num,
                })
        # final dest for windows
        elif icmp_type == ECHO_REPLY:
            seq_num = rest_of_header & 0xFFFF
            packets.append({
                "role": "FINAL_REACHED", 
                "proto": "ICMP", 
                "seq_num": seq_num,
                })
        # final dest for linux
        elif icmp_type == UNREACHABLE:
            src_port = struct.unpack(f'{endian}H', data[28:30])[0]
            packets.append({
                "role": "FINAL_REACHED", 
                "proto": "UDP/PortUnreach", 
                "src_port": src_port,
                })
    return packets

'''
windows:
- map: source seq to the seq number in the payload reply, and 
look at source's TTL for that request

linux:
- source port to the source port in the payload reply, and
look at source's TTL for that request

'''
def icmp_map(parsed_packets):
    """
    Maps ICMP/UDP replies back to their original probes using 
    sequence numbers or source ports.
    """
    # Key: identifier (seq_num or src_port), Value: list of responses
    mapping = {}
    probes = {}

    # First pass: identify and store probes
    for pkt in parsed_packets:
        if pkt["role"] == "PROBE":
            seq = pkt["seq_num"]
            probes[seq] = pkt
            mapping[seq] = []  # Initialize the response list for this probe

    # Second pass: link replies to probes
    for pkt in parsed_packets:
        role = pkt["role"]
        
        if role in ["HOP_REPLY", "FINAL_REACHED"]:
            # ICMP Traceroute uses seq_num
            if "seq_num" in pkt:
                key = pkt["seq_num"]
            # Linux UDP Traceroute uses src_port to identify the probe
            elif "src_port" in pkt:
                key = pkt["src_port"]
            else:
                continue

            if key in mapping:
                mapping[key].append(pkt)
                
    return mapping
'''
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
def main():
    ipv4_pkts = read_cap_file()
    tcp_pkts  = tcp_parse(ipv4_pkts)
    icmp_pkts = icmp_parse(ipv4_pkts)
    icmp_mapping = icmp_map(icmp_pkts)

    src_node  = str(ip_address(ipv4_pkts[0]["source"]))
    dest_node = str(ip_address(ipv4_pkts[0]["destination"]))
    intermediate_nodes = set()
    key = lambda pkt: pkt["data"]
    for pkt in ipv4_pkts:
        data = cast(bytes, pkt["data"])
        if len(data) >= 8 and data[0] == 11: # 11 is the TIMEOUT type
            intermediate_nodes.add(
                    str(ip_address(pkt["source"])))
    intermediates_str = "\n" + "\n".join([f"router {i+1}: {node}" 
        for i, node in enumerate(list(intermediate_nodes))])

        
    proto_map_str = lambda pkt: \
            str(pkt["protocol"]) + ": " + ipv4_protocols[pkt["protocol"]]
    proto_str = "\n" + "\n".join(set(map(proto_map_str, ipv4_pkts)))
    datagram_fragments = defaultdict(int)
    for pkt in ipv4_pkts:
        # Assuming pkt is a dictionary or object with these attributes
        # We include IP and Protocol to ensure the ID is unique to a specific flow
        src = pkt.get('src_ip')
        dst = pkt.get('dst_ip')
        proto = pkt.get('proto')
        ip_id = pkt.get('id')
        
        # Identification '0' usually means fragmentation is not being used
        if ip_id is not None:
            group_key = (src, dst, proto, ip_id)
            datagram_fragments[group_key] += 1
    print("\n".join([
        f"The IP address of the source node: {src_node}",
        f"The IP address of the destination node: {dest_node}",
        f"The IP addresses of the intermediate nodes: {
           intermediates_str 
        }",
        "\n",
        f"The values in the protocol field of IP headers: {
            proto_str
        }",
        f"The number of fragments created from the original datagram is: {
            0
        }"
        f"The offset of the last fragment is: {0}",
        f"The avg RTT between {0} and {0} is {0} ms, the s.d is: {0} ms",
    ])) 
    

if __name__ == "__main__":
    main()
