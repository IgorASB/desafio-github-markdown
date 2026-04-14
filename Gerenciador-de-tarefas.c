#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITULO 60
#define NOME_ARQUIVO "tarefas.dat"

typedef struct {
    int id;
    char titulo[MAX_TITULO];
    int concluida; // 0 = não, 1 = sim 
} Tarefa;

// Carrega tarefas salvas do arquivo (se existir)
void carregar_tarefas(Tarefa **vet, int *qtd) {
    FILE *f = fopen(NOME_ARQUIVO, "rb");
    if (!f) {
        *vet = NULL;
        *qtd = 0;
        return;
    }
    fread(qtd, sizeof(int), 1, f);
    if (*qtd <= 0) {
        *vet = NULL;
        fclose(f);
        return;
    }
    *vet = (Tarefa *)malloc((*qtd) * sizeof(Tarefa));
    if (!*vet) {
        printf("\nErro de memória ao carregar.\n");
        *qtd = 0;
        fclose(f);
        return;
    }
    fread(*vet, sizeof(Tarefa), *qtd, f);
    fclose(f);
}

// Salva tarefas atuais no arquivo
void salvar_tarefas(Tarefa *vet, int qtd) {
    FILE *f = fopen(NOME_ARQUIVO, "wb");
    if (!f) {
        printf("\nNão foi possível salvar as tarefas.\n");
        return;
    }
    fwrite(&qtd, sizeof(int), 1, f);
    if (qtd > 0) {
        fwrite(vet, sizeof(Tarefa), qtd, f);
    }
    fclose(f);
}

// Lê uma linha de texto do stdin removendo o '\n' 
void ler_linha(char *buf, int tam) {
    fgets(buf, tam, stdin);
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
}

// Adiciona nova tarefa usando realloc para crescer o vetor 
void adicionar_tarefa(Tarefa **vet, int *qtd) {
    Tarefa nova;
    nova.id = (*qtd == 0) ? 1 : (*vet)[*qtd - 1].id + 1;
    nova.concluida = 0;

    printf("Título da tarefa: ");
    ler_linha(nova.titulo, MAX_TITULO);

    Tarefa *tmp = (Tarefa *)realloc(*vet, (*qtd + 1) * sizeof(Tarefa));
    if (!tmp) {
        printf("Erro de memória ao adicionar.\n");
        return;
    }
    *vet = tmp;
    (*vet)[*qtd] = nova;
    (*qtd)++;
    printf("Tarefa adicionada com ID %d.\n", nova.id);
}

// Lista todas as tarefas com status 
void listar_tarefas(Tarefa *vet, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma tarefa cadastrada.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s - %s\n",
               vet[i].id,
               vet[i].titulo,
               vet[i].concluida ? "Concluída" : "Pendente");
    }
}

// Marca tarefa como concluída a partir do ID 
void concluir_tarefa(Tarefa *vet, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma tarefa para concluir.\n");
        return;
    }
    int id;
    printf("ID da tarefa a concluir: ");
    scanf("%d", &id);
    getchar(); // consome '\n'
    for (int i = 0; i < qtd; i++) {
        if (vet[i].id == id) {
            vet[i].concluida = 1;
            printf("\nTarefa %d marcada como concluída.\n", id);
            return;
        }
    }
    printf("Tarefa com ID %d não encontrada.\n", id);
}

int main(void) {
    Tarefa *tarefas = NULL;
    int qtd = 0;
    int opcao;

    // Carrega tarefas existentes ao iniciar
    carregar_tarefas(&tarefas, &qtd);

    do {
        printf("\n=== GERENCIADOR DE TAREFAS ===\n");
        printf("1 - Listar tarefas\n");
        printf("2 - Adicionar tarefa\n");
        printf("3 - Concluir tarefa\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        if (scanf("%d", &opcao) != 1) {
            // Tratamento simples para entrada inválida 
            printf("Entrada inválida.\n");
            // limpa stdin 
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
            continue;
        }
        getchar(); // consome '\n' restante 

        switch (opcao) {
            case 1:
                listar_tarefas(tarefas, qtd);
                break;
            case 2:
                adicionar_tarefa(&tarefas, &qtd);
                break;
            case 3:
                concluir_tarefa(tarefas, qtd);
                break;
            case 0:
                printf("Salvando e saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    // Salva antes de encerrar 
    salvar_tarefas(tarefas, qtd);
    free(tarefas);
    return 0;
}
