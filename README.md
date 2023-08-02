# TomCentauroUDP
UDP communication between TOM exoskeleton and CENTAURO robot

- Terminal 1
```
# launch gazebo
mon launch inail2arm_gazebo inail2_dual_arm_world.launch 
```

- Terminal 2
```
# set_xbot2_config ~/tom_ws/ros_src/iit-inail2arm-ros-pkg/inail2arm_config/inail2_dual_arm.yaml
xbot2-core -H sim
```

- Homing
```
# homing start
rosservice call /xbotcore/homing/switch 1
```

- UDP Server plugin
```
# start udp server 
rosservice call /xbotcore/udp_server/switch 1
# start udp server 
rosservice call /xbotcore/udp_server/switch 2
```
