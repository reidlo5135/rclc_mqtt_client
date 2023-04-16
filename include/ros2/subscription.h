#include <stdio.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>

#define LOG_ROS "[ROS]" 

typedef struct {
  int some_int;
  char * some_text;
} sub_context_t;