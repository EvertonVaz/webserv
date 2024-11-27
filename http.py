#bin/python3
import socket
import time

def send_get_request(host, port, method, path, http_version="HTTP/1.1"):
    method = method.upper()
    http_version = http_version.upper()
    try:
        socket_cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket_cliente.connect((host, port))

        request = f"{method} {path} {http_version}\r\nHost: {host}\r\n\r\n"
        socket_cliente.send(request.encode())
        response = b""
        data = socket_cliente.recv(4096)
        response += data
        socket_cliente.close()
        return response.decode()
    except Exception as e:
        print(f"Erro ao conectar ou comunicar com o servidor: {e}")
        return None

if __name__ == "__main__":
    HOST = "127.0.0.1"      # Substitua pelo host desejado
    PORT = 8080             # Substitua pela porta desejada
    PATH = "/upload"              # Substitua pelo caminho desejado
    VERSION = "HTTP/1.1"    # Substitua pela versão desejada
    METHOD = "GET"       # Substitua pelo método


    resposta = send_get_request(HOST, PORT, METHOD, PATH, VERSION)

    if resposta:
        headers, _, body = resposta.partition("\r\n\r\n")
        print("Cabeçalho da Resposta:")
        print(headers)
