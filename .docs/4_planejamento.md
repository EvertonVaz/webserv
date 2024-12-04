**4. Planejamento de Desenvolvimento:**

Organize o desenvolvimento em etapas, priorizando funcionalidades essenciais.

**Etapa 1: Configuração e Inicialização Básica**

- Implementar o **ConfigParser** para ler o arquivo de configuração.
- Criar a classe **Server** para iniciar sockets de escuta.
- Configurar o **ConnectionManager** com `poll()` para gerenciar conexões.

**Etapa 2: Manipulação Básica de Requisições e Respostas**

- Desenvolver o **HTTPRequest** para analisar requisições.
- Implementar o **HTTPResponse** para gerar respostas simples.
- Suportar o método GET inicialmente.
- Implementar o **StaticFileHandler** para servir arquivos estáticos.

**Etapa 3: Implementar Rotas e Configurações Avançadas**

- Criar a classe **Router** para determinar o manipulador adequado.
- Configurar rotas com base nas especificações (métodos permitidos, redirecionamentos, etc.).
- Suportar múltiplos servidores virtuais com base em `server_name` e porta.

**Etapa 4: Suporte a Métodos POST e Upload de Arquivos**

- Adicionar suporte ao método POST no **HTTPRequest** e **HTTPResponse**.
- Implementar funcionalidade de upload de arquivos nas rotas configuradas.
- Gerenciar limites de tamanho do corpo da requisição.

**Etapa 5: Implementar CGI**

- Desenvolver o **CGIHandler** para executar scripts com base na extensão do arquivo.
- Garantir que funciona com os métodos GET e POST.
- Gerenciar adequadamente a entrada e saída dos scripts CGI.

**Etapa 6: Manuseio de Erros e Resiliência**

- Implementar o **ErrorHandler** para gerar respostas de erro apropriadas.
- Garantir que o servidor não bloqueia ou falha inesperadamente em nenhuma circunstância.
- Realizar testes de estresse para verificar a estabilidade.

**Etapa 7: Funcionalidades Adicionais e Otimizações**

- Adicionar suporte ao método DELETE.
- Implementar logging detalhado com a classe **Logger**.
- Otimizar o desempenho e uso de recursos.
- Realizar testes abrangentes com diferentes navegadores e ferramentas (por exemplo, cURL).

