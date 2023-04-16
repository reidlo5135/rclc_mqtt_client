cmake_minimum_required(VERSION 3.5)
project(rclc_mqtt_client)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
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
find_package(rclc_lifecycle REQUIRED)
find_package(std_msgs REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclc_parameter REQUIRED)

include_directories(
  include
  include/${PROJECT_NAME}
)

add_executable(rclc_subscription src/rclc_subscription.c)
ament_target_dependencies(rclc_subscription rcl rclc std_msgs)

add_executable(rclc_chatter src/rclc_chatter.c)
ament_target_dependencies(rclc_chatter rcl rclc std_msgs)

install(TARGETS
  rclc_subscription
  rclc_chatter
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()