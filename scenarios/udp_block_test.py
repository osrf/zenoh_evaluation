#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import importlib
import sys
import time


def get_source_and_sink(net, module):
    return net.get(module.source_name, module.sink_name)


def ping_test(net, scenario_module):
    print('Doing ping test')
    source, sink = get_source_and_sink(net, scenario_module)
    net.ping((source, sink))


def raw_bandwidth_test(net, scenario_module):
    print('Doing raw bandwidth test')
    source, sink = get_source_and_sink(net, scenario_module)

    sink_process = sink.popen('iperf -s -p 5001')
    waitListening(source, sink, 5001, timeout=10)
    sink_process.stdout.readline()
    result = source.cmd('iperf -t 10 -b 1G -p 5001 -c {}'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)


def zenoh_bandwidth_test(net, scenario_module):
    print('Doing zenoh bandwidth test')
    source, sink = get_source_and_sink(net, scenario_module)

    popens = {}
    data_lines = []
    sink_process = sink.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/subscriber')
    print('\tWaiting for subscriber to start')
    time.sleep(5)
    print('\tStarting publisher')
    source_process = source.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/publisher')
    for host, line in pmonitor({source: source_process, sink: sink_process}, timeoutms=2000):
        if host:
            print('\t{}:'.format(len(data_lines)), line)
            if host == sink and (line.startswith('16') or line.startswith('Received')):
                data_lines.append(line)
        if len(data_lines) >= 11:
            break
    source_process.send_signal(SIGINT)
    sink_process.send_signal(SIGINT)
    print('\tCompleted; accumulated data:')
    for l in data_lines:
        print(l.strip())


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
    while True:
        ping_test(net, scenario_module)
        raw_bandwidth_test(net, scenario_module)
    scenario_module.stop_network_load(load)
    net.stop()
    return 0


if __name__ == '__main__':
    sys.exit(main())
