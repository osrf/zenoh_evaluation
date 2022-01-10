#!/usr/bin/python3

from mininet.topo import Topo
from signal import SIGINT


source_name = 'r1'
sink_name = 'h1'


def get_cpu_fraction(target_frequency):
    host_frequency = 3400.0
    return target_frequency / host_frequency


class ScenarioTopo(Topo):
    """
    Topology for scenario 8:
    - One switch
    - One access point, connected to the switch (emulated with a switch)
    - One host (the FMS server) connected directly to the switch
    - 10 robots connected to the wifi

    """
    def build(self):
        # Performance limits for hosts
        # The robots have a 3 GHz, 4-core CPU
        # The FMS server has no limit on performance

        # Devices
        switch = self.addSwitch('s1')
        access_point = self.addSwitch('a1')
        fms_server = self.addHost('h1')
        robots = []
        for ii in range(0, 10):
            robots.append(self.addHost('r{}'.format(ii), cpu=get_cpu_fraction(3000)))

        # Performance parameters for links
        # 1 Gbps connection between the FMS server and the switch
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # Each robot has a 802.11n connection using the 5 GHz band, with 400
        #   Mbps of bandwidth available, and a 0.14% packet loss rate
        # All Wi-Fi devices have a 2ms latency

        # Links
        self.addLink(switch, fms_server, bw=1000)
        # Emulate the maximum throughput of the wireless by limiting the link
        # between the switch and the access point
        self.addLink(switch, access_point, bw=866.7)
        # Robot links
        for robot in robots:
            self.addLink(access_point, robot, bw=400, delay='2ms', loss=0.14)


def start_network_load(net):
    robots = []
    for ii in range(0, 10):
        robots.append(net.get('r{}'.format(ii)))
    fms_server = net.get('h1')

    traffic_sink = fms_server.popen('iperf -s -u')

    processes = [traffic_sink]
    for robot in robots:
        processes.append(robot.popen('iperf -c {} -u -b 50m'.format(fms_server.IP())))

    return processes


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)
