import easyino
import time

print("Conectando ao Arduino...")
with easyino.configure() as board:
    print("Conectado! Lendo sensores...")
    
    while True:
        try:
            # Lê valores do Arduino
            temp = board.read("temperatura")
            led = board.read("led_status")
            
            estado = "Ligado" if led else "Desligado"
            print(f"Temperatura: {temp:.2f}°C | Status do LED: {estado}")
            
            time.sleep(1)
        except KeyboardInterrupt:
            print("\nSaindo...")
            break
        except Exception as e:
            print(f"Erro na leitura: {e}")
            time.sleep(2)
