cmake_minimum_required(VERSION 3.5)
project(rclc_mqtt_client)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_ros REQUIRED)
find_package(rcl REQUIRED)
find_package(rcl_lifecycle REQUIRED)
find_package(rclc REQUIRED)
find_package(std_msgs REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_library(PAHO_MQTT_C_LIB paho-mqtt3c PATHS /usr/local/lib)

include_directories(
  include
  include/${PROJECT_NAME}
  /usr/local/include
)

add_executable(ros_connection_bridge src/ros_connection_bridge/ros_connection_bridge.c)
target_link_libraries(ros_connection_bridge ${PAHO_MQTT_C_LIB})
ament_target_dependencies(ros_connection_bridge rcl rclc std_msgs)

install(TARGETS
  ros_connection_bridge
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()