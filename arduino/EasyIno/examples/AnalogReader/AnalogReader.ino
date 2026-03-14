#include <EasyIno.h>

EasyIno easy(Serial);

// Variável para armazenar o valor lido do pino analógico A0
int sensorValor = 0;
// Variável convertida para voltagem (opcional, apenas para mostrar tipos float)
float voltagem = 0.0;

void setup() {
  // Inicializa o sensor no A0 e a biblioteca EasyIno
  pinMode(A0, INPUT);
  
  easy.begin(115200);

  // Registra as variáveis. O Python poderá ler "valor_bruto" e "voltagem" diretamente.
  easy.addVariable("valor_bruto", &sensorValor);
  easy.addVariable("voltagem", &voltagem);
}

void loop() {
  // Sempre chame update no loop para manter a comunicação ativa
  easy.update();

  // Leitura Real
  sensorValor = analogRead(A0);
  voltagem = sensorValor * (5.0 / 1023.0);
  
  // Pequeno delay (não use delays grandes pois eles bloqueiam a recepção de comandos)
  delay(10);
}
