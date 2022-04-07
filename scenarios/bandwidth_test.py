#!/usr/bin/python3

from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections, waitListening, decode, pmonitor
from signal import SIGINT
import importlib
import os.path
import sys
import time
import utils


def ping_test(net, scenario_module):
    print('Doing ping test')
    source, sink = utils.get_source_and_sink(net, scenario_module)
    net.ping((source, sink))


def raw_bandwidth_test(net, scenario_module):
    source, sink = utils.get_source_and_sink(net, scenario_module)

    print('Raw bandwidth test (TCP)')
    sink_process = sink.popen('iperf -s -p 5001 -e')
    waitListening(source, sink, 5001, timeout=5)
    sink_process.stdout.readline()
    result = source.cmd('iperf -t 10 -p 5001 -c {} -e'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)

    print('Raw bandwidth test (UDP)')
    sink_process = sink.popen('iperf -s -p 5001 -u -e')
    time.sleep(2)
    sink_process.stdout.readline()
    result = source.cmd('iperf -t 10 -b 10G -p 5001 -u -c {} -e'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)


def zenoh_bandwidth_test(net, scenario_module):
    print('Zenoh bandwidth test')
    source, sink = utils.get_source_and_sink(net, scenario_module)

    popens = {}
    data_lines = []
    root_dir = '/home/mininet/zenoh_evaluation/bandwidth_test/zenoh/target/debug'
    sink_process = sink.popen(os.path.join(root_dir, 'subscriber'))
    time.sleep(2)
    source_process = source.popen(os.path.join(root_dir, 'publisher'))
    for host, line in pmonitor({source: source_process, sink: sink_process}, timeoutms=2000):
        if host:
            #print('\t{}:'.format(len(data_lines)), line.strip())
            if host == sink and (line.startswith('16') or line.startswith('Received')):
                data_lines.append(line)
                print(len(data_lines) - 1, end=' ', flush=True)
        if len(data_lines) >= 21:
            print()
            break
    source_process.send_signal(SIGINT)
    sink_process.send_signal(SIGINT)
    print('Completed; accumulated data:')
    for l in data_lines:
        print(l.strip())


def fastdds_bandwidth_test(net,scenario_module):
    print('FastDDS bandwidth test')
    source, sink = utils.get_source_and_sink(net, scenario_module)

    popens = {}
    data_lines = []
    root_dir = '/home/mininet/zenoh_evaluation/bandwidth_test/dds'
    sink_process = sink.popen(
        os.path.join(root_dir, 'build/bin/subscriber'),
        env={'FASTRTPS_DEFAULT_PROFILES_FILE': '{}'.format(os.path.join(root_dir, 'socket_size.xml'))},
        )
    source_process = source.popen(
        os.path.join(root_dir, 'build/bin/publisher'),
        env={'FASTRTPS_DEFAULT_PROFILES_FILE': '{}'.format(os.path.join(root_dir, 'socket_size.xml'))},
        )
    for host, line in pmonitor({source: source_process, sink: sink_process}, timeoutms=2000):
        if host:
            #print('\t{}-->{}:'.format(host, len(data_lines)), line)
            if host == sink and (line.startswith('16') or line.startswith('Received')):
                data_lines.append(line)
                print(len(data_lines) - 1, end=' ', flush=True)
        if len(data_lines) >= 21:
            print('')
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
    raw_bandwidth_test(net, scenario_module)

    tshark = utils.start_tshark_on_source(net, scenario_module, '/tmp/zenoh_source_capture.pcap')
    zenoh_bandwidth_test(net, scenario_module)
    utils.stop_tshark(tshark)

    tshark = utils.start_tshark_on_source(net, scenario_module, '/tmp/dds_source_capture.pcap')
    fastdds_bandwidth_test(net, scenario_module)
    utils.stop_tshark(tshark)

    scenario_module.stop_network_load(load)
    net.stop()

    #utils.process_zenoh_packet_capture('/tmp/zenoh_source_capture.pcap')
    #utils.process_dds_packet_capture('/tmp/dds_source_capture.pcap')

    return 0


if __name__ == '__main__':
    sys.exit(main())
