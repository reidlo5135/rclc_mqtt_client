#include "mqtt/mqtt.h"
#include "ros/rcl_chatter.h"

// callback function for rcl subscription
void chatter_callback(const void * msgin) {
  // received messages from rcl subscription
  const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;

  // error tracing when received message is NULL
  if (msg == NULL) {
    printf("%s Callback: msg NULL\n", LOG_ROS);
  } else {
    // std_msgs__msg__String type instance for extract & initialize data from received rcl message
    const std_msgs__msg__String * received = msg->data.data;
    printf("%s Callback: I heard: %s\n", LOG_ROS, received);

    // invoke mqtt_publish function for mqtt publish with received message
    mqtt_publish("/chatter", received);
  }
}

// main function
int main(int argc, const char * argv[]) {
  // declare & initialize rcl_allocator
  rcl_allocator_t allocator = rcl_get_default_allocator();

  // declare rclc_support
  rclc_support_t support;

  // declare rclc_ret_t
  rcl_ret_t rc;

  // initialize default rcl topic
  const char * topic = "chatter";

  // declare rcl_subscription
  rcl_subscription_t rcl_subscription;

  // declare std_msgs__msg__String type instance
  std_msgs__msg__String message;

  // create rclc init options
  rc = rclc_support_init(&support, argc, argv, &allocator);
  if (rc != RCL_RET_OK) {
    printf("%s Error rclc_support_init.\n", LOG_ROS);
    return -1;
  } else {
    printf("%s rclc_support_init made \n", LOG_ROS);
  }

  // create rcl node
  rcl_node_t tester_chatter_node = rcl_get_zero_initialized_node();
  rc = rclc_node_init_default(&tester_chatter_node, "tester_chatter", "", &support);
  if(rc != RCL_RET_OK) {
    printf("%s Error in rclc_node_init_default \n", LOG_ROS);
    return -1;
  } else {
    printf("%s rclc_node_init made \n", LOG_ROS);
  }

  // initialize rosidl_message_type_support
  const rosidl_message_type_support_t * message_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

  // create rcl subscription
  rcl_subscription = rcl_get_zero_initialized_subscription();

  // initialize rclc subscription with rcl_subscription, rcl_node, message_type_support, topic
  rc = rclc_subscription_init_default(&rcl_subscription, &tester_chatter_node, message_type_support, topic);

  // error tracing when create rcl subscription
  if (rc != RCL_RET_OK) {
    printf("%s Failed to create subscription %s.\n", LOG_ROS, topic);
    return -1;
  } else {
    printf("%s Created subscriber %s \n", LOG_ROS, topic);
  }

  // initalize std_msgs__msg__String
  std_msgs__msg__String__init(&message);

  // declare rclc_executor
  rclc_executor_t executor;
  executor = rclc_executor_get_zero_initialized_executor();
  
  // define num_handles
  unsigned int num_handles = 3;
  printf("%s Debug : number of DDS handles :%u \n", LOG_ROS, num_handles);

  // initialize rclc_executor with executor, support.context, num_handles, allocator
  rclc_executor_init(&executor, &support.context, num_handles, &allocator);

  // register rclc subscription into rclc executor
  rc = rclc_executor_add_subscription(&executor, &rcl_subscription, &message, &chatter_callback, ON_NEW_DATA);

  // error tracing when register rclc subscription
  if (rc != RCL_RET_OK) {
      printf("%s Error in rclc_executor_add_subscription. \n", LOG_ROS);
      return -1;
  }

  // spin rclc executor when RCL_RET_OK
  while(rc == RCL_RET_OK) {
    rc = rclc_executor_spin(&executor);
  }

  // clean up memory

  // destroy rclc executor in memory
  rc = rclc_executor_fini(&executor);

  // destroy rclc subscription in memory
  rc += rcl_subscription_fini(&rcl_subscription, &tester_chatter_node);

  // destroy rclc node in memory
  rc += rcl_node_fini(&tester_chatter_node);

  // destroy rclc support in memory
  rc += rclc_support_fini(&support);

  // destroy std_msgs__msg__String in memory
  std_msgs__msg__String__fini(&message);

  // error tracing when clean up
  if (rc != RCL_RET_OK) {
    printf("%s Error while cleaning up!\n", LOG_ROS);
    return -1;
  }
  
  return 0;
}