#pragma once

#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

namespace AICSSBMP10 {

    struct AMBIENTE {
        float temperatura;
        float pressao;
        float altitude;
        String situacao;
        bool valida;
    };

    struct BMP {
    };

    void inicializar(BMP& bmp, int sda, int scl) {
        Wire.begin(sda, scl);

        if (!sensorPressaoAtm.begin()) {
            Serial.println("Erro ao iniciar BMP");
        }
    }

    AMBIENTE coletar(BMP& bmp) {
    AMBIENTE bm;

    bm.temperatura = sensorPressaoAtm.readTemperature();
    bm.pressao     = sensorPressaoAtm.readPressure();
    bm.altitude    = sensorPressaoAtm.readAltitude();

    if (isnan(bm.temperatura) ||
        isnan(bm.pressao) ||
        isnan(bm.altitude)) {

        bm.valida = false;
        return bm;
    }

    if (bm.temperatura < -40 || bm.temperatura > 85 ||
        bm.pressao < 30000 || bm.pressao > 110000 ||
        bm.altitude < -500 || bm.altitude > 10000) {

        bm.valida = false;
        return bm;
    }

    bm.valida = true;

    if (bm.pressao < 97000) {
        bm.situacao = "Muito baixa";
    }
    else if (bm.pressao < 100000) {
        bm .situacao = "Baixa";
    }
    else if (bm.pressao <= 102500) {
        bm.situacao = "Normal";
    }
    else if (bm.pressao <= 105000) {
        bm.situacao = "Alta";
    }
    else {
        bm.situacao = "Muito alta";
    }

    return bm;
    }
}