#include <EasyIno.h>
#include <Servo.h>

EasyIno easy(Serial);
Servo meuServo;

// Variável que o Python pode ler para saber onde o servo está atualmente
int posicaoAtual = 90;

void mover_para_0() {
  posicaoAtual = 0;
  meuServo.write(posicaoAtual);
}

void mover_para_90() {
  posicaoAtual = 90;
  meuServo.write(posicaoAtual);
}

void mover_para_180() {
  posicaoAtual = 180;
  meuServo.write(posicaoAtual);
}

void setup() {
  easy.begin(115200);
  
  meuServo.attach(9); // Pino 9
  meuServo.write(posicaoAtual);

  easy.addVariable("posicao", &posicaoAtual);
  
  easy.addCommand("pos_0", mover_para_0);
  easy.addCommand("pos_90", mover_para_90);
  easy.addCommand("pos_180", mover_para_180);
}

void loop() {
  easy.update();
}
