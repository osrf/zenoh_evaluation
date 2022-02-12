from signal import SIGINT
import os
import re
import subprocess


def get_source_and_sink(net, module):
    return net.get(module.source_name, module.sink_name)


def start_tshark_native(capture_file_path, interface):
    return subprocess.Popen(
        ['tshark',
         '-i',
         interface,
         '-w',
         capture_file_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
        )


def start_tshark_on(host, interface, capture_file_path):
    tshark_process = host.popen(
        'tshark -i {} -w {}'.format(interface, capture_file_path))
    return tshark_process


def start_tshark_on_source(net, scenario_module, capture_file_path):
    source, sink = get_source_and_sink(net, scenario_module)
    interface = scenario_module.get_capture_interface(
        net,
        scenario_module.source_name)
    return start_tshark_on(source, interface, capture_file_path)


def start_tshark_on_sink(net, scenario_module, capture_file_path):
    source, sink = get_source_and_sink(net, scenario_module)
    interface = scenario_module.get_capture_interface(
        net,
        scenario_module.sink_name)
    return start_tshark_on(sink, interface, capture_file_path)


def stop_tshark(tshark):
    tshark.send_signal(SIGINT)


def count_zenoh_messages(
        title,
        pcap_file,
        tcp_ports=[],
        udp_ports=[],
        include=[],
        exclude=[]):
    tcp_port_dissectors = []
    for p in tcp_ports:
        tcp_port_dissectors.append('-d')
        tcp_port_dissectors.append('tcp.port=={},zenoh-tcp'.format(p))
    udp_port_dissectors = []
    for p in udp_ports:
        udp_port_dissectors.append('-d')
        udp_port_dissectors.append('udp.port=={},zenoh-udp'.format(p))
    if type(include) == str:
        include = [include]
    if type(exclude) == str:
        exclude = [exclude]
    filter_pattern = ''
    for msgid in include:
        filter_pattern = filter_pattern + 'zenoh.msgid == {} && '.format(msgid)
    for msgid in exclude:
        filter_pattern = filter_pattern + 'zenoh.msgid != {} && '.format(msgid)
    filter_pattern = filter_pattern[:-4]
    if len(filter_pattern) > 0:
        filters = ['-2', '-R', filter_pattern]
    else:
        filters = []
    proc = subprocess.run(
        ['tshark',
         '-d', 'tcp.port==7500,zenoh-tcp',
         '-d', 'udp.port==7447,zenoh-udp'] +
        tcp_port_dissectors +
        udp_port_dissectors +
        filters +
        ['-r', pcap_file,
         '-w', '/tmp/zenoh_filtered.pcap',
         '-q',
         '-z', 'io,stat,0'],
        stdout=subprocess.PIPE,
        #stderr=subprocess.STDOUT,
        text=True
        )
    print('Count of {} messages:'.format(title))
    print(proc.stdout)
    os.remove('/tmp/zenoh_filtered.pcap')


def process_zenoh_packet_capture(capture_file_path, robot_count):
    # Calculate the TCP port numbers that will be used
    tcp_ports = []
    for robot_number in range(0, robot_count):
        for port_number in range(
                7501 + robot_number * 50,
                7522 + robot_number * 50):
            tcp_ports.append(port_number)
    tcp_port_matchers = ''
    for p in tcp_ports:
        tcp_port_matchers += ' || tcp.port == {}'.format(p)
    # Find the randomly-chosen UDP ports
    proc = subprocess.run(
        ['tshark',
         '-2',
         '-R', 'udp.dstport == 7447',
         '-r', capture_file_path],
        stdout=subprocess.PIPE,
        text=True
        )
    udp_ports = []
    for l in proc.stdout.split('\n'):
        if not l:
            continue
        m = re.match(r'\d+\s\d+(.\d+)?\s+(\d+\.\d+\.\d+\.\d+)\s.+\s(\d+\.\d+\.\d+\.\d+)\s+ZENOH\s\d+\s(?P<port>\d{4,5})\s.+\s7447', l.strip())
        if m:
            port_number = int(m.group('port'))
            if port_number not in udp_ports:
                udp_ports.append(port_number)
    print()
    print('Found {} randomly-assigned UDP ports:\n\t{}'.format(
        len(udp_ports),
        udp_ports))
    udp_port_matchers = ''
    for p in udp_ports:
        udp_port_matchers += ' || udp.port == {}'.format(p)
    # Filter out known non-zenoh packets
    proc = subprocess.run(
        ['tshark',
         '-2',
         '-R', 'udp.port == 7447 || tcp.port == 7500'
               + tcp_port_matchers
               + udp_port_matchers,
         '-r', capture_file_path,
         '-w', '/tmp/filtered.pcap',
         '-q',
         '-z', 'io,stat,0'],
        stdout=subprocess.PIPE,
        #stderr=subprocess.STDOUT,
        text=True
        )
    print()
    print('Results of initial filter')
    print(proc.stdout)
    print()
    # Count SCOUT (0x01) messages
    count_zenoh_messages(
        'SCOUT',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x01')
    # Count HELLO (0x02) messages
    count_zenoh_messages(
        'HELLO',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x02')
    # Count INIT (0x03) messages
    count_zenoh_messages(
        'INIT',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x03')
    # Count OPEN (0x04) messages
    count_zenoh_messages(
        'OPEN',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x04')
    # Count KEEPALIVE (0x08) messages
    count_zenoh_messages(
        'KEEPALIVE',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x08')
    # Count LINKSTATELIST (0x10) messages
    count_zenoh_messages(
        'LINKSTATELIST',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x10')
    # Count DECLARE (0x0B) messages
    count_zenoh_messages(
        'DECLARE',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x0b')
    # Count DATA (0x0C) messages
    count_zenoh_messages(
        'DATA',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        include='0x0c')
    # Count other messages
    count_zenoh_messages(
        'other (including TCP ACKs)',
        '/tmp/filtered.pcap',
        tcp_ports=tcp_ports,
        udp_ports=udp_ports,
        exclude=['0x01', '0x03', '0x04', '0x08', '0x10', '0x0b', '0x0c'])

    os.remove(capture_file_path)
    os.remove('/tmp/filtered.pcap')


def print_interfaces(net, scenario_module):
    source, sink = get_source_and_sink(net, scenario_module)

    source_interface_info = source.cmd('ip a')
    print('Source interfaces')
    print(source_interface_info)

    sink_interface_info = sink.cmd('ip a')
    print('Sink interfaces')
    print(sink_interface_info)


def print_iptables_rules(net, scenario_module):
    source, sink = get_source_and_sink(net, scenario_module)

    source_rules = source.cmd('sudo iptables -L -nv')
    print('Source iptables rules')
    print(source_rules)

    sink_rules = sink.cmd('sudo iptables -L -nv')
    print('Sink iptables rules')
    print(sink_rules)
