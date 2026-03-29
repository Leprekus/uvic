from os import wait
import platform
import struct
import sys
import statistics
from ipaddress import ip_address 
from typing import Dict, List, Literal, cast, dataclass_transform
from collections import defaultdict
from datetime import datetime
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
                timestamp = datetime.fromtimestamp(ts_sec + (ts_usec / 1000000.0))
                ipv4_pkt = ipv4_parse(pkt_data)
                ipv4_pkt["timestamp"] = timestamp
                packets.append(ipv4_pkt)
    return packets

ECHO_REPLY = 0
UNREACHABLE = 3
ECHO_REQ = 8
TIMEOUT = 11

def icmp_parse(pkts, endian='!'):
    parsed_packets = []
    ICMP_PROTOCOL = 1
    UDP_PROTOCOL = 17

    for pkt in pkts:
        proto = pkt.get("protocol")
        data = pkt.get("data", b"")
        timestamp = pkt.get("timestamp")
        src_ip = pkt.get("source")
        
        if proto == ICMP_PROTOCOL:
            if len(data) < 8: continue
            icmp_type, code, checksum, rest = struct.unpack(f'{endian}2BHI', data[:8])
            
            if icmp_type in [ECHO_REQ, ECHO_REPLY]:
                seq_num = rest >> 16 # Sequence is usually high 16 bits of the 'rest' field
                parsed_packets.append({
                    "role": "PROBE" if icmp_type == ECHO_REQ else "FINAL",
                    "proto": "ICMP",
                    "key": seq_num,
                    "src_ip": src_ip,
                    "timestamp": timestamp
                })

            elif icmp_type in [TIMEOUT, UNREACHABLE]:
                # The original packet header starts at data[8]
                if len(data) < 8 + 20 + 8: continue 
                inner_ip_header = data[8:28]
                inner_proto = inner_ip_header[9]
                
                # Original was ICMP (Windows)
                if inner_proto == ICMP_PROTOCOL:
                    # Inner ICMP header starts after Inner IP header (8 + 20)
                    # Sequence number is at offset 6 of the inner ICMP header
                    inner_seq = struct.unpack(f'{endian}H', data[34:36])[0]
                    parsed_packets.append({
                        "role": "HOP_REPLY" if icmp_type == TIMEOUT else "FINAL",
                        "proto": "ICMP",
                        "key": inner_seq,
                        "src_ip": src_ip,
                        "timestamp": timestamp
                    })
                
                # Original was UDP (Linux)
                elif inner_proto == UDP_PROTOCOL:
                    # Inner UDP header starts at data[28]. Dest Port is at offset 2
                    inner_dest_port = struct.unpack(f'{endian}H', data[30:32])[0]
                    parsed_packets.append({
                        "role": "HOP_REPLY" if icmp_type == TIMEOUT else "FINAL",
                        "proto": "UDP",
                        "key": inner_dest_port,
                        "src_ip": src_ip,
                        "timestamp": timestamp
                    })

        elif proto == UDP_PROTOCOL:
            if len(data) < 8: continue
            src_port, dest_port = struct.unpack('!HH', data[:4])
            if 33434 <= dest_port <= 33534:
                parsed_packets.append({
                    "role": "PROBE",
                    "proto": "UDP",
                    "key": dest_port,
                    "src_ip": src_ip,
                    "dst_ip": pkt.get("destination"),
                    "timestamp": timestamp
                })

    return parsed_packets

def icmp_map(parsed_packets):
    mapping = {}
    for pkt in parsed_packets:
        key = pkt["key"]
        if pkt["role"] == "PROBE":
            mapping[key] = {"probe": pkt, "replies": []}
        elif pkt["role"] in ["HOP_REPLY", "FINAL"]:
            if key in mapping:
                mapping[key]["replies"].append(pkt)
    return dict(sorted(mapping.items()))

def compute_traceroute_stats(origin, sorted_mapping):
    hop_stats = {} # { router_ip: [rtts] }

    for key, data in sorted_mapping.items():
        probe = data["probe"]
        for reply in data["replies"]:
            rtt = (reply["timestamp"] - probe["timestamp"]) * 1000
            rtt = rtt.total_seconds()
            ip = reply["src_ip"]
            if ip not in hop_stats: hop_stats[ip] = []
            hop_stats[ip].append(rtt)

    output = ""
    for ip, rtts in hop_stats.items():
        avg_rtt = sum(rtts) / len(rtts)
        std_dev = statistics.stdev(rtts) if len(rtts) > 1 else 0.0
        output += f"\nThe avg RTT between {
            ip_address(origin)
        } and {
            ip_address(ip)
        } is {
           avg_rtt 
        } ms, the s.d is: {
           std_dev 
        } ms" 
    
    return output
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
        dest = pkt.get('dest_ip')
        proto = pkt.get('proto')
        ip_id = pkt.get('id')
        
        # Identification '0' usually means fragmentation is not being used
        if ip_id is not None:
            group_key = (src, dest, proto, ip_id)
            datagram_fragments[group_key] += 1

    last_frag_offset = 0
    total_fragments = 0

    for pkt in ipv4_pkts:
        offset = pkt["offset"]  
        mf_flag = pkt["flags"] & 0x1 
        
        if offset > 0 or mf_flag == 1:
            total_fragments += 1
            
            if mf_flag == 0 and offset > 0:
                last_frag_offset = offset

    # Convert to actual bytes if needed
    last_byte_start = last_frag_offset * 8
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
           total_fragments 
        }",
        f"The offset of the last fragment is: {last_frag_offset}",
        compute_traceroute_stats(src_node, icmp_mapping)
    ])) 

    

if __name__ == "__main__":
    main()
