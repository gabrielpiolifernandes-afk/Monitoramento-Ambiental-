#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

Adafruit_BMP085 sensorPressaoAtm;

#include "AICSSBMP10.hpp"
#include "AICSSDHT.hpp"
#include "AICSSPotentiometerCAPACITIVO.hpp"
#include "AICSSPotentiometerANEMOMETRO.hpp"
#include "AICSSLDR.hpp"
#include "AICSSLEDS.hpp" 
#include "AICSSWiFi.hpp"

// WiFi configs
AICSSWiFi::Config wifiConfig = {
    wifiConfig.ssid = "Wokwi-GUEST",
    wifiConfig.password = "",
    wifiConfig.channel = 6};

// Servidor (endpoint) python
const char* serverHost = "host.wokwi.internal";
const uint16_t serverPort = 5000;

// BMP (configurações)
#define SDAPIN 18
#define SCLPIN 19
AICSSBMP10::BMP sensorBMP;

// DHT (configurações)
#define DHTPIN 17
#define DHTMODEL DHT22
AICSSDHT::Sensor sensorDHT;

// Potenciômetro (configurações)
#define POT_PIN 34
AICSSPotentiometerCAPACITIVO::Potentiometer sensorPot;

// ANEMÔMETRO (configurações)
#define ANEM_PIN 35
AICSSPotentiometerANEMOMETRO::Potentiometer sensorAnem;
    
// LDR
#define LDR_PIN 21
AICSSLDR::Sensor sensorLDR;

// RED LED
#define LED_PIN 5
AICSSLED::LED ledRED;

// GREEN LED
#define LED_GREEN_PIN 4
AICSSLED::LED ledGREEN;

// Controle temporal
const int INTERVALO = 5000; // 5 segundos para enviar medições
uint64_t ultimoEnvio = 0;

void setup() {
    Serial.begin(115200);

    AICSSLED::inicializar(ledRED, LED_PIN);
    AICSSLED::inicializar(ledGREEN, LED_GREEN_PIN);
    AICSSDHT::inicializar(sensorDHT, DHTPIN, DHTMODEL);
    AICSSPotentiometerCAPACITIVO::inicializar(sensorPot, POT_PIN);
    AICSSPotentiometerANEMOMETRO::inicializar(sensorAnem, ANEM_PIN);
    AICSSBMP10::inicializar(sensorBMP, SDAPIN, SCLPIN);
    AICSSLDR::inicializar(sensorLDR, LDR_PIN);
    AICSSWiFi::conectar(wifiConfig);

    Serial.println("Serviço WebSocket ESP32 iniciado! Envio a cada 5 segundos");
}

void controlarLeds(const AICSSPotentiometerCAPACITIVO::AMBIENTE& solo) {

    if (solo.umidadeSolo >= 3200) {
        AICSSLED::ligar(ledRED);
        AICSSLED::desligar(ledGREEN);
    } else {
        AICSSLED::ligar(ledGREEN);
        AICSSLED::desligar(ledRED);
    }
}

void loop() {
    if (millis() - ultimoEnvio < INTERVALO) return;
    ultimoEnvio = millis();

    Serial.println("TENTANDO LOOP");

    WiFiClient client;

    auto solo = AICSSPotentiometerCAPACITIVO::coletar(sensorPot);
    controlarLeds(solo);

    auto m = AICSSDHT::coletar(sensorDHT);
    if (!m.valida) return;

    auto vento = AICSSPotentiometerANEMOMETRO::coletar(sensorAnem);
    if (!vento.valida) return;

    auto bmp = AICSSBMP10::coletar(sensorBMP);
    if (!bmp.valida) return;

    auto luz = AICSSLDR::coletar(sensorLDR);
    if (!luz.valida) return;

    Serial.printf(
        "T: %.2f U: %.2f IC: %.2f Solo: %d (%s) Vento: %d (%s) bmpt: %.2f Press: %.2f (%s) Luz: %d (%s)\n",
        m.temperatura,
        m.umidade,
        m.indiceCalor,
        solo.umidadeSolo,
        solo.situacao.c_str(),
        vento.velocidadeVento,
        vento.situacao.c_str(),
        bmp.temperatura,
        bmp.pressao,
        bmp.situacao.c_str(),
        luz.luminosidade,
        luz.situacao.c_str()
    );

    if (client.connect(serverHost, serverPort)) {
        JsonDocument doc;

        doc["temperatura"] = m.temperatura;
        doc["umidade"] = m.umidade;
        doc["indiceCalor"] = m.indiceCalor;
        doc["umidadeSolo"] = solo.umidadeSolo;
        doc["situacaoSolo"] = solo.situacao;
        doc["velocidadeVento"] = vento.velocidadeVento;
        doc["situacaoVento"] = vento.situacao;
        doc["temperaturaBMP"] = bmp.temperatura;
        doc["pressaoAtmosferica"] = bmp.pressao;
        doc["situacaoBMP"] = bmp.situacao;
        doc["luminosidade"] = luz.luminosidade;
        doc["situacaoLuz"] = luz.situacao;
        String payload;
        serializeJson(doc, payload);

        client.println(payload);
        Serial.println("Payload enviado");

        client.stop();
    }
}