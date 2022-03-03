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

Also remember to make sure that your installed libraries are findable! You can append this line to `~/.bashrc`:

```shell
export LD_LIBRARY_PATH=/usr/local/lib/
```

## A Note on the Spec

You'll notice if you dig into the utility `Node` class that was written, that only **one** publisher and/or subscriber is initialized and associated with any number of datareaders and datawriters.

This is **as intended** by the DDS spec, which you can find on [page 8 of the spec (page 20 on the PDF)](https://www.omg.org/spec/DDS/1.4/PDF).

## Message Generation

Because the mont_blanc test scenario contains a whole bunch of ROS2 message structs (including nested structs), the FastDDS generator tool [fastddsgen](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/usage/usage.html) was used to do the conversion from IDL.

Most of the IDL definitions used here were repurposed from [this repo](https://github.com/rticommunity/ros-data-types).

- `idl/`: Where the IDL file definitions are

- `gen/`: Folder for generated headers and code

### Generating Your Own Messages

> **BIG WARNING**: The `fastddsgen` tool works for basic IDL definitions, but is quite buggy once you start needing to do more complicated things like:
> 
> - Having IDL includes
> 
> - Having custom IDL types used in containers (e.g. sequences of a custom type)
> 
> These issues have been sidestepped for this test with certain procedures that will be documented below.

If you want to generate your own message objects, run the following commands on your own IDL files (ensuring that they follow [the spec](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/dataTypes/dataTypes.html)).

This will generate all yet-to-be-generated type files and place them in the same directory structure that they appeared in the `idl/` directory!

```shell
cd <REPO_ROOT>/mont_blanc/dds/msg/idl

find . -name "*.idl" -exec sh -c '
  for file do
    printf "\n\n[GENERATING: ${file}]\n"
    mkdir -p ../gen/$(dirname ${file})
    fastddsgen ${file} -typeros2 -d ../gen/$(dirname ${file}) -I . -cs
  done' sh {} +
```

>  If you want to split your headers and sources, remember to move the generated headers to the appropriate location in `include/` and **update the generated sources' include statements**.
> 
> For the generated sources, this means appending `"type/"` to the include path.
> 
> In other words:
> 
> ```cpp
> #include "types/datatypesPubSubTypes.h"
>           ^^^^^
> 
> #include "types/datatypes.h"
>           ^^^^^
> ```

### The Caveats

**IDL includes**

The IDL spec allows for other IDL definitions to be included in another definition. `fastddsgen` invokes a pre-processor to replace the include line with the contents of the included file. This works as intended, but for some reason very often, `fastddsgen` deterministically fails to generate a valid source file.

This is a [yet unsolved issue](https://github.com/eProsima/Fast-DDS-Gen/issues/97), so the way around this is to list all IDL definitions in a **single file**, doing the job of the pre-processor.

**Custom IDL Types in Sequence**

The fastddsgen IDL spec doesn't explicitly support this case, but the fix that allows this to work is fairly simple-- the generated sources need to be manually patched.

This is because there is a missing `const operator ==` operator overload. This issue [appears to have been fixed in the newest release of fastddsgen](https://github.com/eProsima/Fast-DDS-Gen/issues/98), but since it isn't what is installed by default in the latest version of FastDDS, you either need to upgrade manually, or.. manually patch it in the generated header and source file!

For example:

```cpp
// TypeName.h

    eProsima_user_DllExport bool operator ==(
            const PointField& x);

    // Patched in
    eProsima_user_DllExport bool operator ==(
            const PointField& x) const;
```

```cpp
// TypeName.cxx

bool PointField::operator ==(
        const PointField& x)
{

    return (m_name == x.m_name && m_offset == x.m_offset && m_datatype == x.m_datatype && m_count == x.m_count);
}

// Patched in
bool PointField::operator ==(
        const PointField& x) const
{

    return (m_name == x.m_name && m_offset == x.m_offset && m_datatype == x.m_datatype && m_count == x.m_count);
}
```

### Generating Examples

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

## More Gotchas

### Field Max Sizes

Each field actually has a max size (of buffer allocated in memory!) (e.g. for strings, the max size is `255`). If you exceed this amount, FastDDS **FAILS SILENTLY!!!** and the sent message can't get received by subscribers (whether this is due to the message not being published or some issue on the subscriber side remains to be seen).

In order to see the max size, see the `getMaxCdrSerializedSize` method in your generated source files (for the non-PubSubType source file).

For example:

```cpp
size_t String::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;
                                                                                       ^^^
    return current_alignment - initial_alignment;
}
```

Or for a sequence type

```cpp
size_t TwistWithCovariance::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += Twist::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    current_alignment += (100 * 8) + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);
                          ^^^



    return current_alignment - initial_alignment;
}
```

Note that each increment corresponds to a field. 

#### Increasing  Field Max Sizes

There are two ways to increase the max field sizes:

1. Manually patch the generated sources
   
   - This is self explanatory, just increase the numbers highlighted in the previous section for the relevant field for the relevant type you want to increase the max field size for

2. Manually specify the max size in your IDL definition
   
   - The IDL spec allows you to configure the max size for a field. You can specify the max size explicitly using (or appending to) a `< >` tag. So for example, if we wanted to increase the max size to 1024...
   
   - ```cpp
     // String example
     struct String {
         string<1024> data;
                ^^^^
     };
     ```
   
   - ```cpp
     // Sequence example
     struct TwistWithCovariance {
         Twist twist;
         sequence<double, 1024> covariance;
                          ^^^^
     };
     ```

## 

## Test Topology

![](assets/test_topology.png)
