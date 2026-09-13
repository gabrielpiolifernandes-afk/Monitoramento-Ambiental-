#pragma once

#include <Arduino.h>

namespace AICSSPotentiometerANEMOMETRO {

  struct AMBIENTE {
    int velocidadeVento;
    String situacao;
    bool valida;
  };

  struct Potentiometer {
    int pino;
  };

  void inicializar(Potentiometer& pot, int pino) {
    pot.pino = pino;
    pinMode(pot.pino, INPUT);
  }

  AMBIENTE coletar(Potentiometer& pot) {
    AMBIENTE m;

    m.velocidadeVento = analogRead(pot.pino);

    if (m.velocidadeVento < 0 || m.velocidadeVento > 4095) {
        m.valida = false;
        return m;
    }

    m.valida = true;

    if (m.velocidadeVento <= 1200) {
        m.situacao = "Muito lento";
    }
    else if (m.velocidadeVento <= 1800) {
        m.situacao = "Lento";
    }
    else if (m.velocidadeVento <= 2500) {
        m.situacao = "Normal";
    }
    else if (m.velocidadeVento <= 3200) {
        m.situacao = "Rapido";
    }
    else {
        m.situacao = "Muito rapido";
    }

    return m;
}

}
