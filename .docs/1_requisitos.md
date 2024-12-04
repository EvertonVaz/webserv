**Análise dos Requisitos:**

Antes de começar, é essencial compreender completamente os requisitos do projeto:

- **Configuração:**
  - O servidor deve aceitar um arquivo de configuração ou usar um caminho padrão.
  - Deve ser capaz de ouvir em múltiplas portas.

- **Funcionamento:**
  - Não deve bloquear ou encerrar inesperadamente.
  - Deve ser não bloqueante e usar apenas um `poll()` (ou equivalente) para todas as operações de I/O.
  - Deve suportar múltiplas conexões simultâneas.

- **Compatibilidade:**
  - Deve ser compatível com navegadores web modernos.
  - Deve ser compatível com o HTTP 1.1, similar ao NGINX.

- **Funcionalidades:**
  - Suportar os métodos HTTP: GET, POST e DELETE.
  - Servir arquivos estáticos.
  - Suportar CGIs baseados em extensões de arquivo (por exemplo, `.php`).
  - Permitir upload de arquivos.
  - Fornecer páginas de erro padrão.
  - Permitir configuração detalhada de rotas.

