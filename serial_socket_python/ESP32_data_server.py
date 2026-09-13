import socket
import json
import csv
from datetime import datetime
import pytz

HOST = "0.0.0.0"
PORT = 5000
CSV_FILE = "dados.csv"

br_tz = pytz.timezone("America/Sao_Paulo")

# Criação do cabeçalho do CSV se ele não existir
try:
    with open(CSV_FILE, "x", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "timestamp",
            "temperatura",
            "umidade",
            "indiceCalor",
            "umidadeSolo",
            "situacaoSolo",
            "velocidadeVento",
            "situacaoVento",
            "temperaturaBMP",
            "pressaoAtmosferica",
            "situacaoBMP",
            "luminosidade",
            "situacaoLuz"
        ])
except FileExistsError:
    pass

# Servidor TCP
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Servidor aguardando conexões na porta {PORT}...")

    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Conectado por {addr}")
            data = conn.recv(1024)
            if not data:
                continue

            try:
                payload = json.loads(data.decode())
                print("Dados recebidos:", payload)
                timestamp = datetime.now(br_tz).isoformat()

                with open(CSV_FILE, "a", newline="") as f:
                    writer = csv.writer(f)
                    writer.writerow([
                        timestamp,
                        payload.get("temperatura"),
                        payload.get("umidade"),
                        payload.get("indiceCalor"),
                        payload.get("umidadeSolo"),
                        payload.get("situacaoSolo"),
                        payload.get("velocidadeVento"),
                        payload.get("situacaoVento"),
                        payload.get("temperaturaBMP"),
                        payload.get("pressaoAtmosferica"),
                        payload.get("situacaoBMP"),
                        payload.get("luminosidade"),
                        payload.get("situacaoLuz")
                    ])
                print("Dados gravados no CSV.")

            except json.JSONDecodeError:
                print("Erro: JSON inválido")
