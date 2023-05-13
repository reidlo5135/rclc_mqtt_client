#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>

#define LOG_ROS_CONNECTION_BRIDGE "[ROS-CONNECTION-BRIDGE]"

rcl_ret_t ros_chatter_publisher_ini(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr, rcl_publisher_t * ros_chatter_publisher_ptr);
rcl_ret_t ros_chatter_publisher_fini(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr, rcl_publisher_t * ros_chatter_publisher_ptr);
rcl_ret_t ros_chatter_subscription_ini(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr, rcl_subscription_t * ros_chatter_subscription_ptr);
void ros_chatter_subscription_callback(const void * message_in);
rcl_ret_t bridge_chatter_to_mqtt(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr);