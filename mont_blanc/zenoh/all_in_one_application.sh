#!/usr/bin/env bash

(trap 'kill 0' SIGINT; \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/cordoba & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/lyon & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/freeport & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/medellin & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/portsmouth & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/hamburg & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/delhi & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/taipei & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/osaka & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/hebron & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/kingston & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/tripoli & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/mandalay & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/ponce & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/geneva & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/monaco & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/rotterdam & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/barcelona & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/arequipa & \
    /home/${USER}/src/ros/zenoh_evaluation/mont_blanc/zenoh/target/debug/georgetown)

