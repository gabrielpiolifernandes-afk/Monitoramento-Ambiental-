#pragma once

#include <Arduino.h>
#include <DHT.h>

namespace AICSSDHT {

  struct AMBIENTE {
    float temperatura;
    float umidade;
    float indiceCalor;
    bool  valida;
  };

  struct Sensor {
    DHT dht = DHT(0, 0);
  };

  void inicializar(Sensor& s, int pino, int modelo) {
    s.dht = DHT(pino, modelo);
    s.dht.begin();
  }

  AMBIENTE coletar(Sensor& s) {
    AMBIENTE m;
    m.temperatura = s.dht.readTemperature();
    m.umidade     = s.dht.readHumidity();

    if (isnan(m.temperatura) || isnan(m.umidade)) {
      m.valida = false;
      return m;
    }

    m.valida      = true;
    m.indiceCalor = s.dht.computeHeatIndex(m.temperatura, m.umidade, false);
    return m;
  }

}
