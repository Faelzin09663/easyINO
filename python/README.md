# 🔌 EasyIno 🚀

**Menos código, mais automação. A biblioteca "Zero-Config" definitiva que conecta Python e Arduino.**

![PyPI](https://img.shields.io/pypi/v/easyino?color=blue)
![License](https://img.shields.io/github/license/Faelzin09663/easyINO)

`easyino` é o facilitador definitivo para projetos de **Python + Arduino**. Esqueça a conversão manual de Strings, Parsing complexo, `Serial.read()` infinito e a necessidade de especificar portas COM manualmente. 

Com a EasyIno, você simplesmente **registra variáveis (sensores/estados)** e **comandos (ações)** no seu código Arduino. No Python, a "Mágica do Auto-Discovery (RPC)" cria os métodos para você automaticamente, permitindo controlar sua placa com apenas **3 linhas de código**.

---

## ✨ Por que usar a EasyIno?

*   🎩 **Mágica do Auto-Discovery**: O Python pergunta ao Arduino o que ele sabe fazer e gera as funções dinamicamente!
*   📡 **Acesso Direto a Variáveis**: Leia sensores e estados como atributos de objeto: `temp = board.temperatura`.
*   🎯 **Execução de Comandos**: Chame funções do Arduino como métodos nativos: `board.ligar_led()`.
*   🔌 **Auto-detecção de Porta**: O sistema localiza onde o Arduino está conectado (COM, /dev/tty) sem intervenção manual.
*   🔔 **Eventos Proativos**: A placa pode emitir alertas espontâneos (ex: detecção de movimento) sem que o Python precise perguntar.
*   ⚡ **Tipagem Automática via JSON**: Comunicação robusta e transparente usando o padrão JSON.

---

## 📦 Instalação

### 1. No Python
Instale via pip:
```bash
pip install easyino
```

### 2. No Arduino IDE
1.  **Dependência Obrigatória**: No Arduino IDE, vá em `Sketch` -> `Include Library` -> `Manage Libraries...` e instale a biblioteca **ArduinoJson** (Versão 6 ou 7).
2.  **Biblioteca EasyIno**: Copie a pasta `arduino/EasyIno` deste repositório para a sua pasta de bibliotecas do Arduino (geralmente em `Documentos/Arduino/libraries/`).
3.  Reinicie a IDE do Arduino.

---

## 🚀 Quick Start: O Clássico "Pisca LED" Remoto

### 1. No Arduino (`.ino`)
Abra o exemplo `File > Examples > EasyIno > BareMinimum` ou use o código abaixo:

```cpp
#include <EasyIno.h>

EasyIno easy(Serial);
int ledStatus = 0;

void alternar_led() {
  ledStatus = !ledStatus;
  digitalWrite(LED_BUILTIN, ledStatus);
}

void setup() {
  easy.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Registramos a variável e o comando para o Python!
  easy.addVariable("estado_led", &ledStatus);
  easy.addCommand("piscar", alternar_led);
}

void loop() {
  easy.update(); // Mantém a comunicação ativa
}
```

### 2. No Python (Simples assim!)
```python
import easyino
import time

# O configure() encontra a porta sozinho!
with easyino.configure() as board:
    print(f"Conectado na porta: {board.serial.port}")
    
    # Chama o comando 'piscar' definido no Arduino
    board.piscar()
    
    # Lê a variável 'estado_led' do Arduino
    print(f"O LED agora está: {board.estado_led}")
```

---

## 🛠️ API Detalhada

### Python (Classe `EasyIno`)

*   **`configure(port=None, baudrate=115200)`**: Atalho para conectar. Se `port` for omitido, usa auto-discovery.
*   **`board.execute(name, args=None)`**: Executa um comando remoto. (Atalho: `board.nome_do_comando()`).
*   **`board.read(name)`**: Lê o valor de uma variável remota. (Atalho: `valor = board.nome_da_variavel`).
*   **`board.on_event(name, callback)`**: Registra uma função para ser chamada quando o Arduino enviar um evento via `notify()`.
*   **`board.ping()`**: Verifica se a placa está respondendo.
*   **`board.get_schema()`**: Retorna a lista de todos os comandos e variáveis disponíveis na placa conectada.

### Arduino (Classe `EasyIno`)

*   **`easy.begin(baudrate)`**: Inicializa a comunicação (padrão: 115200).
*   **`easy.addVariable(name, &var)`**: Expõe uma variável (`int`, `float`, `bool`) para o Python.
*   **`easy.addCommand(name, function)`**: Expõe uma função `void` para ser chamada pelo Python.
*   **`easy.notify(event_name, value)`**: Envia um evento imediato para o Python (útil para sensores de interrupção ou alarmes).
*   **`easy.update()`**: **Obrigatório** no `loop()` para processar as requisições.

---

## 📡 Exemplo de Automação de 5 Linhas

```python
import easyino

board = easyino.configure() # Conecta automático

while True:
    if board.sensor_presenca: # Lê variável do Arduino
        board.tocar_alarme()  # Executa comando no Arduino
        break
```

---

## 🔍 Live Debugger e Exemplos
Quer testar sua placa sem escrever código Python? Use nosso depurador visual:
```bash
python -m easyino.examples.debugger
```
Explore a pasta `examples/` para ver implementações com **Servo Motores, Sensores DHT, Displays** e muito mais!

---

## 🤝 Contribuição
Sinta-se à vontade para abrir Issues ou enviar Pull Requests. Vamos construir a melhor ferramenta para a comunidade Maker!

---
**Desenvolvido com ❤️ por Faelzin09663 e comunidade.**
