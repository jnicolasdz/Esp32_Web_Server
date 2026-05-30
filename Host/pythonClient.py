import subprocess
import sys
from datetime import datetime
from fastapi import FastAPI, Request
import uvicorn

GREEN  = "\033[0;32m"
CYAN   = "\033[0;36m"
YELLOW = "\033[1;33m"
RESET  = "\033[0m"

app = FastAPI()

@app.post("/print")
async def recibir_mensaje(request: Request):
    client_ip = request.client.host
    mensaje   = (await request.body()).decode("utf-8")
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    print(f"{CYAN}────────────────────────────────────────{RESET}")
    print(f"  {YELLOW}[{timestamp}]{RESET}  IP: {CYAN}{client_ip}{RESET}")
    print(f"  {GREEN}Mensaje → {mensaje}{RESET}")
    print(f"{CYAN}────────────────────────────────────────{RESET}")
    print()

    return "OK"

@app.post("/exit")
async def shutdown():
    print(f"{YELLOW}Recibida solicitud de apagado. Deteniendo servidor...{RESET}")
    uvicorn.Server.should_exit = True
    return "Servidor apagándose..."

@app.post("/suspend")
async def suspend():
    print(f"{YELLOW}Recibida solicitud de suspensión. Suspendiendo el sistema...{RESET}")
    await subprocess.run("sleep 60 && echo mem | sudo tee /sys/power/state", shell=True, check=True)
    return "Sistema apagandose en un minuto..."

if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8080

    print(f"{GREEN}========================================{RESET}")
    print(f"{GREEN}   ESP32 Receiver  —  puerto {port}{RESET}")
    print(f"{GREEN}========================================{RESET}")
    print(f"  Escuchando en {CYAN}http://0.0.0.0:{port}/{RESET}")
    print(f"  Presiona {YELLOW}Ctrl+C{RESET} para detener.")
    print()

    uvicorn.run(app, host="0.0.0.0", port=port, log_level="error")
