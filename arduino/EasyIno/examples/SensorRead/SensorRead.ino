/*
 * EasyIno - SensorRead example
 * Exposes a sensor variable "pot" (0..1023) to Python via board.read("pot")
 */

#include <EasyIno.h>

EasyIno easy(Serial);

int pot = 0;
const int POT_PIN = A0;

void setup() {
  easy.begin(115200);
  easy.addVariable("pot", &pot);
}

void loop() {
  pot = analogRead(POT_PIN);
  easy.update();
}

