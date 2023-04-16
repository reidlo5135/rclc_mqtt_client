# ROS2-MQTT Client - For ROS2-MQTT connection (rclc)

## Document
  - [Environment](#environment)
  - [Installation](#installation)
    - [Prerequisites](#prerequisites)
    - [Install Paho MQTT C Library](#install-paho-mqtt-c-library)
    - [Colcon Build](#colcon-build)
    - [Run Test](#run-test)

## Environment
* <img src="https://img.shields.io/badge/c-A8B9CC?style=for-the-badge&logo=c&logoColor=white">
* <img src="https://img.shields.io/badge/cmake-064F8C?style=for-the-badge&logo=cmake&logoColor=white">
* <img src="https://img.shields.io/badge/mqtt-660066?style=for-the-badge&logo=mqtt&logoColor=white">
* <img src="https://img.shields.io/badge/ROS2-22314E?style=for-the-badge&logo=ros&logoColor=white">
* <img src="https://img.shields.io/badge/ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white">

## Installation

### Prerequisites
- [ROS2 setup](https://index.ros.org/doc/ros2/Installation/) for install rclnodejs by npm -
  **INSTALL [ROS2 Foxy-Fitzroy](https://docs.ros.org/en/foxy/Installation/Ubuntu-Install-Debians.html)**

### Install Paho MQTT C Library
```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
```

### Colcon Build
```bash
source /opt/ros/foxy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

### Run Test
```bash
source rclc_mqtt_client/install/setup.bash
ros2 run rclc_mqtt_client rclc_chatter
```