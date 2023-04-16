#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

#define LOG_MQTT    "[MQTT]"
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "1h1h"
#define TOPIC       "reidlo"
#define PAYLOAD     "test!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;
int mqtt_publish(char * topic, char * payload);