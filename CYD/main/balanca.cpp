#include "balanca.h"

Balanca::Balanca(int dout, int sck, float escala_valor) {
    hx.begin(dout, sck);
    escala = escala_valor;
}

void Balanca::init() {
    hx.set_scale(escala);
    hx.tare();
    Serial.println("Balança iniciada!");
}

float Balanca::getPeso(int media) {
    return hx.get_units(media);
}