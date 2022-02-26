# FastDDS montblanc Test Scenario

## Description

> This test application used in the experiments for evaluating the primary metric is based on the [iRobot Mont Blanc performance benchmark topology](https://github.com/irobot-ros/ros2-performance/blob/master/irobot_benchmark/topology/mont_blanc.pdf).
> 
> **The topology is modified** to use standard ROS types and have on-receive based topic publications as well as timing based.
> 
> This application is implemented as one process per “node”. The nodes in the application print reception of data to standard output, and publish their outputs on the triggers indicated in the figure.

You'll find the image explaining the modified test topology below.

## Directory Overview

- `msg/`: Message IDL definitions and generated headers and code

- `src/`: App source

- `include/`: App headers

- `bin/`: Compiled executables

- `assets/`: Image assets for README

## Setup

This setup guide will help you install the pre-requisites and dependencies necessary for running this test application, including:

- eProsima FastDDS

- Java Runtime

### Steps

1. Install the dependencies for the dependencies

```shell
sudo apt install libasio-dev libtinyxml2-dev default-jre
```

2. Download the FastDDS installer

Download the installer from [here](https://www.eprosima.com/index.php/products-all/eprosima-fast-dds) and run the setup

```shell
cd eProsima_Fast-DDS-<VERSION>-<OS>
sudo ./install.sh 
```

## Message Generation

Because the mont_blanc test scenario contains a whole bunch of ROS2 message structs (including nested structs), the FastDDS generator tool [fastddsgen](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/usage/usage.html) was used to do the conversion from IDL.

Most of the IDL definitions used here were repurposed from [this repo](https://github.com/rticommunity/ros-data-types).

- `idl/`: Where the IDL file definitions are

- `gen/`: Folder for generated headers and code

### Generating Your Own Messages

If you want to generate your own message objects, run the following commands on your own IDL files (ensuring that they follow [the spec](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/dataTypes/dataTypes.html)).

This will generate all yet-to-be-generated type files and place them in the same directory structure that they appeared in the `idl/` directory!

```shell
cd <REPO_ROOT>/msg/idl

find . -name "*.idl" -exec sh -c '
  for file do
    mkdir -p ../gen/$(dirname ${file})
    fastddsgen ${file} -typeros2 -d ../gen/$(dirname ${file}) -I . -cs
  done' sh {} +
```

> If you want an example pubsub application to go with the generated files, simply append `-example CMake` to the `fastddsgen` invocation!
> 
> The example files will be suffixed (before the extension) with:
> 
> - Publisher
> 
> - Subscriber
> 
> - PubSubMain

> Alternatively, if you just want to do your generation one at a time, just invoke `fastddsgen` appropriately:
> 
> ```shell
> fastddsgen <FILE>.idl -typeros2 -cs -d <OUTPUT_DIR> -I <IDL_INCLUDE_DIR>
> ```
> 
> The `-cs` argument is especially important to prevent keyword conflicts.

## Test Topology

![](assets/test_topology.png)
