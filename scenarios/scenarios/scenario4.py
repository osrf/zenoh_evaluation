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
    Topology for scenario 4:
    - One switch
    - One access point, connected to the switch (emulated with a switch)
    - 31 stations on the wifi (the target robot, 10 workstations, and 20 cell
      phones)
    - One host (the target workstation) connected directly to the switch

    """
    def build(self):
        # Performance limits for hosts
        # The robot has a 3 GHz, 4-core CPU
        # The target workstation is a 3 GHz, 4-core laptop
        # The other workstations and cellphones have no limit on performance

        # Devices
        switch = self.addSwitch('s1')
        access_point = self.addSwitch('a1')
        target_workstation = self.addHost('h1', cpu=get_cpu_fraction(3000))
        target_robot = self.addHost('r1', cpu=get_cpu_fraction(3000))
        workstations = []
        for ii in range(0, 10):
            workstations.append(self.addHost('w{}'.format(ii)))
        cellphones = []
        for ii in range(0, 20):
            cellphones.append(self.addHost('c{}'.format(ii)))

        # Performance parameters for links
        # Assume 1 Gbps connection between the workstation and the switch
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # The robot has a 802.11n connection using the 5 GHz band, with 270
        #   Mbps of bandwidth available, and a 0.14% packet loss rate
        # The workstation devices on wifi each have a 866.7 Mbps connection
        # The cellphone devices on wifi each have a 433.3 Mbps connection
        # All Wi-Fi devices have a 2ms latency

        # Links
        self.addLink(switch, target_workstation, bw=1000)
        # Emulate the maximum throughput of the wireless by limiting the link
        # between the switch and the access point
        self.addLink(switch, access_point, bw=866.7)
        self.addLink(access_point, target_robot, bw=270, delay='2ms', loss=0.14)
        # Workstations on the wireless
        for w in workstations:
            self.addLink(access_point, w, bw=866.7, delay='2ms', loss=0.14)
        # Cellphones on the wireless
        for c in cellphones:
            self.addLink(access_point, c, bw=433.3, delay='2ms', loss=0.14)


def configure_network(net):
    source, sink = net.get(source_name, sink_name)

    source.cmd('sudo iptables -I OUTPUT -o {} -p udp -j DROP'.format(source.intfList()[0]))
    sink.cmd('sudo iptables -I OUTPUT -o {} -p udp -j DROP'.format(sink.intfList()[0]))


def start_network_load(net):
    return []


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)


def get_capture_interface(net, host_name):
    return 'any'
