#!/usr/bin/env bash

(trap 'kill 0' SIGINT; \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/geneva 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/monaco 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/rotterdam 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/barcelona 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/arequipa 2>&1 & \
    /home/mininet/zenoh_evaluation/mont_blanc/zenoh/target/debug/georgetown 2>&1)
