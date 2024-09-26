**5. Fluxograma de Desenvolvimento:**

Embora não possa desenhar aqui, você pode criar um fluxograma que representa:

- **Inicialização do Servidor:**
  - Carrega configurações -> Inicia servidores virtuais -> Entra no loop principal.

- **Loop Principal (para cada evento de `poll()`):**
  - **Nova Conexão:**
    - Aceita conexão e adiciona ao `poll()`.

  - **Dados Recebidos:**
    - Lê dados -> Analisa com **HTTPRequest** -> Passa para o **Router**.

  - **Roteamento:**
    - Determina o manipulador (StaticFileHandler, CGIHandler, ErrorHandler).

  - **Resposta:**
    - Gera resposta com **HTTPResponse** -> Envia ao cliente.

  - **Conexão Encerrada:**
    - Remove a conexão do `poll()` e libera recursos.

