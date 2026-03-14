import easyino
import time
import datetime

print("Conectando ao Arduíno (DHT)...")
with easyino.configure() as board:
    print(f"Iniciando coleta na porta: {board.serial.port}")
    print("Salvando dados em 'temperaturas.csv'...")
    
    # Cria o arquivo e escreve o cabeçalho
    with open("temperaturas.csv", "w", encoding="utf-8") as file:
        file.write("Horario,Temperatura_C,Umidade_%\n")
    
    try:
        while True:
            agora = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            
            # Lê as variáveis registradas no Arduino
            temp = board.read("temp")
            umid = board.read("umid")
            
            if temp is not None and umid is not None:
                linha = f"{agora},{temp:.1f},{umid:.1f}"
                
                print(f"[{agora}] Temp: {temp:.1f}°C | Umid: {umid:.1f}%")
                
                # Salva no arquivo
                with open("temperaturas.csv", "a", encoding="utf-8") as file:
                    file.write(linha + "\n")
            
            # Aguarda 2 segundos (DHT é lento)
            time.sleep(2)
            
    except KeyboardInterrupt:
        print("\nColeta finalizada!")
