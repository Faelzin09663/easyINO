import easyino
import time
import os

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

print("=== EasyIno Live Debugger ===")
print("Iniciando escuta de eventos...")

def handle_all_events(nome, valor):
    print(f"[{time.strftime('%H:%M:%S')}] 🔔 EVENTO '{nome}': {valor}")

try:
    with easyino.configure() as board:
        board.set_global_event_handler(handle_all_events)
        
        print(f"Conectado em {board.serial.port}")
        print("Aguardando Arduino emitir eventos (Ctrl+C para sair)...\n")
        
        while True:
            # Escuta passivamente a porta serial buscando mensagens "event"
            board.listen()
            time.sleep(0.01)

except Exception as e:
    print(f"Erro no debugger: {e}")
