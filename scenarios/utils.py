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


def start_tshark_on(host, capture_file_path):
    intf = host.intfList()[0]

    tshark_process = host.popen(
        'tshark -i {} -w {}'.format(intf, capture_file_path))
    return tshark_process


def start_tshark_on_source(net, scenario_module, capture_file_path):
    source, sink = get_source_and_sink(net, scenario_module)
    return start_tshark_on(source, capture_file_path)


def start_tshark_on_sink(net, scenario_module, capture_file_path):
    source, sink = get_source_and_sink(net, scenario_module)
    return start_tshark_on(sink, capture_file_path)


def stop_tshark(tshark):
    tshark.send_signal(SIGINT)


def count_zenoh_messages(title, pcap_file, udp_ports=[], include=[], exclude=[]):
    udp_port_dissectors = []
    for p in udp_ports:
        udp_port_dissectors.append('-d')
        udp_port_dissectors.append('udp.port=={},zenoh-udp'.format(p))
    if type(include) == str:
        include = [include]
    if type(exclude) == str:
        include = [exclude]
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
         '-d', 'tcp.port==7501,zenoh-tcp',
         '-d', 'tcp.port==7502,zenoh-tcp',
         '-d', 'tcp.port==7503,zenoh-tcp',
         '-d', 'tcp.port==7504,zenoh-tcp',
         '-d', 'tcp.port==7505,zenoh-tcp',
         '-d', 'tcp.port==7506,zenoh-tcp',
         '-d', 'tcp.port==7507,zenoh-tcp',
         '-d', 'tcp.port==7508,zenoh-tcp',
         '-d', 'tcp.port==7509,zenoh-tcp',
         '-d', 'tcp.port==7510,zenoh-tcp',
         '-d', 'tcp.port==7511,zenoh-tcp',
         '-d', 'tcp.port==7512,zenoh-tcp',
         '-d', 'tcp.port==7513,zenoh-tcp',
         '-d', 'tcp.port==7514,zenoh-tcp',
         '-d', 'tcp.port==7515,zenoh-tcp',
         '-d', 'tcp.port==7516,zenoh-tcp',
         '-d', 'tcp.port==7517,zenoh-tcp',
         '-d', 'tcp.port==7518,zenoh-tcp',
         '-d', 'tcp.port==7519,zenoh-tcp',
         '-d', 'tcp.port==7520,zenoh-tcp',
         '-d', 'tcp.port==7521,zenoh-tcp',
         '-d', 'tcp.port==7522,zenoh-tcp',
         '-d', 'tcp.port==7447,zenoh-tcp',
         '-d', 'udp.port==7447,zenoh-udp'] +
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


def process_zenoh_packet_capture(capture_file_path):
    # Step 1: Find the randomly-chosen UDP ports
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
        len(udp_ports), udp_ports))
    # Step 2: Filter out known non-zenoh packets
    proc = subprocess.run(
        ['tshark',
         '-2',
         '-R', 'udp.port == 7447 || '
               'tcp.port == 7501 || tcp.port == 7502 || tcp.port == 7503 ||'
               'tcp.port == 7504 || tcp.port == 7505 || tcp.port == 7506 ||'
               'tcp.port == 7507 || tcp.port == 7508 || tcp.port == 7509 ||'
               'tcp.port == 7510 || tcp.port == 7511 || tcp.port == 7512 ||'
               'tcp.port == 7513 || tcp.port == 7514 || tcp.port == 7515 ||'
               'tcp.port == 7516 || tcp.port == 7517 || tcp.port == 7518 ||'
               'tcp.port == 7519 || tcp.port == 7520 || tcp.port == 7521 ||'
               'tcp.port == 7522 || tcp.port == 7523 || tcp.port == 7524 ||'
               'udp.port != 5001',
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
    # Step 3: Counts
    # Count SCOUT (0x01) messages
    count_zenoh_messages('SCOUT', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x01')
    # Count HELLO (0x02) messages
    count_zenoh_messages('HELLO', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x02')
    # Count INIT (0x03) messages
    count_zenoh_messages('INIT', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x03')
    # Count OPEN (0x04) messages
    count_zenoh_messages('OPEN', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x04')
    # Count KEEPALIVE (0x08) messages
    count_zenoh_messages('KEEPALIVE', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x08')
    # Count LINKSTATELIST (0x10) messages
    count_zenoh_messages('LINKSTATELIST', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x10')
    # Count DECLARE (0x0B) messages
    count_zenoh_messages('DECLARE', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x0b')
    # Count DATA (0x0C) messages
    count_zenoh_messages('DATA', '/tmp/filtered.pcap', udp_ports=udp_ports, include='0x0c')
    # Count other messages
    count_zenoh_messages('other (including TCP ACKs)', '/tmp/filtered.pcap', udp_ports=udp_ports, exclude=['0x01', '0x03', '0x04', '0x08', '0x10', '0x0b', '0x0c'])

    #os.remove(capture_file_path)
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
