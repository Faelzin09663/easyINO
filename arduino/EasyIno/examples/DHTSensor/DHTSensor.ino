/*
 * Requer biblioteca DHT sensor library by Adafruit
 * Instale através do Library Manager do Arduino IDE.
 */

#include <EasyIno.h>
#include <DHT.h>

#define DHTPIN 2      // Pino digital conectado ao sensor DHT
#define DHTTYPE DHT11 // ou DHT22

DHT dht(DHTPIN, DHTTYPE);
EasyIno easy(Serial);

float temperatura = 0.0;
float umidade = 0.0;

void setup() {
  easy.begin(115200);
  dht.begin();

  // Registra as variáveis para que o Python leia com board.read("temp")
  easy.addVariable("temp", &temperatura);
  easy.addVariable("umid", &umidade);
}

void loop() {
  easy.update();

  static unsigned long lastUpdate = 0;
  // O DHT é um sensor lento, só faça leituras a cada 2 segundos.
  // Usar millis() aqui impede que o Arduino trave e ignore os comandos Python.
  if (millis() - lastUpdate > 2000) {
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    lastUpdate = millis();
  }
}
