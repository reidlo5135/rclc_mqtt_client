#include "ros_connection_bridge/ros_connection_bridge.h"

int ros_chatter_publisher_ini(rcl_ret_t rc, rcl_node_t * ros_connection_bridge_node_ptr) {
  if(rc != RCL_RET_OK) {
    printf("%s Error in ros_connection_bridge_node with ros_chatter_publisher \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  }

  rcl_publisher_t ros_chatter_publisher;
  ros_chatter_publisher = rcl_get_zero_initialized_publisher();
  const char * topic = "/chatter";
  const rosidl_message_type_support_t * message_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);
  rcl_publisher_options_t ros_publisher_options = rcl_publisher_get_default_options();

  rc = rcl_publisher_init(
    &ros_chatter_publisher,
    ros_connection_bridge_node_ptr,
    message_type_support,
    topic,
    &ros_publisher_options
  );

  if(rc != RCL_RET_OK) {
    printf("%s Error in creating ros_chatter_publisher \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s ros chatter publisher is ready \n", LOG_ROS_CONNECTION_BRIDGE);
  }

  std_msgs__msg__String ros_chatter_publish_message;
  std_msgs__msg__String__init(&ros_chatter_publish_message);
  const unsigned int PUB_MSG_CAPACITY = 20;
  ros_chatter_publish_message.data.data = malloc(PUB_MSG_CAPACITY);
  ros_chatter_publish_message.data.capacity = PUB_MSG_CAPACITY;
  snprintf(ros_chatter_publish_message.data.data, ros_chatter_publish_message.data.capacity, "Hello World!");
  ros_chatter_publish_message.data.size = strlen(ros_chatter_publish_message.data.data);

  rc = rcl_publish(&ros_chatter_publisher, &ros_chatter_publish_message, NULL);
  if (rc == RCL_RET_OK) {
    printf("%s Published message %s\n", LOG_ROS_CONNECTION_BRIDGE, ros_chatter_publish_message.data.data);
  } else {
    printf("%s Error in ros_chatter_publisher publishing \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  }

  rc += rcl_publisher_fini(&ros_chatter_publisher, &ros_connection_bridge_node_ptr);
  std_msgs__msg__String__fini(&ros_chatter_publish_message);

  return rc;
}

int main(int argc, const char * argv[]) {
  rcl_allocator_t rcl_allocator = rcl_get_default_allocator();
  rclc_support_t rcl_support;
  rcl_ret_t rc;

  rc = rclc_support_init(&rcl_support, argc, argv, &rcl_allocator);
  if (rc != RCL_RET_OK) {
    printf("%s Error rclc_support_init.\n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s rclc_support_init made \n", LOG_ROS_CONNECTION_BRIDGE);
  }

  rcl_node_t ros_connection_bridge_node = rcl_get_zero_initialized_node();
  rc = rclc_node_init_default(&ros_connection_bridge_node, "ros_connection_bridge", "", &rcl_support);
  
  if(rc != RCL_RET_OK) {
    printf("%s Error in rclc_node_init_default \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s rclc_node_init made \n", LOG_ROS_CONNECTION_BRIDGE);
  }

  rc = ros_chatter_publisher_ini(rc, &ros_connection_bridge_node);

  if(rc != RCL_RET_OK) {
    printf("%s Error in init ros chatter publisher \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s Ros chatter publishing in order", LOG_ROS_CONNECTION_BRIDGE);
  }

  rclc_executor_t ros_executor;
  ros_executor = rclc_executor_get_zero_initialized_executor();
  
  unsigned int num_handles = 1 + 1;
  printf("%s Debug : number of DDS handles :%u \n", LOG_ROS_CONNECTION_BRIDGE, num_handles);

  rclc_executor_init(&ros_executor, &rcl_support.context, num_handles, &rcl_allocator);

  unsigned int rcl_wait_timeout = 1000;
  rc = rclc_executor_set_timeout(&ros_executor, RCL_MS_TO_NS(rcl_wait_timeout));
  if (rc != RCL_RET_OK) {
    printf("%s Error while set timeout of ros executor \n", LOG_ROS_CONNECTION_BRIDGE);
  }

  while(rc == RCL_RET_OK) {
    rc = rclc_executor_spin(&ros_executor);
  }

  rc = rclc_executor_fini(&ros_executor);
  rc += rcl_node_fini(&ros_connection_bridge_node);
  rc += rclc_support_fini(&rcl_support);

  if (rc != RCL_RET_OK) {
    printf("%s Error while cleaning up!\n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  }
  
  return 0;
}