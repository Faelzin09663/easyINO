import json
import time
try:
    import serial  # type: ignore
    import serial.tools.list_ports  # type: ignore
except Exception as e:  # pragma: no cover
    raise ImportError(
        "Dependência serial incorreta. Instale o PySerial e remova o pacote 'serial'.\n"
        "Execute:\n"
        "  pip uninstall -y serial\n"
        "  pip install pyserial\n"
    ) from e
from typing import List, Dict, Optional, Any

class EasyIno:
    def __init__(self, port=None, baudrate=115200, timeout=1):
        if port is None:
            port = self._find_arduino()

        if port is None:
            raise ConnectionError("No Arduino found. Please specify the port manually.")

        self.serial = serial.Serial(port, baudrate, timeout=timeout)
        self._event_callbacks = {}
        self._on_any_event = None
        
        # Give Arduino time to reset after connection
        time.sleep(2)

    # Context manager: with EasyIno(...) as board:
    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()

    def _find_arduino(self):
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            # Common Arduino VID/PID patterns
            if "Arduino" in p.description or "CH340" in p.description or "USB Serial" in p.description:
                return p.device
        return None

    def _send_receive(self, data):
        message = json.dumps(data) + "\n"
        self.serial.write(message.encode("utf-8"))
        
        # Read lines until we find a non-event response
        while True:
            line = self.serial.readline().decode("utf-8").strip()
            if not line:
                return {"status": "error", "message": "No response from Arduino"}
                
            try:
                response = json.loads(line)
                
                # If it's a proactive event from Arduino, invoke callback if registered
                if "event" in response:
                    event_name = response["event"]
                    event_value = response.get("value")
                    if event_name in self._event_callbacks:
                        self._event_callbacks[event_name](event_value)
                    elif self._on_any_event:
                        self._on_any_event(event_name, event_value)
                    
                    # Se mandamos um comando e recebemos um evento, precisamos continuar esperando
                    # a resposta real do nosso comando. Se não mandamos comando (ex: escuta passiva),
                    # retornamos o evento.
                    if data.get("action") == "listen_only":
                        return response
                    continue
                    
                return response
            except json.JSONDecodeError:
                return {"status": "error", "message": f"Invalid JSON response: {line}"}

    def on_event(self, event_name, callback):
        """Registra uma função para ser chamada quando o Arduino emitir um evento."""
        if not hasattr(self, '_event_callbacks'):
            self._event_callbacks = {}
        self._event_callbacks[event_name] = callback

    def set_global_event_handler(self, callback):
        """Registra uma função para escutar TODOS os eventos emitidos pelo Arduino."""
        self._on_any_event = callback

    def listen(self, timeout_sec=0.1):
        """Escuta a porta serial passivamente por eventos sem enviar comandos."""
        old_timeout = self.serial.timeout
        self.serial.timeout = timeout_sec
        try:
            self._send_receive({"action": "listen_only"})
        finally:
            self.serial.timeout = old_timeout

    def read(self, name):
        """Read a variable value from the Arduino."""
        response = self._send_receive({"action": "read", "name": name})
        if response and response.get("status") == "ok":
            return response.get("value")
        else:
            msg = response.get('message') if response else "No response"
            raise KeyError(f"Error reading '{name}': {msg}")

    def execute(self, name: str, args: Optional[List[Any]] = None) -> bool:
        """Execute a command on the Arduino."""
        cmd = {"action": "exec", "name": name} # type: ignore
        if args is not None:
            cmd["args"] = args # type: ignore
            
        response = self._send_receive(cmd)
        if response and response.get("status") == "ok":
            return True
        else:
            msg = response.get('message') if response else "No response"
            raise RuntimeError(f"Error executing '{name}': {msg}")

    def ping(self) -> bool:
        """Checa se o Arduino está respondendo."""
        response = self._send_receive({"action": "ping"})
        return bool(response and response.get("status") == "ok")

    def get_schema(self) -> dict:
        """Solicita ao Arduino a lista de variáveis e comandos registrados."""
        response = self._send_receive({"action": "schema"})
        if response and response.get("status") == "ok":
            return {
                "variables": response.get("variables", []),
                "commands": response.get("commands", [])
            }
        return {"variables": [], "commands": []}

    def __getattr__(self, name: str):
        """
        Magia do Auto-Discovery.
        Permite chamar board.ligar_led() ou ler board.temperatura diretamente
        sem precisar chamar board.execute("ligar_led") ou board.read("temperatura").
        Fazemos validação via get_schema() apenas na primeira vez que um atributo é pedido.
        """
        if not hasattr(self, "_schema"):
            self._schema = self.get_schema()

        if name in self._schema["commands"]:
            # Retorna uma função callable que executa o comando
            return lambda *args: self.execute(name, list(args) if args else None)
            
        if name in self._schema["variables"]:
            # Lê o valor imediatamente (isso o torna um atributo "get" virtual)
            return self.read(name)

        raise AttributeError(f"O Arduino não registrou o comando ou variável '{name}'. Você chamou addCommand() ou addVariable() para ele?")

    def close(self):
        self.serial.close()

def configure(port=None, baudrate=115200):
    """Helper function to quickly connect to an Arduino."""
    return EasyIno(port=port, baudrate=baudrate)

def available_ports() -> List[str]:
    """Lista portas seriais disponíveis (ex: COM3, COM5)."""
    return [p.device for p in serial.tools.list_ports.comports()]


class _CommandProxy:
    """
    Lightweight helper so the user can call:

        cmds = easyino.commands("ligar_led", "desligar_led", port="COM5")
        cmds.ligar_led()
        cmds.desligar_led()

    without escrever execute(...) toda hora.
    """

    def __init__(self, board: EasyIno, names: List[str]):
        self._board = board
        for name in names:
            # Cria um método para cada comando remoto
            def make_func(remote_name):
                def _f():
                    return self._board.execute(remote_name)

                return _f

            setattr(self, name, make_func(name))

    @property
    def board(self) -> EasyIno:
        return self._board


def commands(*names: str, port=None, baudrate: int = 115200) -> _CommandProxy:
    """
    Atalho de alto nível para registrar comandos simples.

    Exemplo:
        from easyino import commands
        cmds = commands("ligar_led", "desligar_led", port="COM5")
        cmds.ligar_led()
        cmds.desligar_led()
    """
    board = configure(port=port, baudrate=baudrate)
    return _CommandProxy(board, list(names))
