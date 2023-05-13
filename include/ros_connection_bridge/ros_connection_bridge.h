#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>

#define LOG_ROS_CONNECTION_BRIDGE "[ROS-CONNECTION-BRIDGE]"

typedef struct {
  rcl_publisher_t ros_publisher;
  rcl_publisher_options_t ros_publisher_options;
  const rosidl_message_type_support_t * message_type_support;
  const char * topic;
} ros_publisher_t;

int ros_chatter_publisher_ini(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr);