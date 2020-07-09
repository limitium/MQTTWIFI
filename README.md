# MQTTWIFI - robust mqtt client over wifi for ESP8266 and ESP32
build on top of arduino mqtt client library [pubsubclient](http://pubsubclient.knolleary.net/)

[![Build Status](https://travis-ci.org/limitium/MQTTWIFI.svg?branch=master)](https://travis-ci.org/limitium/MQTTWIFI)
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](http://doge.mit-license.org)

## Features
- Simplest usage and api ever
- Aggressive wifi and mqtt connection sustenance

## Quick start

```c++
    MQTTWIFI::MQTTWIFI mqttwifi = MQTTWIFI::MQTTWIFI("wifiname", "password", "broker.mqttdashboard.com", 1883);
    void setup()
    {
        mqttwifi.init();
        mqttwifi.subscribe("topic1", [](const char *payload) {
            Serial.printf("Rcv from topic1, %s", payload);
        });
    }

    void loop()
    {
        mqttwifi.loop();
        mqttwifi.publish("topic1", "msg!");
}
```
## Exmaple
- [Basic](https://github.com/limitium/mqttwifi/tree/master/examples/basic) - basic=full usage example

## Dependencies
* (pubsubclient)[https://github.com/knolleary/pubsubclient]
* (log4esp)[https://github.com/hunsalz/log4Esp]

## Limitations
* all from underlying library (pubsubclient)[https://github.com/knolleary/pubsubclient#limitations]

## License
MQTTWIFI is open-source software licensed under the [MIT license](http://opensource.org/licenses/MIT)
