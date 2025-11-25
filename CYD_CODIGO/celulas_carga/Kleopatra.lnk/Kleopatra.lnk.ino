#include <HX711.h>

#define HX711_DOUT 27
#define HX711_SCK 22

HX711 balanca;
float escala = -7050;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando balança...");

  balanca.begin(HX711_DOUT, HX711_SCK);
  balanca.set_scale(escala);
  balanca.tare();

  Serial.println("Balança pronta!");
}

void loop() {
  float peso = balanca.get_units(10);
  Serial.print("Peso: ");
  Serial.print(peso, 2);
  Serial.println(" kg");

  delay(200);
}
