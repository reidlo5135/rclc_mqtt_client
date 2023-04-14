#include "include/mqtt.h"

int main(int argc, char **argv) {
    struct mosquitto *mosq = NULL;

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if(!mosq) {
        perror("Cant Initialize mosquitto lib \n");
        exit(-1);
    };

    mosquitto_username_pw_set(mosq, MQTT_USERNAME, MQTT_PASSWORD);

    int ret = mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
    if (ret) {
        perror("Cannot connect to mosquitto server \n");
        exit(-1);
    };

    char text[20] = "Hello";

    ret = mosquitto_publish(mosq, NULL, MQTT_TOPIC, strlen(text), text, 0, false);
    if(ret) {
        perror("Cannot connect to mosquitto server \n");
        exit(-1);
    };

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
};