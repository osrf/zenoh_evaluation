#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import importlib
import sys
import time


def application_test(net):
    workstation, robot = net.get('h1', 'w1')

    robot_process = robot.popen('/home/mininet/zenoh_evaluation/mont_blanc/zenoh/robot_application.sh')
    workstation_process = workstation.popen('/home/mininet/zenoh_evaluation/mont_blanc/zenoh/workstation_application.sh')

    robot_processes = ['cordoba',
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
        'ponce']
    workstation_processes = ['geneva',
        'monaco',
        'rotterdam',
        'barcelona',
        'arequipa',
        'georgetown']
    not_started_processes = robot_processes + workstation_processes
    started_processes = []
    received_data_processes = []
    robot_received_lines = {}
    workstation_received_lines = {}
    unknown_process_received_lines = {}
    start_time = time.time()
    started = False
    for host, line in pmonitor({robot: robot_process, workstation: workstation_process}, timeoutms=2000):
        if host:
            line = line.strip()
            splits = line.split(':', maxsplit=1)
            if len(splits) != 2:
                print(line.strip())
                continue
            source_process, output = splits
            source_process = source_process.lower().strip()
            output = output.strip()
            if output not in ['Data generation started', 'Data generation done', 'Starting loop']:
                if source_process in robot_processes:
                    if line in robot_received_lines:
                        robot_received_lines[line] = robot_received_lines[line] + 1
                    else:
                        robot_received_lines[line] = 1
                elif source_process in workstation_processes:
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
                if output == 'Starting loop':
                    not_started_processes.remove(source_process)
                    started_processes.append(source_process)
                    print('Started processes ({}): {}'.format(len(started_processes), started_processes))
                    print('Waiting for processes ({}): {}'.format(len(not_started_processes), not_started_processes))
                else:
                    print(line)
            elif len(not_started_processes) == 0 and not started:
                started = True
                start_time = time.time()
            if started:
                print(line)
        if started and (time.time() - start_time) > 60:
            break
    print('Run time exceeded; terminating')
    robot_process.send_signal(SIGINT)
    workstation_process.send_signal(SIGINT)
    print("Received {} lines from robot processes:".format(len(robot_received_lines)))
    for l in robot_received_lines:
        print("{}\t{}".format(robot_received_lines[l], l))
    print("Received {} lines from workstation processes:".format(len(workstation_received_lines)))
    for l in workstation_received_lines:
        print("{}\t{}".format(workstation_received_lines[l], l))
    print("Received {} lines from unknown processes:".format(len(unknown_process_received_lines)))
    for l in unknown_process_received_lines:
        print("{}\t{}".format(unknown_process_received_lines[l], l))


def main():
    if len(sys.argv) != 2:
        print('Please supply a scenario name')
        return 1
    scenario_name = sys.argv[1]
    scenario_module = importlib.import_module('.' + scenario_name, 'scenarios')
    topo = scenario_module.ScenarioTopo()
    net = Mininet(topo, host=CPULimitedHost, link=TCLink)
    net.start()
    load = scenario_module.start_network_load(net)
    print('Connections:')
    dumpNodeConnections(net.hosts)
    application_test(net)
    scenario_module.stop_network_load(load)
    net.stop()
    return 0


if __name__ == '__main__':
    sys.exit(main())
