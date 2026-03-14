import easyino
import tkinter as tk

def main():
    print("Conectando ao robô/servo...")
    try:
        board = easyino.commands("pos_0", "pos_90", "pos_180")
    except ConnectionError:
        print("Erro: Arduino não encontrado!")
        return
        
    janela = tk.Tk()
    janela.title("Painel do ServoMotor")
    janela.geometry("300x250")
    
    tk.Label(janela, text="Controle EasyIno Servo", font=("Arial", 14, "bold")).pack(pady=10)
    
    # Label para mostrar o estado atual lido diretamente do arduíno
    lbl_pos = tk.Label(janela, text="Posição atual: ?", font=("Arial", 12))
    lbl_pos.pack(pady=5)
    
    def on_click(comando):
        # Executa o comando
        getattr(board, comando)()
        # Em seguida lê a posição atualizada do arduino
        atual = board.board.read("posicao")
        lbl_pos.config(text=f"Posição atual: {atual}°")

    tk.Button(janela, text="Mover ESQUERDA (0°)", bg="lightblue", 
              command=lambda: on_click("pos_0")).pack(pady=5, fill="x", padx=40)
              
    tk.Button(janela, text="Mover CENTRO (90°)", bg="lightgrey", 
              command=lambda: on_click("pos_90")).pack(pady=5, fill="x", padx=40)
              
    tk.Button(janela, text="Mover DIREITA (180°)", bg="lightpink", 
              command=lambda: on_click("pos_180")).pack(pady=5, fill="x", padx=40)

    # Faz a primeira leitura assim que a interface abre
    try:
        atual = board.board.read("posicao")
        lbl_pos.config(text=f"Posição atual: {atual}°")
    except:
        pass

    janela.mainloop()

if __name__ == "__main__":
    main()
