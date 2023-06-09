#include "mqtt/mqtt.h"

void mqtt_publish(char * topic, char * payload) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
 
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
         printf("%s Failed to create client, return code %d\n", LOG_MQTT, rc);
         exit(EXIT_FAILURE);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("%s Failed to connect, return code %d\n", LOG_MQTT, rc);
        exit(EXIT_FAILURE);
    }
 
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("%s Failed to publish message, return code %d\n", LOG_MQTT, rc);
        exit(EXIT_FAILURE);
    } else {
        printf("%s published to [%s] with [%s] --length [%d]\n", LOG_MQTT, topic, pubmsg.payload, pubmsg.payloadlen);
    }
 
    printf("%s Waiting for up to %d seconds for publication of %s on topic %s for client with ClientID: %s\n", LOG_MQTT, (int)(TIMEOUT/1000), payload, topic, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("%s Message with delivery token %d delivered\n", LOG_MQTT, token);
 
    return rc;
}

void delivered(MQTTClient_deliveryToken dt) {
    printf("%s subscription message with token value %d delivery confirmed \n", LOG_MQTT, dt);
    deliveredtoken = dt;
}

int message_arrived(char * topic, int topic_length, MQTTClient_message * message) {
    printf("%s message arrived \n", LOG_MQTT);
    printf("%s topic : %s \n", LOG_MQTT, topic);
    printf("%s message : %.*s \n", LOG_MQTT, message -> payloadlen, (char*)message -> payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic);
    return 1;
}

void connection_lost(char * cause) {
    printf("%s connection lost \n", LOG_MQTT);
    printf("%s cause : %s \n", LOG_MQTT, cause);
}

void mqtt_subscribe(char * topic) {
    MQTTClient client;
    MQTTClient_connectOptions connection_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to create client, return code %d \n", LOG_MQTT, rc);
        rc = EXIT_FAILURE;
        goto exit;
    } else {
        printf("%s client created... %d \n", LOG_MQTT, rc);
    };

    if((rc = MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, delivered)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to set callbacks, return code %d \n", LOG_MQTT, rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    } else {
        printf("%s set callbacks successed... %d \n", LOG_MQTT, rc);
    };

    connection_opts.keepAliveInterval = 20;
    connection_opts.cleansession = 1;
    if((rc = MQTTClient_connect(client, &connection_opts)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to connect, return code %d \n", LOG_MQTT, rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    } else {
        printf("%s connection established... %d \n", LOG_MQTT, rc);
    };

    printf("%s subscribing to topic %s for client id %s using QoS %d \n\n", LOG_MQTT, topic, CLIENTID, QOS);
    printf("%s Press Q<Enter> to quit \n\n", LOG_MQTT);
    if((rc = MQTTClient_subscribe(client, topic, QOS)) != MQTTCLIENT_SUCCESS) {
        printf("%s failed to subscribe, return code %d \n", LOG_MQTT, rc);
        rc = EXIT_FAILURE;
    } else {
        int ch;
        do {
            ch = getchar();
        } while(ch != 'Q' && ch != 'q');
        if((rc = MQTTClient_unsubscribe(client, topic)) != MQTTCLIENT_SUCCESS) {
                printf("%s failed to unsubscribe, return code %d \n", LOG_MQTT, rc);
                rc = EXIT_FAILURE;
        };
    };

    if((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) {
        printf("%s faield to disconnect, return code %d \n", LOG_MQTT, rc);
        rc = EXIT_FAILURE;
    };

    destroy_exit:
        MQTTClient_destroy(&client);
    exit:
        return rc;
}