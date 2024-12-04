**3. Design das Classes:**

**a. Configuração:**

- **ConfigParser**
  - Responsável por ler e interpretar o arquivo de configuração.
  - Gera objetos `ServerConfig`.

- **ServerConfig**
  - Contém as configurações específicas de cada servidor (host, porta, rotas, etc.).

**b. Core do Servidor:**

- **Server**
  - Inicializa os sockets de escuta com base nas configurações.
  - Aceita novas conexões.

- **ConnectionManager**
  - Utiliza `poll()` para monitorar eventos de I/O.
  - Gerencia o ciclo de vida das conexões.

**c. Processamento de Requisições:**

- **HTTPRequest**
  - Analisa a requisição recebida (método, URL, headers, corpo).

- **HTTPResponse**
  - Constrói a resposta HTTP (status code, headers, corpo).

- **Router**
  - Determina qual manipulador deve ser usado com base na rota e método.

**d. Manipuladores Específicos:**

- **StaticFileHandler**
  - Serve arquivos estáticos do sistema de arquivos.

- **CGIHandler**
  - Executa scripts CGI e processa a entrada e saída.

- **ErrorHandler**
  - Gera páginas de erro padrão ou personalizadas.

**e. Utilidades:**

- **Logger**
  - Registra atividades do servidor e erros para depuração.

- **Utils**
  - Funções auxiliares necessárias em várias partes do servidor.

