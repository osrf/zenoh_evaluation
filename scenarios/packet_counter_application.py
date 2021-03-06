#!/usr/bin/python3

from signal import SIGINT
import os
import selectors
import subprocess
import sys
import time
import utils


def node_names(process_set):
    node_names_1 = [
        'arequipa',
        'barcelona',
        'cordoba',
        'delhi',
        'freeport',
        'geneva',
        'georgetown',
        'hamburg',
        'hebron',
        'kingston',
        ]
    node_names_2 = [
        'lyon',
        'mandalay',
        'medellin',
        'monaco',
        'osaka',
        'ponce',
        'portsmouth',
        'rotterdam',
        'taipei',
        'tripoli',
        ]
    if process_set == '1':
        return node_names_1
    elif process_set == '2':
        return node_names_2
    else:
        return node_names_1 + node_names_2


def start_processes(selector, process_set):
    processes = {}
    for node_name in node_names(process_set):
        processes[node_name] = subprocess.Popen(
            ['../mont_blanc/zenoh/target/debug/' + node_name],
            stdout=subprocess.PIPE,
            universal_newlines=True)
        selector.register(
            processes[node_name].stdout,
            selectors.EVENT_READ,
            data=node_name)
    return processes


def terminate_processes(processes):
    for p in processes:
        processes[p].send_signal(SIGINT)


def process_line(processes, line):
    ignore_line = False
    line_is_expected = False
    started = False

    splits = line.split(':', maxsplit=1)
    if len(splits) == 2:
        source_process, output = splits
        source_process = source_process.lower().strip()
        output = output.strip()

        if output == 'Starting loop':
            started = True
            ignore_line = True
        elif output in ['Data generation started', 'Data generation done']:
            ignore_line = True
        else:
            if source_process in processes:
                line_is_expected = True
            else:
                line_is_expected = False
    else:
        print(line)

    return ignore_line, line_is_expected, started


def application_test(process_set):
    selector = selectors.DefaultSelector()
    processes = start_processes(selector, process_set)

    not_started_processes = list(processes.keys())
    started_processes = []
    expected_lines = {}
    unexpected_lines = {}

    start_time = time.time()
    started = False

    while True:
        events = selector.select(timeout=2)
        for key, mask in events:
            node_name = key.data
            line = key.fileobj.readline().strip()
            if len(line) == 0:
                continue

            ignore, is_expected, has_started = process_line(
                processes.keys(),
                line)
            if not ignore:
                if is_expected:
                    if line in expected_lines:
                        expected_lines[line] = expected_lines[line] + 1
                    else:
                        expected_lines[line] = 1
                else:
                    if line in unexpected_lines:
                        unexpected_lines[line] = unexpected_lines[line] + 1
                    else:
                        unexpected_lines[line] = 1

            if has_started:
                not_started_processes.remove(node_name)
                started_processes.append(node_name)
                print('Started processes ({}): {}'.format(len(started_processes), started_processes))
                print('Waiting for processes ({}): {}'.format(len(not_started_processes), not_started_processes))

            if len(not_started_processes) == 0 and not started:
                print('All nodes started')
                started = True
                start_time = time.time()
        if started and (time.time() - start_time) > 30:
            break
    print('Run time exceeded; terminating')
    terminate_processes(processes)
    print("Received {} lines from processes:".format(len(expected_lines)))
    for l in expected_lines:
        print("{}\t{}".format(expected_lines[l], l))
    print("Received {} lines from unknown processes:".format(len(unexpected_lines)))
    for l in unexpected_lines:
        print("{}\t{}".format(unexpected_lines[l], l))


def main():
    tshark = utils.start_tshark_native('/tmp/application_capture.pcap', 'any')
    time.sleep(2)
    process_set = 'full'
    if len(sys.argv) > 1:
        process_set = sys.argv[1]
    application_test(process_set)
    time.sleep(2)
    utils.stop_tshark(tshark)

    utils.process_zenoh_packet_capture('/tmp/application_capture.pcap', 1)

    return 0


if __name__ == '__main__':
    sys.exit(main())
