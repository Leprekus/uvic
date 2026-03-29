### Usage

Run the script from the command line, passing the path to your packet capture file as an argument:
`python traceroute_analyzer.py your_capture_file.cap`
### Output Example

The script provides a detailed summary including:

    Source/Destination IPs: The endpoints of the traceroute.

    Intermediate Nodes: A list of every router encountered in the path.

    Protocol Field Values: All IP-layer protocols identified (e.g., ICMP, UDP, TCP).

    Fragmentation Stats: Number of fragments and the offset of the final fragment.

    Performance Metrics: Average RTT and standard deviation for each hop.
