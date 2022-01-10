#!/usr/bin/env bash

(trap 'kill 0' SIGINT; \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/cordoba & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/lyon & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/freeport & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/medellin & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/portsmouth & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/hamburg & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/delhi & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/taipei & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/osaka & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/hebron & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/kingston & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/tripoli & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/mandalay & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/ponce & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/geneva & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/monaco & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/rotterdam & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/barcelona & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/arequipa & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/georgetown)

