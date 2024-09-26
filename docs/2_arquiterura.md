**Definição da Arquitetura:**

Divida o servidor em módulos para facilitar o desenvolvimento e manutenção:

- **Gerenciamento de Configuração:**
  - **ConfigParser**: Carrega e interpreta o arquivo de configuração.
  - **ServerConfig**: Armazena as configurações de cada servidor virtual.

- **Core do Servidor:**
  - **Server**: Gerencia a inicialização e encerramento do servidor.
  - **ConnectionManager**: Gerencia todas as conexões ativas e utiliza `poll()`.

- **Processamento de Requisições:**
  - **HTTPRequest**: Analisa as requisições HTTP recebidas.
  - **HTTPResponse**: Constrói as respostas HTTP para enviar aos clientes.
  - **Router**: Determina o tratamento das requisições com base nas rotas configuradas.

- **Manipuladores Específicos:**
  - **StaticFileHandler**: Serve arquivos estáticos.
  - **CGIHandler**: Executa scripts CGI e processa suas saídas.
  - **ErrorHandler**: Gera respostas de erro apropriadas.

- **Utilidades:**
  - **Logger**: Registra logs de atividades e erros.
  - **Utils**: Funções auxiliares (por exemplo, parsing de strings, manipulação de MIME types).

