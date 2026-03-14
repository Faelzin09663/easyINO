/*
 * EasyIno.ino - Example sketch using the EasyIno library
 * This firmware acts as a bridge between Python and Arduino,
 * delegating protocol details to the EasyIno library.
 */

#include <EasyIno.h>

#define BAUD_RATE 115200

EasyIno easy(Serial);

float temperatura = 24.5;
bool ledStatus = false;

void ligar_led() {
  ledStatus = true;
  digitalWrite(LED_BUILTIN, HIGH);
}

void desligar_led() {
  ledStatus = false;
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  easy.begin(BAUD_RATE);

  // Register Variables
  easy.addVariable("temperatura", &temperatura);
  easy.addVariable("led_status", &ledStatus);

  // Register Commands
  easy.addCommand("ligar_led", ligar_led);
  easy.addCommand("desligar_led", desligar_led);
}

void loop() {
  easy.update();

  // Simulate sensor reading change
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {
    temperatura += (random(-10, 11) / 10.0);
    lastUpdate = millis();
  }
}

