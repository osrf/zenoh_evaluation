#!/usr/bin/python3

import os
import os.path
import selectors
import subprocess
import sys
import time
import utils


def main():
    if len(sys.argv) != 2:
        print('Please supply a pcap file name prefix')
        return 1

    utils.process_packet_capture(
        sys.argv[1] + 'robot_capture.pcap',
        1,
        'dds')
    utils.process_packet_capture(
        sys.argv[1] + 'ws_capture.pcap',
        1,
        'dds')

    return 0


if __name__ == '__main__':
    sys.exit(main())
