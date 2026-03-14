import easyino
import tkinter as tk

def main():
    try:
        # Atalho para registrar os comandos do Arduino em um objeto
        board = easyino.commands("ligar_led", "desligar_led")
    except ConnectionError:
        print("Erro: Arduino não encontrado. Conecte a placa e tente novamente.")
        return

    # Cria interface gráfica básica
    janela = tk.Tk()
    janela.title("Controle EasyIno")
    janela.geometry("300x150")

    tk.Label(janela, text="Controle do LED", font=("Arial", 14)).pack(pady=10)

    # Botões chamam os comandos diretamente
    tk.Button(janela, text="💡 Ligar LED", bg="lightgreen", font=("Arial", 12),
              command=board.ligar_led).pack(pady=5, fill="x", padx=50)
              
    tk.Button(janela, text="🌑 Desligar LED", bg="salmon", font=("Arial", 12),
              command=board.desligar_led).pack(pady=5, fill="x", padx=50)

    janela.mainloop()

if __name__ == "__main__":
    main()
