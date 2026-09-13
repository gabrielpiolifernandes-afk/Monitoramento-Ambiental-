#pragma once

#include <Arduino.h>

namespace AICSSLED {

struct LED {
    int pino;
    unsigned long tempoAnterior = 0;
    bool estado = false;
};

  void inicializar(LED& led, int pino) {
    led.pino = pino;
    pinMode(led.pino, OUTPUT);
  }

  void ligar(LED& led) {
    digitalWrite(led.pino, HIGH);
  }

  void desligar(LED& led) {
    digitalWrite(led.pino, LOW);
  }

  void piscar(LED& led, int intervalo = 1000) {
    unsigned long tempoAtual = millis();

    if ((tempoAtual - led.tempoAnterior) >= intervalo) {
      led.tempoAnterior = tempoAtual;
      led.estado = !led.estado;
      digitalWrite(led.pino, led.estado);
    }
}

}
