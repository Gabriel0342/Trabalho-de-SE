#ifndef BALANCA_H
#define BALANCA_H

#include <HX711.h>

class Balanca {
    public:
        Balanca(int dout, int sck, float escala_valor);
        void init();
        float getPeso(int media = 10);
    private:
        HX711 hx;
        float escala;
};

#endif