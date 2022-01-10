#!/usr/bin/env bash

(trap 'kill 0' SIGINT; \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/cordoba 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/lyon 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/freeport 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/medellin 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/portsmouth 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/hamburg 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/delhi 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/taipei 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/osaka 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/hebron 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/kingston 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/tripoli 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/mandalay 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/ponce 2>&1)
