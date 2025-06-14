#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_NOME 10
#define TAMANHO_EMAIL 10
#define TAMANHO_PESSOA (TAMANHO_NOME + sizeof(int) + TAMANHO_EMAIL)
#define TAMANHO_INICIAL (3 * sizeof(int) + TAMANHO_PESSOA)
#define MAX_PESSOAS 100

void* pBuffer;

void inicializarBuffer() {
    pBuffer = malloc(TAMANHO_INICIAL);
    if (!pBuffer) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    memset(pBuffer, 0, TAMANHO_INICIAL);
    *(int*)((char*)pBuffer + sizeof(int)) = 0;
}

void redimensionarBuffer(int aumentar) {
    void* novoBuffer = realloc(pBuffer, 
        aumentar ? 
        *(int*)((char*)pBuffer + 2 * sizeof(int)) + TAMANHO_PESSOA :
        *(int*)((char*)pBuffer + 2 * sizeof(int)) - TAMANHO_PESSOA);
    
    if (!novoBuffer) {
        printf("Erro ao redimensionar memoria\n");
        return;
    }
    
    pBuffer = novoBuffer;
    printf("Buffer redimensionado para %d bytes\n", *(int*)((char*)pBuffer + 2 * sizeof(int)));
}

int encontrarPessoa() {
    char* current = (char*)pBuffer + 3 * sizeof(int);
    
    for (*(int*)pBuffer = 0; *(int*)pBuffer < *(int*)((char*)pBuffer + sizeof(int)); (*(int*)pBuffer)++) {
        if (strcmp(current, (char*)pBuffer + TAMANHO_INICIAL) == 0) {
            return *(int*)pBuffer;
        }
        current += TAMANHO_PESSOA;
    }
    return -1;
}

void adicionarPessoa() {
    if (*(int*)((char*)pBuffer + sizeof(int)) >= MAX_PESSOAS) {
        printf("Limite máximo de pessoas atingido!\n");
        return;
    }

    if (*(int*)((char*)pBuffer + 2 * sizeof(int)) + TAMANHO_PESSOA > *(int*)((char*)pBuffer + 2 * sizeof(int))) {
        redimensionarBuffer(1);
    }

    printf("Digite o nome (max %d caracteres): ", TAMANHO_NOME);
    scanf("%s", (char*)pBuffer + *(int*)((char*)pBuffer + 2 * sizeof(int)));
    *(int*)((char*)pBuffer + 2 * sizeof(int)) += TAMANHO_NOME;

    printf("Digite a idade: ");
    scanf("%d", (int*)((char*)pBuffer + *(int*)((char*)pBuffer + 2 * sizeof(int))));
    *(int*)((char*)pBuffer + 2 * sizeof(int)) += sizeof(int);

    printf("Digite o email (max %d caracteres): ", TAMANHO_EMAIL);
    scanf("%s", (char*)pBuffer + *(int*)((char*)pBuffer + 2 * sizeof(int)));
    *(int*)((char*)pBuffer + 2 * sizeof(int)) += TAMANHO_EMAIL;

    (*(int*)((char*)pBuffer + sizeof(int)))++;
    printf("Pessoa adicionada com sucesso!\n");
}

void removerPessoa() {
    printf("Digite o nome da pessoa a ser removida: ");
    scanf("%s", (char*)pBuffer + TAMANHO_INICIAL);

    *(int*)pBuffer = encontrarPessoa();
    if (*(int*)pBuffer == -1) {
        printf("Pessoa nao encontrada!\n");
        return;
    }

    char* posicaoRemover = (char*)pBuffer + 3 * sizeof(int) + (*(int*)pBuffer * TAMANHO_PESSOA);
    char* posicaoProxima = posicaoRemover + TAMANHO_PESSOA;
    
    if (*(int*)((char*)pBuffer + 2 * sizeof(int)) - (posicaoProxima - (char*)pBuffer) > 0) {
        memmove(posicaoRemover, posicaoProxima, *(int*)((char*)pBuffer + 2 * sizeof(int)) - (posicaoProxima - (char*)pBuffer));
    }

    *(int*)((char*)pBuffer + 2 * sizeof(int)) -= TAMANHO_PESSOA;
    (*(int*)((char*)pBuffer + sizeof(int)))--;
    
    redimensionarBuffer(0);
    printf("Pessoa removida com sucesso!\n");
}

void buscarPessoa() {
    printf("Digite o nome da pessoa a ser buscada: ");
    scanf("%s", (char*)pBuffer + TAMANHO_INICIAL);

    *(int*)pBuffer = encontrarPessoa();
    if (*(int*)pBuffer == -1) {
        printf("Pessoa nao encontrada!\n");
        return;
    }

    char* posicao = (char*)pBuffer + 3 * sizeof(int) + (*(int*)pBuffer * TAMANHO_PESSOA);
    printf("\nDados da pessoa:\n");
    printf("Nome: %s\n", posicao);
    printf("Idade: %d\n", *(int*)(posicao + TAMANHO_NOME));
    printf("Email: %s\n", posicao + TAMANHO_NOME + sizeof(int));
}

void listarTodos() {
    if (*(int*)((char*)pBuffer + sizeof(int)) == 0) {
        printf("Agenda vazia!\n");
        return;
    }

    printf("\nLista de Pessoas:\n");
    char* current = (char*)pBuffer + 3 * sizeof(int);
    
    for (*(int*)pBuffer = 0; *(int*)pBuffer < *(int*)((char*)pBuffer + sizeof(int)); (*(int*)pBuffer)++) {
        printf("Pessoa %d:\n", *(int*)pBuffer + 1);
        printf("Nome: %s\n", current);
        current += TAMANHO_NOME;
        
        printf("Idade: %d\n", *(int*)current);
        current += sizeof(int);
        
        printf("Email: %s\n", current);
        current += TAMANHO_EMAIL;
        printf("-------------------\n");
    }
}

int main() {
    inicializarBuffer();
    
    do {
        printf("\n=== Agenda ===\n");
        printf("1. Adicionar pessoa\n");
        printf("2. Remover pessoa\n");
        printf("3. Buscar pessoa\n");
        printf("4. Listar todos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", (int*)pBuffer);
        
        switch (*(int*)pBuffer) {
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
    } while (*(int*)pBuffer != 0);

    free(pBuffer);
    return 0;
}