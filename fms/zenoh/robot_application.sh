#!/usr/bin/env bash

(trap 'kill 0' SIGINT; \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/cordoba $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/lyon $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/freeport $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/medellin $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/portsmouth $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/hamburg $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/delhi $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/taipei $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/osaka $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/hebron $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/kingston $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/tripoli $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/mandalay $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/ponce $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/geneva $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/monaco $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/rotterdam $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/barcelona $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/arequipa $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/georgetown $1 & \
    /home/mininet/zenoh_evaluation/fms/zenoh/target/debug/status_reporter $1)

