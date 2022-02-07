#!/usr/bin/python3


from mininet.util import pmonitor 
from mn_wifi.cli import CLI

from signal import SIGINT
import sys
import time
import os

from scenarios import scenarios_mnw

source_name = 'w1'
sink_name = 'r1'


def get_nodes(net):
	nodes = net.nameToNode.keys()
	return nodes


def get_source_and_sink(net):
	for key in net.nameToNode.keys():
		if key == source_name:
			source = net.nameToNode[key]
		if key == sink_name:
			sink = net.nameToNode[key]
	return source, sink


def ping_test(net):
    print('Doing ping test')
    source, sink = get_source_and_sink(net)
    net.ping(hosts=[source, sink])


def raw_bandwidth_test(net):
    print('Doing raw bandwidth test')
    source, sink = get_source_and_sink(net)
    sink_process = sink.popen('iperf3 -s -p 5001')
    # wait for short while for iperf to start
    time.sleep(2)
    print(' <---- Running TCP test ----> ')
    result = source.cmd('iperf3 -p 5001 -t 10 -c {}'.format(sink.IP()))
    print(result)
    time.sleep(2)
    print(' <---- Running UDP test ----> ')
    result = source.cmd('iperf3 -p 5001 -b 200M -t 10 -u -c {}'.format(sink.IP()))
    print(result)
    sink_process.send_signal(SIGINT)


def zenoh_bandwidth_test(net):
    print('Doing zenoh bandwidth test')
    source, sink = get_source_and_sink(net)

    popens = {}
    data_lines = []
    home_path = '/home/' + os.getenv("SUDO_USER")
    binary_path = home_path + '/zenoh_evaluation/bandwidth_test/zenoh/target/debug'
    sink_process = sink.popen(binary_path + '/subscriber')
    print('\tWaiting for subscriber to start')
    time.sleep(5)
    print('\tStarting publisher')
    source_process = source.popen(binary_path + '/publisher')
    for host, line in pmonitor({source: source_process, sink: sink_process}, timeoutms=2000):
        if host:
            print('\t{}-->{}:'.format(host,len(data_lines)), line)
            if host == sink and (line.startswith('16') or line.startswith('Received')):
                data_lines.append(line)
        if len(data_lines) >= 11:
        	print('data_lines >= 11, stopping')
        	break
    source_process.send_signal(SIGINT)
    sink_process.send_signal(SIGINT)
    print('\tCompleted; accumulated data:')
    for l in data_lines:
        print(l.strip())


def main():
	# We only have one case to evaluate, Scenario 4	
	net = scenarios_mnw.scenario4()
	
	print('Connections:')
	print('{}'.format(get_nodes(net)))
	
	# wait for a bit to let stations associate with AP
	# especially needed for situations with high count of devices
	time.sleep(15)
	
	# Uncomment for CLI access
	#print('Starting CLI')
	#CLI(net)
	
	ping_test(net)
	raw_bandwidth_test(net)
	zenoh_bandwidth_test(net)

	print('Stopping mininet')
	net.stop()
		
	return 0


if __name__ == '__main__':
    sys.exit(main())
