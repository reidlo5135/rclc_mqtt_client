#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <stdio.h>

#define MQTT_HOSTNAME "localhost" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "root"  
#define MQTT_PASSWORD "1234"
#define MQTT_TOPIC "reidlo"