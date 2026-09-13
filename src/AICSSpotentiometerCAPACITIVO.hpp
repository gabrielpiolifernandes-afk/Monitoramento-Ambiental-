#pragma once

#include <Arduino.h>

namespace AICSSPotentiometerCAPACITIVO {

  struct AMBIENTE {
    int umidadeSolo;
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

    m.umidadeSolo = analogRead(pot.pino);

    if (m.umidadeSolo < 0 || m.umidadeSolo > 4095) {
        m.valida = false;
        return m;
    }

    m.valida = true;

    if (m.umidadeSolo <= 1200) {
        m.situacao = "Muito molhado";
    }
    else if (m.umidadeSolo <= 1800) {
        m.situacao = "Umido";
    }
    else if (m.umidadeSolo <= 2500) {
        m.situacao = "Normal";
    }
    else if (m.umidadeSolo <= 3200) {
        m.situacao = "Seco";
    }
    else {
        m.situacao = "Muito seco";
    }

    return m;
}

}
