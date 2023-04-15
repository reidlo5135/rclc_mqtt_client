#include "../include/mqtt.h"

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("%s subscription message with token value %d delivery confirmed \n", LOG_TOPIC, dt);
    deliveredtoken = dt;
};

int message_arrived(void *context, char *topic_name, int topic_length, MQTTClient_message *message) {
    printf("%s message arrived \n", LOG_TOPIC);
    printf("%s topic : %s \n", topic_name);
    printf("%s message : %.*s \n", message -> payloadlen, (char*)message -> payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic_name);
    return 1;
};

void connection_lost(void *context, char *cause) {
    printf("%s connection lost \n", LOG_TOPIC);
    printf("%s cause : %s \n", LOG_TOPIC, cause);
};

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions connection_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to create cleint, return code %d \n", LOG_TOPIC, rc);
        rc = EXIT_FAILURE;
        goto exit;
    };

    if((rc = MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, delivered)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to set callbacks, return code %d \n", LOG_TOPIC, rc);
        rc = EXIT_FAILURE;
        goto exit;
    };

    connection_opts.keepAliveInterval = 20;
    connection_opts.cleansession = 1;
    if((rc = MQTTClient_connect(client, &connection_opts)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to connect, return code %d \n", LOG_TOPIC, rc);
        rc = EXIT_FAILURE;
        goto exit;
    };

    printf("%s subscribing to topic %s \n for client %s using QoS %d \n\n Press Q<Enter> to quit \n\n", TOPIC, CLIENTID, QOS);
    if((rc == MQTTClient_subscribe(client, TOPIC, QOS)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to subscribe, return code %d \n", LOG_TOPIC, rc);
        rc = EXIT_FAILURE;
    } else {
        int ch;
        do {
            ch = getchar();
        } while(ch != 'Q' && ch != 'q');
        if((rc = MQTTClient_unsubscribe(client, TOPIC)) != MQTTCLIENT_SUCCESS) {
                printf("%s failed to unsubscribe, return code %d \n", LOG_TOPIC, rc);
                rc = EXIT_FAILURE;
        };
    };

    if((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) {
        printf("%s faield to disconnect, return code %d \n", LOG_TOPIC, rc);
        rc = EXIT_FAILURE;
    }

    destroy_exit:
        MQTTClient_destroy(&client);
    exit:
        return rc;
};