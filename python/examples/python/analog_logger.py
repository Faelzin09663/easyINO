import easyino
import time
import os

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

print("Buscando Arduino...")
with easyino.configure() as board:
    print(f"Conectado via {board.serial.port}!")
    time.sleep(1)
    
    print("Iniciando monitoramento de Potenciômetro/LDR...")
    
    try:
        while True:
            # Lé do AnalogReader.ino
            valor = board.read("valor_bruto")
            volts = board.read("voltagem")
            
            # Desenha uma barra de progresso simples no terminal
            barra_tamanho = 30
            progresso = int((valor / 1023) * barra_tamanho)
            barra = "#" * progresso + "-" * (barra_tamanho - progresso)
            
            clear_screen()
            print("=== Monitor Analógico ===")
            print(f"Sinal (0-1023) : {valor:4d}  [{barra}]")
            print(f"Voltagem (0-5) : {volts:.2f}v")
            print("\nPressione Ctrl+C para sair.")
            
            # FPS do console
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\nMonitoramento encerrado.")
