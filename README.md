# TomCentauroUDP
UDP communication between TOM exoskeleton and CENTAURO robot

# Usage
Launch `roscore` in terminal 1
```sh
roscore
```
Launch Gazebo in terminal 2
```sh
source setup.bash
mon launch inail2arm_gazebo inail2_dual_arm_world.launch
```

Start `xbot2-core` in terminal 3 with apropriate config file
```sh
source setup.bash
xbot2-core -V -H sim -C ~/tom_ws/ros_src/iit-inail2arm-ros-pkg/inail2arm_config/inail2_dual_arm.yaml
```

Launch `xbot2-gui` from terminal 4 to switch plugins
```sh
source setup.bash
xbot2-gui
```

## Plugins
## homing
```sh
# autostops on homing completion
rosservice call /xbotcore/homing/switch 1
```
## udp server
start command 
```sh
# following service allso starts ik_plugin
rosservice call /xbotcore/udp_server/switch 1
```
stop command 
```sh
rosservice call /xbotcore/udp_server/switch 0
# stop ik_plugin
rosservice call /xbotcore/ik_plugin/switch 0
```

# Requirements

- [ros-noetic](http://wiki.ros.org/noetic/Installation/Ubuntu)
- [XBot2](https://advrhumanoids.github.io/xbot2/master/quickstart.html#system-setup)
- [forest](https://github.com/ADVRHumanoids/forest)

# Local Install
```
# for cartesio
apt-get update && apt-get install -y \
ros-noetic-moveit-ros-planning \ 
ros-noetic-moveit-ros-planning-interface

mkdir tom_ws && cd tom_ws
export HHCM_FOREST_CLONE_DEFAULT_PROTO=https
forest init

cd src
git clone https://github.com/siddharthdeore/tomcentauroudp.git --branch dual_arm_2023 tom_centauro_udp

mkdir ros_src && cd ros_src
git clone https://github.com/siddharthdeore/iit-inail2arm-ros-pkg.git --branch dual_arm_fix

# go to tom_ws directory
cd ..
forest add-recipes git@github.com:advrhumanoids/multidof_recipes.git --tag master 
forest grow tom_centauro_udp -j4
forest grow iit-dagana-ros-pkg -j4
```


# Important Notice

Regarding XBot2, please take note that the current version of `xbot2-core` is `2.11.0`, with the identifier `afcf13a` dated June 22, 2023, at 14:36:08 (GMT+2). The ik_plugin links to moveit libraries version `1.1.12`. However, standard Moveit installations use version `1.1.13`. As a temporary workaround, the necessary libraries are copied from the docker container to the `/opt/ros/noetic/lib` directory.
