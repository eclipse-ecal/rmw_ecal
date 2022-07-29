# eCAL ROS2 middleware layer
![Build](https://github.com/eclipse-ecal/rmw_ecal/actions/workflows/ros_ci.yml/badge.svg)

ROS2 middleware based on eCAL.

**NOTE:** Minimun eCAL compatible with RMW is 5.10.

eCAL RMW can offer:

* High performance (shared memory is used for inter-process communication and udp multicast for inter-host communication)
* Support for [eCAL](https://github.com/eclipse-ecal/ecal) ecosystem powerful tools like eCAL recorder, eCAL player, eCAL monitor, which can be used to record, replay and monitor RMW layer
* Communication between ROS2 nodes and eCAL tasks without need for a gateway

## Build instructions

* Install [eCAL](https://eclipse-ecal.github.io/ecal/getting_started/setup.html)
* Clone latest release of this repository into your [ROS2 workspace](https://index.ros.org/doc/ros2/Tutorials/Workspace/Creating-A-Workspace/)
* Source ROS2
```bash
# On Linux
source /path/to/your/ros/distro/folder/setup.bash
# On Windows
call C:/path/to/your/ros/distro/folder/setup.bat
```
* Run `colcon build` from your workspace folder
* Setup your workspace
```bash
# On Linux
source /path/to/your/workspace/install/setup.bash
# On Windows
call C:/path/to/your/workspace/install/setup.bat
```

## How to use
There are currently two rmw implementations
* rmw_ecal_dynamic_cpp
* rmw_ecal_proto_cpp

### rmw_ecal_dynamic_cpp
rmw_ecal_dynamic_cpp uses custom dynamic typesupport which is builtin into rmw.  
To use this implementation just set variable RMW_IMPLEMENTATION to "rmw_ecal_dynamic_cpp".  

Run all nodes using rmw_ecal_dynamic_cpp
```bash
# On Linux
export RMW_IMPLEMENTATION=rmw_ecal_dynamic_cpp
# On Windows
set RMW_IMPLEMENTATION=rmw_ecal_dynamic_cpp
```
Run specific node (in this example demo_nodes_cpp talker) using 
```bash
RMW_IMPLEMENTATION=rmw_ecal_dynamic_cpp ros2 run demo_nodes_cpp talker
```

Pros:
* Faster than rmw_ecal_proto_cpp 
* Plug&Play doesn't require any additional steps/ros packages to work
  
Cons:
* Doesn't integrate well into eCAL ecosystem (monitor will only show binary data for messages and native eCAL nodes won't be able to deserialize its data)
 
### rmw_ecal_proto_cpp
rmw_ecal_proto_cpp uses protobuf based static typesupport.  
To use this implementation setup [rosidl_typesupport_protobuf](https://github.com/eclipse-ecal/rosidl_typesupport_protobuf) and set variable RMW_IMPLEMENTATION to "rmw_ecal_proto_cpp".  

Run all nodes using rmw_ecal_proto_cpp
```bash
# On Linux
export RMW_IMPLEMENTATION=rmw_ecal_proto_cpp
# On Windows
set RMW_IMPLEMENTATION=rmw_ecal_proto_cpp
```
Run specific node (in this example demo_nodes_cpp talker) using 
```bash
RMW_IMPLEMENTATION=rmw_ecal_proto_cpp ros2 run demo_nodes_cpp talker
```
Pros:
* Integrates well into eCAL ecosystem (monitor will show actual message data and native eCAL nodes will be able to deserialize its messages)

Cons:
* A bit slower than rmw_ecal_dynamic_cpp
* Not plug&play, requires [rosidl_typesupport_protobuf](https://github.com/eclipse-ecal/rosidl_typesupport_protobuf) to be built and sourced to work

## Zero copy support
[eCAL 5.10 introduced zero copy support for publishers](https://eclipse-ecal.github.io/ecal/advanced/layers/shm.html#zero-copy-mode-optional), it's currently disabled by default since it's still in experimental stage.
You can enable it by adding this config to ecal.ini

```ini
[publisher]
memfile_zero_copy = 1
```

## Currently supported ROS2 distributions

* Foxy Fitzroy
* Galactic Geochelone
* Humble Hawksbill

## Legacy ROS2 distributions

* [Dashing Diademata](https://github.com/eclipse-ecal/rmw_ecal/tree/legacy/dashing)
* [Eloquent Elusor](https://github.com/eclipse-ecal/rmw_ecal/tree/legacy/eloquent)
