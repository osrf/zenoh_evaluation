#!/usr/bin/python3

from signal import SIGINT
import os
import selectors
import subprocess
import sys
import time
import utils


def start_processes(selector, executables, robot_number):
    processes = {}
    for executable in executables:
        process_key = '{}_{}'.format(executable, robot_number)
        processes[process_key] = subprocess.Popen(
            ['../fms/zenoh/target/debug/' + executable,
             str(robot_number)],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            universal_newlines=True)
        selector.register(
            processes[process_key].stdout,
            selectors.EVENT_READ,
            data=process_key)
    return processes


def start_processes_no_number(selector, executables):
    processes = {}
    for executable in executables:
        processes[executable] = subprocess.Popen(
            ['../fms/zenoh/target/debug/' + executable],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            universal_newlines=True)
        selector.register(
            processes[executable].stdout,
            selectors.EVENT_READ,
            data=executable)
    return processes


def terminate_processes(processes):
    for k in processes:
        processes[k].send_signal(SIGINT)


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


def application_test(robot_count, start_fms=True):
    selector = selectors.DefaultSelector()

    robot_executables = [
        'cordoba',
        'lyon',
        'freeport',
        'medellin',
        'portsmouth',
        'delhi',
        'hamburg',
        'taipei',
        'osaka',
        'hebron',
        'kingston',
        'tripoli',
        'mandalay',
        'ponce',
        'geneva',
        'monaco',
        'rotterdam',
        'barcelona',
        'arequipa',
        'georgetown',
        'status_reporter']
    fms_executables = ['fms']

    processes = {}
    for robot_number in range(1, robot_count + 1):
        processes.update(start_processes(
            selector,
            robot_executables,
            str(robot_number)))
    if start_fms:
        processes.update(start_processes_no_number(
            selector,
            fms_executables))

    not_started_processes = list(processes.keys())
    started_processes = []
    expected_lines = {}
    unexpected_lines = {}

    start_time = time.time()
    started = False

    while True:
        events = selector.select(timeout=2)
        for key, mask in events:
            process = key.data
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
                not_started_processes.remove(process)
                started_processes.append(process)
                print('Started processes ({}): {}'.format(len(started_processes), started_processes))
                print('Waiting for processes ({}): {}'.format(len(not_started_processes), not_started_processes))

            if len(not_started_processes) == 0 and not started:
                print('All nodes started')
                started = True
                start_time = time.time()
        if started and (time.time() - start_time) > 10:
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
    if len(sys.argv) != 2:
        print('Please supply a robot count')
        return 1
    robot_count = int(sys.argv[1])
    start_fms = True
    if len(sys.argv) == 3 and sys.argv[2] == 'nofms':
        start_fms = False

    tshark = utils.start_tshark_native(
        '/tmp/application_capture.pcap',
        'any')
    time.sleep(2)
    application_test(robot_count, start_fms)
    time.sleep(2)
    utils.stop_tshark(tshark)

    utils.process_zenoh_packet_capture(
        '/tmp/application_capture.pcap',
        robot_count)

    return 0


if __name__ == '__main__':
    sys.exit(main())
