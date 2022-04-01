#!/usr/bin/python3

from mininet.node import Controller
from mininet.log import setLogLevel, info

from mn_wifi.wmediumdConnector import interference, snr
from mn_wifi.node import Station
from mn_wifi.net import Mininet_wifi
from mn_wifi.link import wmediumd, WirelessLink

from signal import SIGINT
import time
import random

import uuid

uid = str(uuid.uuid4())[:3]

import matplotlib
matplotlib.rc('figure', figsize=(10, 10))

# Common args for mininet wifi
station_args = dict()
ap_args = dict(ssid='zenohdragon', passwd='123456789a', encrypt='wpa2',
               mode='ac', channel=36, client_isolation=False,
               range='150',
               datapath='user', ieee80211w='2', failMode='standalone')

# Setup mininet wifi
net = Mininet_wifi(controller=Controller, ipBase='192.168.1.0/24', link=WirelessLink)

def scenario4():
    """
    Topology for scenario 4:
    - One access point, all devices connected via wifi
    - One host (the target workstation) and 10 other workstations
    - 20 cellphones
    - 1 robot
    """
    # Devices
    workstations = []
    robots = []
    phones = []

    # Primary wifi AP
    a0 = net.addAccessPoint(f'ap0-{uid}', **ap_args, position='0,0,1')

    for i in range(1, 10+1):
        name=f'w{i:02}-{uid}'
        pos = f'{random.randrange(-10, 10)}, {random.randrange(-5, -3)}, 1'

        workstations.append(net.addStation(f'{name}', position=pos, **station_args))
        print(f'[{name} ADDED] POS: {pos}')

    for i in range(1, 1+1):
        name=f'r{i:02}-{uid}'
        pos = f'{random.randrange(-20, 20)}, {random.randrange(10, 25)}, 0'

        robots.append(net.addStation(f'{name}', position=pos, **station_args))
        print(f'[{name} ADDED] POS: {pos}')

    for i in range(1, 20+1):
        name=f'p{i:02}-{uid}'
        pos = f'{random.randrange(-30, 30)}, {random.randrange(-30, 30)}, 2'

        phones.append(net.addStation(f'{name}', position=pos, **station_args))
        print(f'[{name} ADDED] POS: {pos}')

    # Controller for AP(s)
    c0 = net.addController(f'c0-{uid}', controller=Controller)

    # Performance parameters for links : Logarithmic propogation model
    net.setPropagationModel(model="logDistance", exp=4.5)

    # Configure wireless
    net.configureWifiNodes()

    # Add Links
    for w in workstations:
        net.addLink(w, a0)
    for r in robots:
        net.addLink(r, a0)
    for p in phones:
        net.addLink(p, a0)

    # net.plotGraph(max_x=50, max_y=50, min_x=-50, min_y=-50)

    # Build and start network
    net.build()

    c0.start()
    a0.start([c0])

    return net
