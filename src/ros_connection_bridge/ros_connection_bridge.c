#include "ros_connection_bridge/ros_connection_bridge.h"

void chatter_callback(const void * msgin) {
  const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;

  if (msg == NULL) {
    printf("%s Callback: msg NULL\n", LOG_ROS_CONNECTION_BRIDGE);
  } else {
    const std_msgs__msg__String * received = msg->data.data;
    printf("%s Callback: I heard: %s\n", LOG_ROS_CONNECTION_BRIDGE, received);
  }
}

int main(int argc, const char * argv[]) {
  rcl_allocator_t allocator = rcl_get_default_allocator();
  rclc_support_t support;
  rcl_ret_t rc;

  const char * topic = "chatter";

  rcl_publisher_t rcl_publisher;
  rcl_subscription_t rcl_subscription;
  std_msgs__msg__String message;

  rc = rclc_support_init(&support, argc, argv, &allocator);
  if (rc != RCL_RET_OK) {
    printf("%s Error rclc_support_init.\n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s rclc_support_init made \n", LOG_ROS_CONNECTION_BRIDGE);
  }

  rcl_node_t tester_chatter_node = rcl_get_zero_initialized_node();
  rc = rclc_node_init_default(&tester_chatter_node, "tester_chatter", "", &support);
  
  if(rc != RCL_RET_OK) {
    printf("%s Error in rclc_node_init_default \n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  } else {
    printf("%s rclc_node_init made \n", LOG_ROS_CONNECTION_BRIDGE);
  }
  const rosidl_message_type_support_t * message_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

  rcl_subscription = rcl_get_zero_initialized_subscription();

  rc = rclc_subscription_init_default(&rcl_subscription, &tester_chatter_node, message_type_support, topic);

  if (rc != RCL_RET_OK) {
    printf("%s Failed to create subscription %s.\n", LOG_ROS_CONNECTION_BRIDGE, topic);
    return -1;
  } else {
    printf("%s Created subscriber %s \n", LOG_ROS_CONNECTION_BRIDGE, topic);
  }

  std_msgs__msg__String__init(&message);

  rclc_executor_t executor;
  executor = rclc_executor_get_zero_initialized_executor();
  
  unsigned int num_handles = 3;
  printf("%s Debug : number of DDS handles :%u \n", LOG_ROS_CONNECTION_BRIDGE, num_handles);

  rclc_executor_init(&executor, &support.context, num_handles, &allocator);
  rc = rclc_executor_add_subscription(&executor, &rcl_subscription, &message, &chatter_callback, ON_NEW_DATA);

  if (rc != RCL_RET_OK) {
      printf("%s Error in rclc_executor_add_subscription. \n", LOG_ROS_CONNECTION_BRIDGE);
      return -1;
  }

  while(rc == RCL_RET_OK) {
    rc = rclc_executor_spin(&executor);
  }

  rc = rclc_executor_fini(&executor);
  rc += rcl_subscription_fini(&rcl_subscription, &tester_chatter_node);
  rc += rcl_node_fini(&tester_chatter_node);
  rc += rclc_support_fini(&support);

  std_msgs__msg__String__fini(&message);

  if (rc != RCL_RET_OK) {
    printf("%s Error while cleaning up!\n", LOG_ROS_CONNECTION_BRIDGE);
    return -1;
  }
  
  return 0;
}