# eCAL ROS2 middleware layer

ROS2 middleware based on eCAL.

eCAL RMW can offer:

* High performance (shared memory is used for inter-process communication and udp multicast for inter-host communication)
* Support for eCAL ecosystem powerful tools like eCAL recorder, eCAL player, eCAL monitor, which can be used to record, replay and monitor RMW layer
* Communication between ROS2 nodes and eCAL tasks without need for a gateway

## Build instructions

### Linux

* Install [eCAL](https://github.com/continental/ecal)
* Clone latest release of this repository into your [ROS2 workspace](https://index.ros.org/doc/ros2/Tutorials/Workspace/Creating-A-Workspace/)
* Source ROS2 `source /path/to/your/ros/distro/folder/setup.bash`
* Run `colcon build` from your workspace folder
* Setup your workspace `source /path/to/your/workspace/install/setup.bash`
* Specify ecal as rmw implementation `export RMW_IMPLEMENTATION=rmw_ecal_dynamic_cpp`

### Windows

* Install [eCAL](https://github.com/continental/ecal)
* Clone latest release of this repository into your [ROS2 workspace](https://index.ros.org/doc/ros2/Tutorials/Workspace/Creating-A-Workspace/)
* Source ROS2 `call C:/path/to/your/ros/distro/folder/setup.bat`
* Run `colcon build` from your workspace folder
* Setup your workspace `call C:/path/to/your/workspace/install/setup.bat`
* Specify ecal as rmw implementation `set RMW_IMPLEMENTATION=rmw_ecal_dynamic_cpp`


### Multiple serialization options

Two serialization methods are currently supported:

* Custom
* Protobuf

Custom serialization offers fast performance, but its data format is not compatible with eCAL Monitor tool (it will show topic names/types, but data won't be visualized), Recorder and Player should work fine.

Protobuf serialization offers good integration with eCAL Monitor and allows ROS2 Nodes and eCAL tasks to communicate without any kind of gateway, but has reduced performance and isn't compatible with rosbag.

By default custom serialization is enabled, to use protobuf USE_PROTOBUF_SERIALIZATION option should be turned on.

```
colcon build --cmake-args -DUSE_PROTOBUF_SERIALIZATION=ON
```

RMW can currently work with only one serialization method at the time.

### Currently supported ROS2 distributions

* Dashing Diademata
* Eloquent Elusor
