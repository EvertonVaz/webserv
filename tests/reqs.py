#bin/python3
import socket

def send_get_request(host, port, method, path, http_version="HTTP/1.1"):
    method = method.upper()
    http_version = http_version.upper()
    try:
        socket_cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket_cliente.connect((host, port))
        request = f"{method} {path} {http_version}\r\nHost: {host}\r\nConnection: close\r\n\r\n"
        socket_cliente.send(request.encode())
        response = b""
        data = socket_cliente.recv(4096)
        response += data
        socket_cliente.close()
        return response.decode()
    except Exception as e:
        print(f"Erro ao conectar ou comunicar com o servidor: {e}")
        return None


def send_post_chunked_request(path, data, chunk_size=1024, http_version="HTTP/1.1"):
    host = "localhost"
    port = 8080
    http_version = http_version.upper()
    filename = "testFile.txt"

    try:
        socket_cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket_cliente.connect((host, port))
        request = (
            f"POST {path} {http_version}\r\n"
            f"Host: {host}\r\n"
            f"Content-Type: application/octet-stream\r\n"
            f"Transfer-Encoding: chunked\r\n"
            f"Connection: close\r\n\r\n"
        )
        socket_cliente.send(request.encode())

        # Enviar dados em chunks
        for i in range(0, len(data), chunk_size):
            chunk = data[i:i + chunk_size].encode()  # Codifica o chunk para bytes
            chunk_header = f"{len(chunk):X}\r\n".encode()
            socket_cliente.send(chunk_header + chunk + b"\r\n")

        # Enviar o chunk final
        socket_cliente.send(b"0\r\n\r\n")

        response = b""
        while True:
            recv_data = socket_cliente.recv(4096)
            if not recv_data:
                break
            response += recv_data
        socket_cliente.close()
        return response.decode()
    except Exception as e:
        print(f"Erro ao conectar ou comunicar com o servidor: {e}")
        return None


if __name__ == "__main__":
    HOST = "127.0.0.1"      # Substitua pelo host desejado
    PORT = 8080             # Substitua pela porta desejada
    PATH = "/"              # Substitua pelo caminho desejado
    VERSION = "HTTP/1.1"    # Substitua pela versão desejada
    METHOD = "GET"       # Substitua pelo método
    get = False # send_get_request(HOST, PORT, METHOD, PATH, VERSION)
    if get:
        headers, _, body = get.partition("\r\n\r\n")
        print("Cabeçalho da Resposta GET:")
        print(headers)

    data = "a" * 1024 * 1024 * 15
    post = False #send_post_chunked_request("/upload", data)
    if post:
        headers, _, body = post.partition("\r\n\r\n")
        print("Cabeçalho da Resposta POST:")
        print(headers)

    with open("testFile.txt", "w") as f:
        f.write(data)