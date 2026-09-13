#pragma once

#include <Arduino.h>
#include <WiFi.h>


namespace AICSSWiFi {

    struct Config {
        const char* ssid;
        const char* password;
        uint8_t channel;
    };

    void conectar(Config& c) {
        WiFi.begin(c.ssid, c.password, c.channel);
        Serial.print("Conectando wo WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println("WiFi conectado!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }

}