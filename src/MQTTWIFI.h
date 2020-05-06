#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Log4Esp.h>
#include <cstring>
#include <map>

namespace MQTTWIFI
{
#define SUB_CALLBACK_SIGNATURE std::function<void(const char *)>

unsigned long MQTT_RECONNECT_TIMEOUT = 5000;

struct cmp_str
{
    bool operator()(char const *a, char const *b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

class MQTTWIFI
{
public:
    MQTTWIFI() {}
    MQTTWIFI(const char *SSID, const char *password, const char *domain, uint16_t port)
    {
        WiFiClient client;
        MQTT_CALLBACK_SIGNATURE = std::bind(&MQTTWIFI::subscribeHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        this->mqtt = PubSubClient(domain, port, callback, client);
        this->SSID = SSID;
        this->password = password;
    }
    void init()
    {
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
        WiFi.begin(this->SSID, this->password);
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);

        log.verbose(F("Connecting with WiFi %s"), this->SSID);
        while (!WiFi.isConnected())
        {
            delay(500);
        }
        log.verbose(F("Connected"));
    }
    void loop()
    {
        unsigned long now = millis();
        if (!mqtt.connected())
        {
            if (now - lastReconnectMQTTAttempt > MQTT_RECONNECT_TIMEOUT)
            {
                lastReconnectMQTTAttempt = now;
                if (reconnectMQTT())
                {
                    lastReconnectMQTTAttempt = 0;
                }
            }
        }
        else
        {
            mqtt.loop();
        }
    }
    void publish(const char *topic, const char *payload)
    {
        if (mqtt.connected())
        {
            log.verbose(F(">> %s:%s"), topic, payload);
            mqtt.publish(topic, payload);
        }
    }
    void subscribe(const char *topic, SUB_CALLBACK_SIGNATURE callback)
    {
        log.verbose(F("sub:%s"), topic);
        callbacks[topic] = callback;
        if (mqtt.connected())
        {
            mqtt.subscribe(topic);
        }
    }
    void unsubscribe(const char *topic)
    {
        log.verbose(F("unsub:%s"), topic);
        callbacks.erase(topic);
        if (mqtt.connected())
        {
            mqtt.unsubscribe(topic);
        }
    }

private:
    const char *SSID;
    const char *password;

    PubSubClient mqtt;
    unsigned long lastReconnectMQTTAttempt;
    std::map<const char *, SUB_CALLBACK_SIGNATURE, cmp_str> callbacks;

    Logger log = Logger("MQTTWIFI");

    boolean reconnectMQTT()
    {
        if (mqtt.connect("MQTTWIFI"))
        {
            log.verbose(F("Reconnected"));
            for (const auto &pair : callbacks)
            {
                mqtt.subscribe(pair.first);
            }
        }
        return mqtt.connected();
    }
    void subscribeHandler(const char *topic, uint8_t *payload, unsigned int length)
    {
        payload[length] = '\0';
        SUB_CALLBACK_SIGNATURE callback = callbacks[topic];
        log.verbose(F("<< %s:%s"), topic, payload);
        if (callback != nullptr)
        {
            return callback((const char *)payload);
        }
    }
};
} // namespace MQTTWIFI