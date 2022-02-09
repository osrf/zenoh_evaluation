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
    Topology for scenario 9:
    - One switch
    - One access point, connected to the switch (emulated with a switch)
    - One host (the workstation) connected directly to the switch
    - One robot connected to the Wifi
    """
    def build(self):
        # Performance limits for hosts
        # The robot has a 3 GHz, 4-core CPU
        # The workstation is a 3 GHz, 4-core laptop

        # Devices
        switch = self.addSwitch('s1')
        access_point = self.addSwitch('a1')
        workstation = self.addHost('h1', cpu=get_cpu_fraction(3000))
        robot = self.addHost('r1', cpu=get_cpu_fraction(3000))

        # Performance parameters for links
        # 1 Gbps connection between the workstation and the switch
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # The robot has a 802.11n connection using the 5 GHz band, with 400
        #   Mbps bandwidth available, and a 0.14% packet loss rate
        # All Wi-Fi devices have a 2ms latency

        # Links
        self.addLink(switch, workstation, bw=1000)
        # Emulate the maximum throughput of the wireless by limiting the link
        # between the switch and the access point
        self.addLink(switch, access_point, bw=866.7)
        self.addLink(access_point, robot, bw=400, delay='2ms', loss=0.14)


def configure_network(net):
    pass


def start_network_load(net):
    return []


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)
