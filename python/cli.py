import easyino, sys

with easyino.configure() as board:
    cmd = sys.argv[1] if len(sys.argv) > 1 else "ligar_led"
    board.execute(cmd)
    print(f"✅ Comando '{cmd}' enviado com sucesso!")
