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
    Topology for scenario 5:
    - One 10 Gbps switch
    - One cellular modem, connected to the switch by a 4G connection
    - One access point, connected to the cellular modem by a USB 3.1 connection
    - One host (the Internet server) connected directly to the switch by a
      high-speed (internal datacenter) link
    - One robot, connected to the wifi
    """
    def build(self):
        # Performance limits for hosts
        # The robot has a 3 GHz, 4-core CPU
        # The Internet server has effectively unlimited performance

        # Devices
        switch = self.addSwitch('s1')
        cellular_modem = self.addSwitch('c1')
        access_point = self.addSwitch('a1')
        internet_server = self.addHost('h1')
        robot = self.addHost('r1', cpu=get_cpu_fraction(3000))

        # Performance parameters for links
        # The cellular connection is 4G, with a latency of between 35 ms and
        #   150 ms.
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # The connection between cellular and wifi is USB 3.1, with 5 Gbps of
        # bandwidth
        # The robot has a 802.11n connection using the 5 GHz band, with 270
        #   Mbps of bandwidth available, a 0.14% packet loss rate, and a
        #   latency 

        # Links
        self.addLink(switch, internet_server, bw=10000)
        self.addLink(switch, cellular_modem, bw=100)
        self.addLink(cellular_modem, access_point, bw=50000, delay='100ms', loss=0.5)
        self.addLink(access_point, robot, bw=270, delay='2ms', loss=0.14)


def configure_network(net):
    pass


def start_network_load(net):
    return []


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)
