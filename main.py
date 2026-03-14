import sys
import os

# Add local directory to path for testing
sys.path.append(os.path.abspath("."))

import easyino
import time

def run_demo():
    print("🚀 EasyIno Demo")
    print("Attempting to connect...")
    
    try:
        # For manual testing, you'd specify the port: board = easyino.configure("COM5")
        # Here we use auto-discovery
        board = easyino.configure()
        print(f"✅ Connected to {board.serial.port}")
        
        print("\n--- Reading Data ---")
        temp = board.read("temperatura")
        print(f"🌡️ Temperature: {temp}°C")
        
        status = board.read("led_status")
        print(f"💡 LED Status: {'On' if status else 'Off'}")
        
        print("\n--- Executing Actions ---")
        print("Toggling LED...")
        board.execute("ligar_led")
        time.sleep(1)
        
        new_status = board.read("led_status")
        print(f"💡 New LED Status: {'On' if new_status else 'Off'}")
        
        board.execute("desligar_led")
         print("✅ Demo finished successfully!")
        
    except ConnectionError as e:
        print(f"❌ Error: {e}")
    except Exception as e:
        print(f"⚠️ Unexpected error: {e}")

if __name__ == "__main__":
    run_demo()
