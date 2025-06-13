#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_NOME 10
#define TAMANHO_EMAIL 10
#define TAMANHO_PESSOA (TAMANHO_NOME + sizeof(int) + TAMANHO_EMAIL)
#define TAMANHO_BUFFER (2 * sizeof(int) + MAX_PESSOAS * TAMANHO_PESSOA)
#define MAX_PESSOAS 100

static char* buffer = NULL;
static size_t offset = 0;

void inicializarBuffer() {
    buffer = (char*)malloc(TAMANHO_BUFFER);
    if (!buffer) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    memset(buffer, 0, TAMANHO_BUFFER);
    offset = 2 * sizeof(int);
}

int encontrarPessoa(const char* nome) {
    int* qtdPessoas = (int*)(buffer + sizeof(int));
    size_t currentOffset = 2 * sizeof(int);
    
    for (int i = 0; i < *qtdPessoas; i++) {
        if (strcmp(buffer + currentOffset, nome) == 0) {
            return i;
        }
        currentOffset += TAMANHO_PESSOA;
    }
    return -1;
}

void adicionarPessoa() {
    if (offset >= TAMANHO_BUFFER) {
        printf("Agenda cheia!\n");
        return;
    }

    int* qtdPessoas = (int*)(buffer + sizeof(int));
    
    if (*qtdPessoas >= MAX_PESSOAS) {
        printf("Agenda cheia!\n");
        return;
    }

    printf("Digite o nome (max %d caracteres): ", TAMANHO_NOME);
    scanf("%s", buffer + offset);
    offset += TAMANHO_NOME;

    printf("Digite a idade: ");
    scanf("%d", (int*)(buffer + offset));
    offset += sizeof(int);

    printf("Digite o email (max %d caracteres): ", TAMANHO_EMAIL);
    scanf("%s", buffer + offset);
    offset += TAMANHO_EMAIL;

    (*qtdPessoas)++;
    printf("Pessoa adicionada com sucesso!\n");
}

void removerPessoa() {
    char nome[TAMANHO_NOME];
    printf("Digite o nome da pessoa a ser removida: ");
    scanf("%s", nome);

    int index = encontrarPessoa(nome);
    if (index == -1) {
        printf("Pessoa nao encontrada!\n");
        return;
    }

    int* qtdPessoas = (int*)(buffer + sizeof(int));
    size_t posicaoRemover = 2 * sizeof(int) + (index * TAMANHO_PESSOA);
    size_t posicaoProxima = posicaoRemover + TAMANHO_PESSOA;
    size_t bytesParaMover = offset - posicaoProxima;

    if (bytesParaMover > 0) {
        memmove(buffer + posicaoRemover, buffer + posicaoProxima, bytesParaMover);
    }

    offset -= TAMANHO_PESSOA;
    (*qtdPessoas)--;
    printf("Pessoa removida com sucesso!\n");
}

void buscarPessoa() {
    char nome[TAMANHO_NOME];
    printf("Digite o nome da pessoa a ser buscada: ");
    scanf("%s", nome);

    int index = encontrarPessoa(nome);
    if (index == -1) {
        printf("Pessoa nao encontrada!\n");
        return;
    }

    size_t posicao = 2 * sizeof(int) + (index * TAMANHO_PESSOA);
    printf("\nDados da pessoa:\n");
    printf("Nome: %s\n", buffer + posicao);
    printf("Idade: %d\n", *(int*)(buffer + posicao + TAMANHO_NOME));
    printf("Email: %s\n", buffer + posicao + TAMANHO_NOME + sizeof(int));
}

void listarTodos() {
    int* qtdPessoas = (int*)(buffer + sizeof(int));
    
    if (*qtdPessoas == 0) {
        printf("Agenda vazia!\n");
        return;
    }

    printf("\nLista de Pessoas:\n");
    size_t currentOffset = 2 * sizeof(int);
    
    for (int i = 0; i < *qtdPessoas; i++) {
        printf("Pessoa %d:\n", i + 1);
        printf("Nome: %s\n", buffer + currentOffset);
        currentOffset += TAMANHO_NOME;
        
        printf("Idade: %d\n", *(int*)(buffer + currentOffset));
        currentOffset += sizeof(int);
        
        printf("Email: %s\n", buffer + currentOffset);
        currentOffset += TAMANHO_EMAIL;
        printf("-------------------\n");
    }
}

int main() {
    inicializarBuffer();
    int opcao;
    int* opcaoPtr = (int*)buffer;
    
    do {
        printf("\n=== Agenda ===\n");
        printf("1. Adicionar pessoa\n");
        printf("2. Remover pessoa\n");
        printf("3. Buscar pessoa\n");
        printf("4. Listar todos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", opcaoPtr);
        
        switch (*opcaoPtr) {
            case 1:
                adicionarPessoa();
                break;
            case 2:
                removerPessoa();
                break;
            case 3:
                buscarPessoa();
                break;
            case 4:
                listarTodos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (*opcaoPtr != 0);

    free(buffer);
    return 0;
} 