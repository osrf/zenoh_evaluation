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


# Common args for mininet wifi
station_args = dict()
ap_args = dict(ssid='zenohdragon', passwd='123456789a', encrypt='wpa2',
		mode='ac', channel=36, client_isolation=False, 
		datapath='user', ieee80211w='2', failMode='standalone')

# Setup mininet wifi
net = Mininet_wifi(ipBase='192.168.16.0/24', link=WirelessLink)

# Controller for AP(s)
controller = net.addController('controller', controller=Controller)
	
# Primary wifi AP
access_point = net.addAccessPoint('ap0', **ap_args, position='0,0,1')


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
	for ii in range(1, 11+1):
            workstations.append(net.addStation('w{}'.format(ii), **station_args,
            	position='{},{},1'.format(random.randrange(-10,10), random.randrange(-5,-4))))
	
	robots = []
	for ii in range(1, 1+1):
            robots.append(net.addStation('r{}'.format(ii), **station_args,
            	position='{},{},0'.format(random.randrange(-20,20), random.randrange(10,25))))
            	
	phones = []
	for ii in range(1, 21+1):
            robots.append(net.addStation('p{}'.format(ii), **station_args,
            	position='{},{},2'.format(random.randrange(-30,30), random.randrange(-30,30))))

	# Performance parameters for links : Logarithmic propogation model
	net.setPropagationModel(model="logDistance", exp=4.5)
	
	# Configure wireless
	net.configureWifiNodes()
	
	# Add Links
	for w in workstations:
		net.addLink(access_point, w)
	for r in robots:
		net.addLink(access_point, r)
	for p in robots:
		net.addLink(access_point, p)
	
	net.plotGraph(max_x=50, max_y=50, min_x=-50, min_y=-50)
		
	# Build and start network
	net.build()
	controller.start()
	access_point.start([controller])
	
	return net
