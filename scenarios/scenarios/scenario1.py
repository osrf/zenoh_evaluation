#!/usr/bin/python3

from mininet.topo import Topo


def get_cpu_fraction(target_frequency):
    host_frequency = 3400.0
    return target_frequency / host_frequency


class Scenario1Topo(Topo):
    """
    Topology for scenario 1:
    - One switch
    - One host (the workstation) connected directly to the switch
    """
    def build(self):
        # Performance limits for hosts
        # Workstation is a 2 GHz, 4-core laptop

        # Devices
        switch = self.addSwitch('s1')
        workstation = self.addHost('h1', cpu=get_cpu_fraction(2000))

        # Performance parameters for links
        # Assume 1 Gbps connection between workstation and switch

        # Links
        self.addLink(switch, workstation, bw=1000)

