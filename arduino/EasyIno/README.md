# EasyIno (Arduino Edition)

A biblioteca oficial para a placa Arduino (C++) do ecossistema **EasyIno**.
Desenvolvida para trabalhar em conjunto com o pacote Python `easyino`.

## O que é?
A EasyIno abstrai toda a complexidade da comunicação Serial. Em vez de ler e interpretar Strings manualmente, você apenas registra variáveis e funções na placa. O Python se encarrega de descobri-las automaticamente (RPC Auto-Discovery).

## Instalação
1. Certifique-se de ter a biblioteca **ArduinoJson** instalada através do Library Manager.
2. Extraia esta pasta `EasyIno` dentro do diretório `Documentos/Arduino/libraries/`.
3. Reinicie a IDE do Arduino.

## Exemplo Rápido (BareMinimum)
```cpp
#include <EasyIno.h>

EasyIno easy(Serial);
int ledStatus = 0;

void setup() {
  easy.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Registra a variável e a função para o Python enxergar!
  easy.addVariable("estado_led", &ledStatus);
  easy.addCommand("piscar", [](){ 
    ledStatus = !ledStatus; 
    digitalWrite(LED_BUILTIN, ledStatus); 
  });
}

void loop() {
  easy.update(); // Mantém a comunicação viva e escutando o Python
}
```

## Como usar com o Python
No seu computador, instale o pacote via terminal:
```bash
pip install easyino
```

E controle a placa com apenas 3 linhas:
```python
import easyino
with easyino.configure() as arduino:
    arduino.piscar() # Executa a função do C++!
    print(arduino.estado_led) # Lê a variável do C++!
```

Para documentação completa, ferramentas de debug visual e exemplos avançados, visite o repositório oficial no GitHub.
