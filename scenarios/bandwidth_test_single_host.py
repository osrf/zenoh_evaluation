#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import time

from scenarios import scenario1


def ping_test(net):
    print('Doing ping test')
    workstation = net.get('h1')
    net.ping((workstation, workstation))


def raw_bandwidth_test(net):
    print('Doing raw bandwidth test')
    workstation = net.get('h1')

    workstation_process = workstation.popen('iperf -s -p 5001')
    waitListening(workstation, workstation, 5001)
    workstation_process.stdout.readline()
    result = workstation.cmd('iperf -t 10 -c {}'.format(workstation.IP()))
    print(result)
    workstation_process.send_signal(SIGINT)


def zenoh_bandwidth_test(net):
    print('Doing zenoh bandwidth test')
    workstation = net.get('h1')

    popens = {}
    data_lines = []
    subscriber_process = workstation.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/subscriber')
    print('\tWaiting for subscriber to start')
    time.sleep(5)
    print('\tStarting publisher')
    publisher_process = workstation.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/publisher')
    for host, line in pmonitor({workstation: subscriber_process}, timeoutms=2000):
        if host:
            print('\tData received:', line)
            data_lines.append(line)
        if len(data_lines) >= 11:
            break
    publisher_process.send_signal(SIGINT)
    subscriber_process.send_signal(SIGINT)
    print('\tCompleted; accumulated data:')
    for l in data_lines:
        print(l.strip())


def main():
    topo = scenario1.Scenario1Topo()
    net = Mininet(topo, host=CPULimitedHost, link=TCLink)
    net.start()
    print('Connections:')
    dumpNodeConnections(net.hosts)
    ping_test(net)
    raw_bandwidth_test(net)
    zenoh_bandwidth_test(net)
    net.stop()


if __name__ == '__main__':
    main()
