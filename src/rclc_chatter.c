#include "../include/ros2/rcl_subscription.h"

void chatter_callback(const void * msgin) {
  const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;
  if (msg == NULL) {
    printf("%s Callback: msg NULL\n", LOG_ROS);
  } else {
    printf("%s Callback: I heard: %s\n", LOG_ROS, msg->data.data);
  }

}

int main(int argc, const char * argv[]) {
  rcl_allocator_t allocator = rcl_get_default_allocator();
  rclc_support_t support;
  rcl_ret_t rc;

  const char * topic = "chatter";

  rcl_subscription_t subscription_topic;
  std_msgs__msg__String message;

  rc = rclc_support_init(&support, argc, argv, &allocator);
  if (rc != RCL_RET_OK) {
    printf("%s Error rclc_support_init.\n", LOG_ROS);
    return -1;
  }

  const rosidl_message_type_support_t * message_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

  rcl_node_t tester_chatter = rcl_get_zero_initialized_node();
  rc = rclc_node_init_default(&tester_chatter, "tester_chatter", "rclc_mqtt_client", &support);
  if(rc != RCL_RET_OK) {
    printf("%s Error in rclc_node_init_default \n", LOG_ROS);
    return -1;
  }

  while(RCL_RET_OK) {
    subscription_topic = rcl_get_zero_initialized_subscription();
    rc = rclc_subscription_init_default(&topic, &tester_chatter, message_type_support, topic);

    if (rc != RCL_RET_OK) {
      printf("%s Failed to create subscriber %s.\n", LOG_ROS, topic);
      return -1;
    } else {
      printf("%s Created subscriber %s:\n", LOG_ROS, topic);
    }

    std_msgs__msg__String__init(&message);
  }

  rclc_executor_t executor;
  executor = rclc_executor_get_zero_initialized_executor();
  
  unsigned int num_handles = 3;
  printf("%s Debug : number of DDS handles :%u\n", LOG_ROS, num_handles);
  rclc_executor_init(&executor, &support.context, num_handles, &allocator);

  rc = rclc_executor_add_subscription(&executor, &subscription_topic, &message, &chatter_callback, ON_NEW_DATA);
  if (rc != RCL_RET_OK) {
      printf("%s Error in rclc_executor_add_subscription. \n", LOG_ROS);
      return -1;
  }

  rc = rclc_executor_spin_some(&executor, 1000 * (1000 * 1000));

  // rc = rclc_executor_fini(&executor);

  return 0;
}