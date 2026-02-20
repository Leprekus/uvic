import sys
import struct

LITTLE_ENDIAN = 0xa1b2c3d4
BIG_ENDIAN = 0xd4c3b2a1
'''
    network byte ordering is big endian
'''
def main():
    if len(sys.argv) != 2:
        raise Exception('Usage ./<filename> <path-to-cap-file>')

    with open(sys.argv[1], 'rb') as f:
        global_header = f.read(24)
        magic, version_major, version_minor, thiszone, sigfigs, snaplen, network = \
                struct.unpack('<IHHIIII', global_header) # little-endian

        if magic == BIG_ENDIAN:
            magic, version_major, version_minor, thiszone, sigfigs, snaplen, network = \
            struct.unpack('>IHHIIII', global_header)
            endian = '>'
        else:
            endian = '<'

        while True:
            record = f.read(16)

            if not record: break
            if len(record) < 16:
                raise Exception(f'Truncated packet header: {len(record)} bytes')

            ts_sec, ts_usec, incl_len, orig_len = \
                    struct.unpack(f'{endian}IIII', record)  
            print(f'''
                ts_sec    ts_usec    incl_len    orig_len
                {ts_sec}    {ts_usec}    {incl_len}    {orig_len}
                ''')


            
if __name__ == '__main__':
    main()
    
