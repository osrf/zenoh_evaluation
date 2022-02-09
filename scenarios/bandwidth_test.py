#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import importlib
import sys
import time
import utils


def ping_test(net, scenario_module):
    print('Doing ping test')
    source, sink = utils.get_source_and_sink(net, scenario_module)
    net.ping((source, sink))


def raw_bandwidth_test(net, scenario_module):
    source, sink = utils.get_source_and_sink(net, scenario_module)

    print('Doing raw bandwidth test (TCP)')
    sink_process = sink.popen('iperf -s -p 5001')
    waitListening(source, sink, 5001, timeout=5)
    sink_process.stdout.readline()
    result = source.cmd('iperf -t 10 -p 5001 -c {}'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)

    print('Doing raw bandwidth test (UDP)')
    sink_process = sink.popen('iperf -s -p 5001 -u')
    time.sleep(2)
    sink_process.stdout.readline()
    result = source.cmd('iperf -t 10 -b 10G -p 5001 -u -c {}'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)


def zenoh_bandwidth_test(net, scenario_module):
    print('Doing zenoh bandwidth test')
    source, sink = utils.get_source_and_sink(net, scenario_module)

    popens = {}
    data_lines = []
    sink_process = sink.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/subscriber')
    #print('\tWaiting for subscriber to start')
    time.sleep(5)
    #print('\tStarting publisher')
    source_process = source.popen('/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug/publisher')
    for host, line in pmonitor({source: source_process, sink: sink_process}, timeoutms=2000):
        if host:
            #print('\t{}:'.format(len(data_lines)), line.strip())
            if host == sink and (line.startswith('16') or line.startswith('Received')):
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
    if len(sys.argv) != 2:
        print('Please supply a scenario name')
        return 1
    scenario_name = sys.argv[1]
    scenario_module = importlib.import_module('.' + scenario_name, 'scenarios')
    topo = scenario_module.ScenarioTopo()
    net = Mininet(topo, host=CPULimitedHost, link=TCLink)
    net.start()
    scenario_module.configure_network(net)
    load = scenario_module.start_network_load(net)
    print('Connections:')
    dumpNodeConnections(net.hosts)

    utils.print_interfaces(net, scenario_module)
    print()
    ping_test(net, scenario_module)
    print()
    #raw_bandwidth_test(net, scenario_module)

    tshark = utils.start_tshark_on_source(net, scenario_module, '/tmp/source_capture.pcap')
    zenoh_bandwidth_test(net, scenario_module)
    utils.stop_tshark(tshark)

    scenario_module.stop_network_load(load)
    net.stop()

    utils.process_zenoh_packet_capture('/tmp/source_capture.pcap')

    return 0


if __name__ == '__main__':
    sys.exit(main())
