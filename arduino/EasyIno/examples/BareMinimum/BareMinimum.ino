/*
  BareMinimum.ino - Um template limpo para começar seus projetos com EasyIno.
  
  Passo 1: Instale a biblioteca "ArduinoJson" no Library Manager da Arduino IDE.
  Passo 2: Faça o upload desse código para sua placa.
  Passo 3: No Python, use `easyino.configure()` e chame suas variáveis e funções!
*/

#include <EasyIno.h>

EasyIno easy(Serial);

// Suas variáveis aqui:
int minha_variavel = 0;

// Suas funções (ações) aqui:
void minha_acao() {
  minha_variavel++;
}

void setup() {
  easy.begin(115200);

  // Registre o que o Python poderá acessar:
  easy.addVariable("contador", &minha_variavel);
  easy.addCommand("somar", minha_acao);
}

void loop() {
  // Obrigatório: Mantenha essa linha viva para escutar o Python
  easy.update();

  // Seu código que precisa rodar a cada instante vai aqui.
  // IMPORTANTE: Evite usar delay() pois ele trava a comunicação com o Python!
}
