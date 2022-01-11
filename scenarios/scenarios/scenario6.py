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
    Topology for scenario 6:
    - One switch
    - One access point, connected to the switch (emulated with a switch)
    - One host (the target workstation) connected directly to the switch
    - 10 other devices connected directly to the switch
    - One robot connected to the Wifi
    - 10 other devices connected to the wifi

    """
    def build(self):
        # Performance limits for hosts
        # The robot has a 3 GHz, 4-core CPU
        # The target workstation is a 3 GHz, 4-core laptop
        # The other devices have no limit on performance

        # Devices
        switch = self.addSwitch('s1')
        access_point = self.addSwitch('a1')
        target_workstation = self.addHost('h1', cpu=get_cpu_fraction(3000))
        target_robot = self.addHost('r1', cpu=get_cpu_fraction(3000))
        other_lan_devices = self.addHost('l1')
        other_wifi_devices = self.addHost('w1')

        # Performance parameters for links
        # 1 Gbps connection between the workstation and the switch
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # The robot has a 802.11n connection using the 5 GHz band, with 270
        #   Mbps of bandwidth available, and a 0.14% packet loss rate
        # The other devices on the LAN have a total of 10 Gbps bandwidth
        # The other devices on the Wifi have a total of 8667 Mbps bandwidth
        # All Wi-Fi devices have a 2ms latency

        # Links
        self.addLink(switch, target_workstation, bw=1000)
        # Emulate the maximum throughput of the wireless by limiting the link
        # between the switch and the access point
        self.addLink(switch, access_point, bw=866.7)
        # Robot link
        self.addLink(access_point, target_robot, bw=270, delay='2ms', loss=0.14)
        # Other devices
        self.addLink(switch, other_lan_devices, bw=10000)
        self.addLink(access_point, other_wifi_devices, bw=8667, delay='2ms', loss=0.14)


def start_network_load(net):
    other_lan_devices, other_wifi_devices = net.get('l1', 'w1')

    traffic_sink = other_lan_devices.popen('iperf -s -u')

    traffic_source = other_wifi_devices.popen(
        'iperf -c {} -u -b 10000m'.format(other_lan_devices.IP()))

    return traffic_sink, traffic_source


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)
