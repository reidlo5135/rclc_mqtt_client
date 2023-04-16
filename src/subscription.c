#include "../include/ros2/subscription.h"

void my_subscriber_callback_with_context(const void * msgin, void * context_void_ptr) {
  const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;
  if (msg == NULL) {
    printf("%s Callback: msg NULL\n", LOG_ROS);
  } else {
    printf("%s Callback: I heard: %s\n", LOG_ROS, msg->data.data);
  }

  if (context_void_ptr == NULL) {
    printf("%s Callback: context is empty\n", LOG_ROS);
  } else {
    sub_context_t * context_ptr = (sub_context_t *) context_void_ptr;
    printf("%s Callback: context contains: %s\n", LOG_ROS, context_ptr->some_text);
    printf("%s Callback: context also contains: %d\n", LOG_ROS, context_ptr->some_int);
    context_ptr->some_int++;
  }
}

int main(int argc, const char * argv[]) {
  rcl_allocator_t allocator = rcl_get_default_allocator();
  rclc_support_t support;
  rcl_ret_t rc;

  const unsigned int n_topics = 3;
  const char * topic_names[] = {"topic_foo", "topic_bar", "topic_baz"};
  sub_context_t my_contexts[] = {
    {0, "foo counting from zero"},
    {100, "bar counting from 100"},
    {300, "baz counting from 300"},
  };
  rcl_publisher_t my_pubs[n_topics];
  std_msgs__msg__String pub_msgs[n_topics];
  rcl_subscription_t my_subs[n_topics];
  std_msgs__msg__String sub_msgs[n_topics];

  rc = rclc_support_init(&support, argc, argv, &allocator);
  if (rc != RCL_RET_OK) {
    printf("%s Error rclc_support_init.\n", LOG_ROS);
    return -1;
  }

  rcl_node_t my_node = rcl_get_zero_initialized_node();
  rc = rclc_node_init_default(&my_node, "node_0", "executor_examples", &support);
  if (rc != RCL_RET_OK) {
    printf("%s Error in rclc_node_init_default\n", LOG_ROS);
    return -1;
  }

  const rosidl_message_type_support_t * my_type_support =
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

  for (unsigned int i = 0; i < n_topics; i++) {
    rc = rclc_publisher_init_default(
      &(my_pubs[i]),
      &my_node,
      my_type_support,
      topic_names[i]);
    if (RCL_RET_OK != rc) {
      printf("%s Error in rclc_publisher_init_default %s.\n", LOG_ROS, topic_names[i]);
      return -1;
    }
    std_msgs__msg__String__init(&( pub_msgs[i] ) );
    const unsigned int PUB_MSG_CAPACITY = 40;
    pub_msgs[i].data.data = malloc(PUB_MSG_CAPACITY);
    pub_msgs[i].data.capacity = PUB_MSG_CAPACITY;
    snprintf(
      pub_msgs[i].data.data, pub_msgs[i].data.capacity, "Hello World! on %s",
      topic_names[i]
    );
    pub_msgs[i].data.size = strlen(pub_msgs[i].data.data);

    my_subs[i] = rcl_get_zero_initialized_subscription();
    rc = rclc_subscription_init_default(
      &my_subs[i],
      &my_node,
      my_type_support,
      topic_names[i]);
    if (rc != RCL_RET_OK) {
      printf("%s Failed to create subscriber %s.\n", LOG_ROS, topic_names[i]);
      return -1;
    } else {
      printf("%s Created subscriber %s:\n", LOG_ROS, topic_names[i]);
    }
    std_msgs__msg__String__init(&( sub_msgs[i] ) );
  }

  rclc_executor_t executor;
  executor = rclc_executor_get_zero_initialized_executor();

  unsigned int num_handles = n_topics + 0;
  printf("%s Debug: number of DDS handles: %u\n", LOG_ROS, num_handles);
  rclc_executor_init(&executor, &support.context, num_handles, &allocator);

  for (unsigned int i = 0; i < n_topics; i++) {
    sub_context_t * context_ptr = &( my_contexts[i] );
    void * context_void_ptr = (void *) context_ptr;

    rc = rclc_executor_add_subscription_with_context(
      &executor,
      &(my_subs[i]), &(sub_msgs[i]),
      &my_subscriber_callback_with_context,
      context_void_ptr,
      ON_NEW_DATA);
    if (rc != RCL_RET_OK) {
      printf("%s Error in rclc_executor_add_subscription. \n", LOG_ROS);
    }
  }

  for (unsigned int tick = 0; tick < 10; tick++) {
    rc = rclc_executor_spin_some(&executor, 1000 * (1000 * 1000));

    for (unsigned int i = 0; i < n_topics; i++) {
      rc = rcl_publish(&my_pubs[i], &pub_msgs[i], NULL);
      if (rc == RCL_RET_OK) {
        printf("%s \nPublished message %s\n", LOG_ROS, pub_msgs[i].data.data);
      } else {
        printf("%s Error publishing message %s\n", LOG_ROS, pub_msgs[i].data.data);
      }
      rc = rclc_executor_spin_some(&executor, 1000 * (1000 * 1000));
    }
  }

  rc = rclc_executor_fini(&executor);

  for (unsigned int i = 0; i < n_topics; i++) {
    rc += rcl_publisher_fini(&(my_pubs[i]), &my_node);
    rc += rcl_subscription_fini(&(my_subs[i]), &my_node);
  }
  rc += rcl_node_fini(&my_node);
  rc += rclc_support_fini(&support);

  for (unsigned int i = 0; i < n_topics; i++) {
    std_msgs__msg__String__fini(&(pub_msgs[i]));
    std_msgs__msg__String__fini(&(sub_msgs[i]));
  }

  if (rc != RCL_RET_OK) {
    printf("%s Error while cleaning up!\n", LOG_ROS);
    return -1;
  }
  return 0;
}
