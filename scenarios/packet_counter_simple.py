#!/usr/bin/python3

from signal import SIGINT
import subprocess
import sys
import time
import utils


def zenoh_bandwidth_test():
    data_lines = []
    sink_process = subprocess.Popen(
        ['../bandwidth_test/zenoh/target/debug/subscriber'],
        stdout=subprocess.PIPE,
        universal_newlines=True)
    time.sleep(1)
    source_process = subprocess.Popen(
        ['../bandwidth_test/zenoh/target/debug/publisher'],
        stdout=subprocess.PIPE,
        universal_newlines=True)
    for line in sink_process.stdout:
        #print('\t{}:'.format(len(data_lines)), line.strip())
        if line.startswith('16') or line.startswith('Received'):
            data_lines.append(line)
            print(len(data_lines) - 1, end=' ', flush=True)
        if len(data_lines) >= 11:
            print()
            break
    source_process.send_signal(SIGINT)
    sink_process.send_signal(SIGINT)
    print('Completed; accumulated data:')
    for l in data_lines:
        print(l.strip())


def main():
    tshark = utils.start_tshark_native('/tmp/source_capture.pcap', 'any')
    time.sleep(2)
    zenoh_bandwidth_test()
    time.sleep(2)
    utils.stop_tshark(tshark)

    utils.process_zenoh_packet_capture('/tmp/source_capture.pcap')

    return 0


if __name__ == '__main__':
    sys.exit(main())
