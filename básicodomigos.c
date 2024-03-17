#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

// Estrutura para representar um arquivo
typedef struct Arquivo {
    char nome[100];
    FILE *ponteiro;
} Arquivo;

// Função para abrir um arquivo
Arquivo* abrirArquivo(const char* nomeArquivo, const char* modo) {
    FILE *ponteiro = fopen(nomeArquivo, modo);
    if (ponteiro == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }
    Arquivo *arquivo = malloc(sizeof(Arquivo));
    if (arquivo == NULL) {
        fclose(ponteiro);
        perror("Erro ao alocar memória para o arquivo");
        return NULL;
    }
    arquivo->ponteiro = ponteiro;
    strcpy(arquivo->nome, nomeArquivo);
    return arquivo;
}

// Função para fechar um arquivo
void fecharArquivo(Arquivo *arquivo) {
    if (arquivo == NULL || arquivo->ponteiro == NULL) {
        printf("Arquivo inválido.\n");
        return;
    }
    fclose(arquivo->ponteiro);
    free(arquivo);
}

// Função para ler o conteúdo de um arquivo
void lerArquivo(Arquivo *arquivo) {
    if (arquivo == NULL || arquivo->ponteiro == NULL) {
        printf("Arquivo inválido.\n");
        return;
    }
    char caractere;
    printf("Conteúdo do arquivo %s:\n", arquivo->nome);
    rewind(arquivo->ponteiro); // Retorna ao início do arquivo
    while ((caractere = fgetc(arquivo->ponteiro)) != EOF) {
        putchar(caractere);
    }
    printf("\n");
}

// Função para escrever em um arquivo
void escreverArquivo(Arquivo *arquivo, const char *conteudo) {
    if (arquivo == NULL || arquivo->ponteiro == NULL) {
        printf("Arquivo inválido.\n");
        return;
    }
    fprintf(arquivo->ponteiro, "%s", conteudo);
}

// Função para excluir um arquivo
void excluirArquivo(const char* nomeArquivo) {
    if (remove(nomeArquivo) != 0) {
        perror("Erro ao excluir o arquivo");
    } else {
        printf("Arquivo %s excluído com sucesso.\n", nomeArquivo);
    }
}

// Função para renomear um arquivo
void renomearArquivo(const char* nomeAntigo, const char* nomeNovo) {
    if (rename(nomeAntigo, nomeNovo) != 0) {
        perror("Erro ao renomear o arquivo");
    } else {
        printf("Arquivo %s renomeado para %s com sucesso.\n", nomeAntigo, nomeNovo);
    }
}

// Função para copiar um arquivo
void copiarArquivo(const char* nomeOriginal, const char* novoNome) {
    FILE *original = fopen(nomeOriginal, "rb");
    if (original == NULL) {
        perror("Erro ao abrir o arquivo original para cópia");
        return;
    }
    FILE *copia = fopen(novoNome, "wb");
    if (copia == NULL) {
        perror("Erro ao criar o arquivo de cópia");
        fclose(original);
        return;
    }
    char caractere;
    while ((caractere = fgetc(original)) != EOF) {
        fputc(caractere, copia);
    }
    fclose(original);
    fclose(copia);
}

// Função para mover um arquivo
void moverArquivo(const char* nomeOriginal, const char* novoDiretorio) {
    char novoCaminho[200];
    snprintf(novoCaminho, sizeof(novoCaminho), "%s/%s", novoDiretorio, nomeOriginal);
    if (rename(nomeOriginal, novoCaminho) != 0) {
        perror("Erro ao mover o arquivo");
    } else {
        printf("Arquivo %s movido para %s com sucesso.\n", nomeOriginal, novoCaminho);
    }
}

// Função para criar um arquivo temporário
Arquivo* criarArquivoTemporario() {
    char nomeTemp[20] = "tempXXXXXX";
    int fd = mkstemp(nomeTemp);
    if (fd == -1) {
        perror("Erro ao criar arquivo temporário");
        return NULL;
    }
    close(fd);
    return abrirArquivo(nomeTemp, "w+");
}

// Função para obter o tamanho de um arquivo
long int obterTamanhoArquivo(Arquivo *arquivo) {
    if (arquivo == NULL || arquivo->ponteiro == NULL) {
        printf("Arquivo inválido.\n");
        return -1;
    }
    long int tamanho;
    fseek(arquivo->ponteiro, 0, SEEK_END); // Vai para o final do arquivo
    tamanho = ftell(arquivo->ponteiro); // Retorna a posição atual, que é o tamanho do arquivo
    rewind(arquivo->ponteiro); // Retorna ao início do arquivo
    return tamanho;
}

// Função para verificar se um arquivo existe
int arquivoExiste(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo) {
        fclose(arquivo);
        return 1;
    }
    return 0;
}

// Função para verificar se um diretório existe
int diretorioExiste(const char *diretorio) {
    if (opendir(diretorio)) {
        return 1;
    }
    return 0;
}

int main() {
    // Exemplo de uso das funções
    Arquivo *arquivo = abrirArquivo("exemplo.txt", "w+");
    if (arquivo != NULL) {
        escreverArquivo(arquivo, "Olá, mundo!");
        lerArquivo(arquivo);
        fecharArquivo(arquivo);
    } else {
        printf("Falha ao abrir o arquivo.\n");
    }
    return 0;
}

