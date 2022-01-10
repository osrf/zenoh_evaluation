#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import time

from scenarios import scenario1


def application_test(net):
    workstation = net.get('h1')

    workstation_process = workstation.popen('/home/mininet/zenoh_evaluation/mont_blanc/zenoh/all_in_one_application.sh 2>&1')

    workstation_processes = ['cordoba',
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
        'georgetown']
    not_started_processes = workstation_processes
    started_processes = []
    workstation_received_lines = {}
    unknown_process_received_lines = {}
    start_time = time.time()
    started = False
    for host, line in pmonitor({workstation: workstation_process}, timeoutms=2000):
        if host:
            line = line.strip()
            source_process = line.split(':')
            if source_process:
                source_process = source_process[0].lower()
            else:
                source_process = 'None'
            if source_process in workstation_processes:
                if line in workstation_received_lines:
                    workstation_received_lines[line] = workstation_received_lines[line] + 1
                else:
                    workstation_received_lines[line] = 1
            else:
                if line in unknown_process_received_lines:
                    unknown_process_received_lines[line] = unknown_process_received_lines[line] + 1
                else:
                    unknown_process_received_lines[line] = 1
            if source_process in not_started_processes:
                not_started_processes.remove(source_process)
                started_processes.append(source_process)
                print('Started processes ({}): {}'.format(len(started_processes), started_processes))
                print('Waiting for processes ({}): {}'.format(len(not_started_processes), not_started_processes))
            elif len(not_started_processes) == 0 and not started:
                started = True
                start_time = time.time()
            if started:
                print(line)
        if started and (time.time() - start_time) > 15:
            break
    print('Run time exceeded; terminating')
    workstation_process.send_signal(SIGINT)
    print("Received {} lines from workstation processes:".format(len(workstation_received_lines)))
    for l in workstation_received_lines:
        print("{}\t{}".format(workstation_received_lines[l], l))
    print("Received {} lines from unknown processes:".format(len(unknown_process_received_lines)))
    for l in unknown_process_received_lines:
        print("{}\t{}".format(unknown_process_received_lines[l], l))


def main():
    topo = scenario1.Scenario1Topo()
    net = Mininet(topo, host=CPULimitedHost, link=TCLink)
    net.start()
    print('Connections:')
    dumpNodeConnections(net.hosts)
    application_test(net)
    net.stop()


if __name__ == '__main__':
    main()
