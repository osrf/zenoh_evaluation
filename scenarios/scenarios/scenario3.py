#!/usr/bin/python3

from mininet.topo import Topo
from signal import SIGINT


source_name = 'w1'
sink_name = 'h1'


def get_cpu_fraction(target_frequency):
    host_frequency = 3400.0
    return target_frequency / host_frequency


class ScenarioTopo(Topo):
    """
    Topology for scenario 3:
    - One switch
    - One access point, connected to the switch (emulated with a switch)
    - Three stations on the wifi (the target robot, another robot, a third
      robot)
    - Three hosts (the target workstation and two other workstations) connected
      directly to the switch

    """
    def build(self):
        # Performance limits for hosts
        # Each robot has a Raspberry Pi 4 at 1.5 GHz and 4 cores
        # Each workstation is a 2 GHz, 4-core laptop

        # Devices
        switch = self.addSwitch('s1')
        access_point = self.addSwitch('a1')
        target_workstation = self.addHost('h1', cpu=get_cpu_fraction(2000))
        other_workstation_1 = self.addHost('h2', cpu=get_cpu_fraction(2000))
        other_workstation_2 = self.addHost('h3', cpu=get_cpu_fraction(2000))
        target_robot = self.addHost('w1', cpu=get_cpu_fraction(1500))
        other_robot_1 = self.addHost('w2', cpu=get_cpu_fraction(1500))
        other_robot_2 = self.addHost('w3', cpu=get_cpu_fraction(1500))

        # Performance parameters for links
        # Assume 1 Gbps connection between each workstation and the switch
        # Wi-Fi is Wi-Fi 5 (802.11ac)
        # Each robot has a 802.11n connection using the 5 GHz band, with 60
        #   Mbps of bandwidth available, and a 0.14% packet loss rate
        #   (empirically measured)
        # All Wi-Fi devices have a 2ms latency

        # Links
        self.addLink(switch, target_workstation, bw=1000)
        self.addLink(switch, other_workstation_1, bw=1000)
        self.addLink(switch, other_workstation_2, bw=1000)
        # Emulate the maximum throughput of the wireless by limiting the link
        # between the switch and the access point
        self.addLink(switch, access_point, bw=866.7)
        self.addLink(access_point, target_robot, bw=60, delay='2ms', loss=0.14)
        self.addLink(access_point, other_robot_1, bw=60, delay='2ms', loss=0.14)
        self.addLink(access_point, other_robot_2, bw=60, delay='2ms', loss=0.14)


def configure_network(net):
    source, sink = net.get(source_name, sink_name)

    source.cmd('sudo iptables -I OUTPUT -o {} -p udp -j DROP'.format(source.intfList()[0]))
    sink.cmd('sudo iptables -I OUTPUT -o {} -p udp -j DROP'.format(sink.intfList()[0]))


def start_network_load(net):
    workstation_2, workstation_3, robot_2, robot_3 = net.get('h2', 'h3', 'w2', 'w3')

    workstation_2_traffic_sink = workstation_2.popen('iperf -s -u')
    workstation_3_traffic_sink = workstation_3.popen('iperf -s -u')

    robot_2_traffic_source = robot_2.popen('iperf -c {} -u -b 60m'.format(workstation_2.IP()))
    robot_3_traffic_source = robot_3.popen('iperf -c {} -u -b 60m'.format(workstation_3.IP()))

    return workstation_2_traffic_sink, workstation_3_traffic_sink, robot_2_traffic_source, robot_3_traffic_source


def stop_network_load(processes):
    for p in processes:
        p.send_signal(SIGINT)
