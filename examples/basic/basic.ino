#include <MQTTWIFI.h>

MQTTWIFI::MQTTWIFI mqttwifi = MQTTWIFI("wifiname", "password", "broker.mqttdashboard.com", 1883);

void setup()
{
    Serial.begin(9600);
    Serial.setDebugOutput(false);
    while (!Serial && !Serial.available())
    {
    };
    delay(300);
    Serial.println();

    mqttwifi.init();

    mqttwifi.subscribe("topic1", [](const char *payload) {
        Serial.printf("Rcv from topic1, %s", payload);
    });
}

void loop()
{
    mqttwifi.loop();

    mqttwifi.publish("topic1", "msg!");

    delay(1000);
}