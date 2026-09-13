#pragma once

#include <Arduino.h>

namespace AICSSLDR {

  struct AMBIENTE {
    int luminosidade;
    String situacao;
    bool valida;
  };

  struct Sensor {
    int pino;
  };

  void inicializar(Sensor& sensor, int pino) {
    sensor.pino = pino;
    pinMode(sensor.pino, INPUT);
  }

  AMBIENTE coletar(Sensor& sensor) {
    AMBIENTE m;

    m.luminosidade = digitalRead(sensor.pino);

    if (m.luminosidade < 0 || m.luminosidade > 1) {
      m.valida = false;
      return m;
    }

    m.valida = true;

    if (m.luminosidade <= 0) {
      m.situacao = "escuro";
    } else {
      m.situacao = "iluminado";
    }

    return m;
  }
}