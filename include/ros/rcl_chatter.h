#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>

#include "mqtt/mqtt.h"
#include "MQTTClient.h"

#define LOG_ROS "[ROS]" 

typedef struct {
  int some_int;
  char * some_text;
} sub_context_t;