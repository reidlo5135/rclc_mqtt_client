#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>

#define LOG_ROS_CONNECTION_BRIDGE "[ROS-CONNECTION-BRIDGE]"

typedef struct {
  int some_int;
  char * some_text;
} sub_context_t;