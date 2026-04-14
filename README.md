# 📝 Desafio GitHub Markdown — Gerenciador de Tarefas em C

<div align="center">

[![Linguagem](https://img.shields.io/badge/Linguagem-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Compilador](https://img.shields.io/badge/Compilador-GCC-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)](https://gcc.gnu.org/)
[![Plataforma](https://img.shields.io/badge/Plataforma-Linux%20%7C%20Windows-0078D6?style=for-the-badge&logo=linux&logoColor=white)](https://github.com/IgorASB/desafio-github-markdown)
[![Status](https://img.shields.io/badge/Status-Concluído-28a745?style=for-the-badge)](https://github.com/IgorASB/desafio-github-markdown)
[![GitHub](https://img.shields.io/badge/GitHub-IgorASB-181717?style=for-the-badge&logo=github)](https://github.com/IgorASB)

</div>

---

> Repositório criado para o **Desafio GitHub Markdown**, onde a documentação em si é o projeto. O arquivo documentado é um **Gerenciador de Tarefas** desenvolvido em linguagem C, com persistência em arquivo binário e gerenciamento dinâmico de memória.

---

## 📌 Sobre o Projeto

O `Gerenciador-de-tarefas.c` é um programa de terminal interativo que permite ao usuário gerenciar tarefas simples com persistência automática de dados. Toda vez que o programa é encerrado, as tarefas são salvas em um arquivo binário e recarregadas na próxima execução — sem perda de dados.

**Principais diferenciais:**
- 🔄 Persistência automática via arquivo binário (`tarefas.dat`)
- 🧠 Alocação dinâmica de memória com `malloc` / `realloc` / `free`
- 📋 Interface de menu limpa e intuitiva no terminal

---

## 🖥️ Demonstração

```
=== GERENCIADOR DE TAREFAS ===
1 - Listar tarefas
2 - Adicionar tarefa
3 - Concluir tarefa
0 - Sair
Opção: 1

[1] Estudar ponteiros em C     - Pendente
[2] Fazer exercício de structs - Concluída
[3] Revisar alocação dinâmica  - Pendente
```

Ao sair (opção `0`), todas as tarefas são **salvas automaticamente** no arquivo `tarefas.dat`.

---

## ⚙️ Funcionalidades

| # | Funcionalidade | Descrição |
|---|---|---|
| 1 | 📋 Listar | Exibe todas as tarefas com ID, título e status |
| 2 | ➕ Adicionar | Cria nova tarefa com ID automático e status `Pendente` |
| 3 | 🏁 Concluir | Marca uma tarefa como `Concluída` pelo seu ID |
| 4 | 💾 Persistência | Salva/carrega tarefas em arquivo binário automaticamente |

---

## 🗂️ Estrutura de Dados

O programa utiliza uma `struct` para representar cada tarefa:

```c
#define MAX_TITULO 60
#define NOME_ARQUIVO "tarefas.dat"

typedef struct {
    int id;                  // Identificador único
    char titulo[MAX_TITULO]; // Título da tarefa (máx. 60 caracteres)
    int concluida;           // 0 = Pendente | 1 = Concluída
} Tarefa;
```

---

## 🔧 Implementação

### 1. Carregar Tarefas
Ao iniciar, o programa lê o arquivo binário `tarefas.dat` e carrega as tarefas na memória com `fread`:

```c
void carregar_tarefas(Tarefa **vet, int *qtd) {
    FILE *f = fopen(NOME_ARQUIVO, "rb");
    if (!f) { *vet = NULL; *qtd = 0; return; }
    fread(qtd, sizeof(int), 1, f);
    *vet = (Tarefa *)malloc((*qtd) * sizeof(Tarefa));
    fread(*vet, sizeof(Tarefa), *qtd, f);
    fclose(f);
}
```

### 2. Adicionar Tarefa
Usa `realloc` para expandir o vetor dinamicamente a cada nova tarefa:

```c
void adicionar_tarefa(Tarefa **vet, int *qtd) {
    Tarefa nova;
    nova.id = (*qtd == 0) ? 1 : (*vet)[*qtd - 1].id + 1;
    nova.concluida = 0;
    printf("Título da tarefa: ");
    ler_linha(nova.titulo, MAX_TITULO);

    Tarefa *tmp = (Tarefa *)realloc(*vet, (*qtd + 1) * sizeof(Tarefa));
    *vet = tmp;
    (*vet)[*qtd] = nova;
    (*qtd)++;
}
```

### 3. Listar Tarefas
Percorre o vetor e exibe cada tarefa com ID, título e status:

```c
void listar_tarefas(Tarefa *vet, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s - %s\n",
               vet[i].id,
               vet[i].titulo,
               vet[i].concluida ? "Concluída" : "Pendente");
    }
}
```

### 4. Concluir Tarefa
Busca a tarefa pelo ID e marca o campo `concluida = 1`:

```c
void concluir_tarefa(Tarefa *vet, int qtd) {
    int id;
    printf("ID da tarefa a concluir: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < qtd; i++) {
        if (vet[i].id == id) {
            vet[i].concluida = 1;
            return;
        }
    }
}
```

---

## 🚀 Como Compilar e Executar

### Pré-requisitos

- [GCC](https://gcc.gnu.org/) instalado — verifique com `gcc --version`

### Passos

```bash
# 1. Clone o repositório
git clone https://github.com/IgorASB/desafio-github-markdown.git
cd desafio-github-markdown

# 2. Compile
gcc Gerenciador-de-tarefas.c -o gerenciador

# 3. Execute
./gerenciador
```

> **Windows (MinGW):** Após compilar, execute com `gerenciador.exe`.  
> **Linux/macOS:** Use `./gerenciador` diretamente no terminal.

---

## 🧠 Conceitos de C Aplicados

| Conceito | Aplicação no Projeto |
|---|---|
| `struct` | Modelagem da entidade `Tarefa` |
| `malloc` / `realloc` | Alocação dinâmica do vetor de tarefas |
| `free` | Liberação de memória ao encerrar |
| `fopen` / `fread` / `fwrite` | Persistência em arquivo binário |
| Ponteiros duplos (`**`) | Passagem e modificação do vetor por referência |
| `do...while` | Loop principal do menu |
| `switch...case` | Controle de fluxo do menu |

---

## 📁 Estrutura do Repositório

```
desafio-github-markdown/
├── Gerenciador-de-tarefas.c   # Código-fonte principal
├── tarefas.dat                # Arquivo de persistência (gerado em execução)
└── README.md                  # Documentação do projeto
```

---

## ✅ Checklist do Desafio de Markdown

- [x] Criar repositório no GitHub
- [x] Escrever README com Markdown
- [x] Usar títulos hierárquicos (`#`, `##`, `###`)
- [x] Inserir blocos de código com syntax highlight (` ```c `)
- [x] Criar tabelas
- [x] Usar listas com e sem marcadores
- [x] Usar emojis e ênfase (**negrito**, `código inline`)
- [x] Adicionar blocos de citação/aviso (`>`)
- [x] Usar badges com shields.io
- [x] Documentar um projeto real

---

## 📚 Referências

- [Guia de Markdown do GitHub](https://docs.github.com/pt/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)
- [Documentação da linguagem C — cppreference](https://en.cppreference.com/w/c)
- [Manual do GCC](https://gcc.gnu.org/onlinedocs/)
- [shields.io — Badges](https://shields.io/)

---

## 👤 Autor

Feito com 💙 por **Igor** — estudante de Ciência da Computação.

[![GitHub](https://img.shields.io/badge/GitHub-IgorASB-181717?style=flat&logo=github)](https://github.com/IgorASB)
[![Linguagem](https://img.shields.io/badge/Linguagem-C-00599C?style=flat&logo=c)](https://en.cppreference.com/w/c)
