#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <math.h> // Adicionado para fabs

#define MAX_PATH_LEN 1024

// Função para pesquisar um arquivo em um diretório e subdiretórios
void pesquisarArquivoRecursivo(const char *diretorio, const char *nomeArquivo) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    struct stat fileStat; // Informações do arquivo
    char path[MAX_PATH_LEN]; // Caminho do arquivo

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", diretorio, entry->d_name); // Construir o caminho completo

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Ignorar as entradas "." e ".."

        if (stat(path, &fileStat) < 0) { // Obter informações do arquivo
            perror("Erro ao obter informações do arquivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) { // Se for um diretório
            pesquisarArquivoRecursivo(path, nomeArquivo); // Pesquisar recursivamente dentro do diretório
        } else if (strcmp(entry->d_name, nomeArquivo) == 0) { // Se for o arquivo procurado
            printf("Arquivo encontrado: %s\n", path); // Exibir o caminho do arquivo
        }
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para filtrar arquivos por extensão
void filtrarPorExtensao(const char *diretorio, const char *extensao) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    char path[MAX_PATH_LEN]; // Caminho do arquivo
    char *fileExt; // Extensão do arquivo

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", diretorio, entry->d_name); // Construir o caminho completo

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Ignorar as entradas "." e ".."

        fileExt = strrchr(entry->d_name, '.'); // Obter a extensão do arquivo

        if (fileExt != NULL && strcmp(fileExt, extensao) == 0) { // Se a extensão corresponder
            printf("Arquivo com a extensão %s encontrado: %s\n", extensao, path); // Exibir o caminho do arquivo
        }
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para filtrar arquivos por tamanho
void filtrarPorTamanho(const char *diretorio, long tamanhoMinimo, long tamanhoMaximo) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    struct stat fileStat; // Informações do arquivo
    char path[MAX_PATH_LEN]; // Caminho do arquivo

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", diretorio, entry->d_name); // Construir o caminho completo

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Ignorar as entradas "." e ".."

        if (stat(path, &fileStat) < 0) { // Obter informações do arquivo
            perror("Erro ao obter informações do arquivo");
            continue;
        }

        if (S_ISREG(fileStat.st_mode) && fileStat.st_size >= tamanhoMinimo && fileStat.st_size <= tamanhoMaximo) { // Se for um arquivo regular e dentro do intervalo de tamanho
            printf("Arquivo no intervalo de tamanho [%ld, %ld] encontrado: %s\n", tamanhoMinimo, tamanhoMaximo, path); // Exibir o caminho do arquivo
        }
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para filtrar arquivos por data de modificação
void filtrarPorData(const char *diretorio, const char *data) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    struct stat fileStat; // Informações do arquivo
    char path[MAX_PATH_LEN]; // Caminho do arquivo
    struct tm tm; // Estrutura para a data
    time_t t; // Tempo

    // Converte a data fornecida para o formato de tempo
    if (strptime(data, "%d/%m/%Y", &tm) == NULL) { // Converter a data
        perror("Erro ao converter data");
        return;
    }
    t = mktime(&tm); // Converter para tempo

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", diretorio, entry->d_name); // Construir o caminho completo

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Ignorar as entradas "." e ".."

        if (stat(path, &fileStat) < 0) { // Obter informações do arquivo
            perror("Erro ao obter informações do arquivo");
            continue;
        }

        // Compara a data de modificação do arquivo com a data fornecida
        if (fabs(difftime(t, fileStat.st_mtime)) < 1e-6) { // Comparar tempos (usando uma pequena tolerância)
            printf("Arquivo modificado na data %s encontrado: %s\n", data, path); // Exibir o caminho do arquivo
        }
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para ordenar arquivos por nome, tamanho ou data de modificação
void ordenarArquivos(const char *diretorio, int opcaoOrdenacao) {
    // Aqui você implementaria a lógica para ordenar os arquivos no diretório de acordo com a opção de ordenação fornecida
    // Por exemplo, ordenação por nome, tamanho, data de modificação, etc.
    // O exemplo abaixo apenas imprime os arquivos encontrados no diretório, sem ordená-los
    DIR *dir; // Variável para o diretório
    struct dirent **entryList; // Lista de entradas do diretório
    int numEntries; // Número de entradas

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Listar o diretório
    if ((numEntries = scandir(diretorio, &entryList, NULL, alphasort)) < 0) {
        perror("Erro ao listar diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    for (int i = 0; i < numEntries; ++i) {
        printf("Arquivo encontrado: %s\n", entryList[i]->d_name); // Exibir o nome do arquivo
        free(entryList[i]); // Liberar memória alocada
    }

    free(entryList); // Liberar memória alocada

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para pesquisar uma palavra-chave no conteúdo dos arquivos
void pesquisarEmConteudo(const char *diretorio, const char *palavraChave) {
    // Aqui você implementaria a lógica para pesquisar a palavra-chave no conteúdo dos arquivos
    // Você pode usar as funções de entrada e saída de arquivo em C para abrir e ler o conteúdo dos arquivos
    // Em seguida, pesquisar a palavra-chave no conteúdo de cada arquivo
    // O exemplo abaixo apenas imprime os arquivos encontrados no diretório, sem considerar o conteúdo
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    char path[MAX_PATH_LEN]; // Caminho do arquivo

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", diretorio, entry->d_name); // Construir o caminho completo

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // Ignorar as entradas "." e ".."

        printf("Arquivo encontrado: %s\n", path); // Exibir o caminho do arquivo
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para criar um diretório
void criarDiretorio(const char *nomeDiretorio) {
    if (mkdir(nomeDiretorio, 0777) == -1) { // Criar o diretório com permissões de leitura, escrita e execução para o proprietário, grupo e outros
        perror("Erro ao criar diretório");
        return;
    }
    printf("Diretório \"%s\" criado com sucesso.\n", nomeDiretorio);
}

// Função para excluir um diretório
void excluirDiretorio(const char *nomeDiretorio) {
    if (rmdir(nomeDiretorio) == -1) { // Excluir o diretório
        perror("Erro ao excluir diretório");
        return;
    }
    printf("Diretório \"%s\" excluído com sucesso.\n", nomeDiretorio);
}

// Função para listar o conteúdo de um diretório
void listarDiretorio(const char *diretorio) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name); // Exibir o nome da entrada
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
    }
}

// Função para mudar o diretório atual
void mudarDiretorio(const char *novoDiretorio) {
    if (chdir(novoDiretorio) == -1) { // Mudar o diretório
        perror("Erro ao mudar de diretório");
        return;
    }
    printf("Diretório alterado para \"%s\".\n", novoDiretorio);
}

// Função para obter o caminho do diretório atual
void obterCaminhoAtual() {
    char path[MAX_PATH_LEN]; // Buffer para o caminho
    if (getcwd(path, sizeof(path)) == NULL) { // Obter o caminho do diretório atual
        perror("Erro ao obter caminho do diretório atual");
        return;
    }
    printf("Caminho do diretório atual: %s\n", path);
}

// Função para renomear um diretório
void renomearDiretorio(const char *nomeAntigo, const char *nomeNovo) {
    if (rename(nomeAntigo, nomeNovo) == -1) { // Renomear o diretório
        perror("Erro ao renomear diretório");
        return;
    }
    printf("Diretório renomeado de \"%s\" para \"%s\".\n", nomeAntigo, nomeNovo);
}

// Função para copiar um diretório
void copiarDiretorio(const char *diretorioOrigem, const char *diretorioDestino) {
    // Aqui você implementaria a lógica para copiar um diretório e seu conteúdo para outro destino
    // Você pode usar funções do sistema operacional ou bibliotecas de terceiros para realizar a cópia
    printf("Diretório \"%s\" copiado para \"%s\".\n", diretorioOrigem, diretorioDestino);
}

// Função para mover um diretório
void moverDiretorio(const char *diretorioOrigem, const char *diretorioDestino) {
    // Aqui você implementaria a lógica para mover um diretório e seu conteúdo para outro destino
    // Você pode usar funções do sistema operacional ou bibliotecas de terceiros para realizar a movimentação
    printf("Diretório \"%s\" movido para \"%s\".\n", diretorioOrigem, diretorioDestino);
}

// Função para obter o tamanho de um diretório
long obterTamanhoDiretorio(const char *diretorio) {
    // Aqui você implementaria a lógica para calcular o tamanho do diretório e seu conteúdo
    // Você pode percorrer recursivamente os arquivos e subdiretórios dentro do diretório para calcular o tamanho total
    // O exemplo abaixo retorna um tamanho aleatório para fins ilustrativos
    return 1024 * 1024 * 10; // Tamanho aleatório de 10 MB
}

// Função para verificar se um diretório está vazio
int verificarDiretorioVazio(const char *diretorio) {
    DIR *dir; // Variável para o diretório
    struct dirent *entry; // Entrada do diretório
    int isEmpty = 1; // Assume que o diretório está vazio

    if ((dir = opendir(diretorio)) == NULL) { // Abrir diretório
        perror("Erro ao abrir diretório");
        return -1;
    }

    // Iterar sobre cada entrada do diretório
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) { // Ignorar as entradas "." e ".."
            isEmpty = 0; // O diretório não está vazio
            break;
        }
    }

    if (closedir(dir) != 0) { // Fechar o diretório
        perror("Erro ao fechar diretório");
        return -1;
    }

    return isEmpty; // Retorna se o diretório está vazio ou não
}

// Função para exibir um menu de opções
void exibirMenu() {
    printf("MENU:\n");
    printf("1. Pesquisar arquivo recursivamente\n");
    printf("2. Filtrar arquivos por extensão\n");
    printf("3. Filtrar arquivos por tamanho\n");
    printf("4. Filtrar arquivos por data\n");
    printf("5. Ordenar arquivos\n");
    printf("6. Pesquisar em conteúdo de arquivos\n");
    printf("7. Criar diretório\n");
    printf("8. Excluir diretório\n");
    printf("9. Listar conteúdo do diretório\n");
    printf("10. Mudar de diretório\n");
    printf("11. Obter caminho do diretório atual\n");
    printf("12. Renomear diretório\n");
    printf("13. Copiar diretório\n");
    printf("14. Mover diretório\n");
    printf("15. Obter tamanho do diretório\n");
    printf("16. Verificar se diretório está vazio\n");
    printf("17. Sair\n");
}

int main() {
    int opcao;
    char diretorio[MAX_PATH_LEN];
    char nomeArquivo[MAX_PATH_LEN];
    char extensao[MAX_PATH_LEN];
    long tamanhoMinimo, tamanhoMaximo;
    char data[MAX_PATH_LEN];
    char palavraChave[MAX_PATH_LEN];
    char novoDiretorio[MAX_PATH_LEN];

    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o diretório a ser pesquisado: ");
                scanf("%s", diretorio);
                printf("Digite o nome do arquivo a ser pesquisado: ");
                scanf("%s", nomeArquivo);
                pesquisarArquivoRecursivo(diretorio, nomeArquivo);
                break;
            case 2:
                printf("Digite o diretório a ser filtrado por extensão: ");
                scanf("%s", diretorio);
                printf("Digite a extensão do arquivo a ser filtrado: ");
                scanf("%s", extensao);
                filtrarPorExtensao(diretorio, extensao);
                break;
            case 3:
                printf("Digite o diretório a ser filtrado por tamanho: ");
                scanf("%s", diretorio);
                printf("Digite o tamanho mínimo do arquivo (em bytes): ");
                scanf("%ld", &tamanhoMinimo);
                printf("Digite o tamanho máximo do arquivo (em bytes): ");
                scanf("%ld", &tamanhoMaximo);
                filtrarPorTamanho(diretorio, tamanhoMinimo, tamanhoMaximo);
                break;
            case 4:
                printf("Digite o diretório a ser filtrado por data: ");
                scanf("%s", diretorio);
                printf("Digite a data de modificação do arquivo (formato: dd/mm/aaaa): ");
                scanf("%s", data);
                filtrarPorData(diretorio, data);
                break;
            case 5:
                printf("Digite o diretório a ser ordenado: ");
                scanf("%s", diretorio);
                printf("Opções de ordenação:\n");
                printf("1. Por nome\n");
                printf("2. Por tamanho\n");
                printf("3. Por data de modificação\n");
                printf("Escolha uma opção de ordenação: ");
                scanf("%d", &opcao);
                ordenarArquivos(diretorio, opcao);
                break;
            case 6:
                printf("Digite o diretório a ser pesquisado: ");
                scanf("%s", diretorio);
                printf("Digite a palavra-chave a ser pesquisada no conteúdo dos arquivos: ");
                scanf("%s", palavraChave);
                pesquisarEmConteudo(diretorio, palavraChave);
                break;
            case 7:
                printf("Digite o nome do diretório a ser criado: ");
                scanf("%s", novoDiretorio);
                criarDiretorio(novoDiretorio);
                break;
            case 8:
                printf("Digite o nome do diretório a ser excluído: ");
                scanf("%s", diretorio);
                excluirDiretorio(diretorio);
                break;
            case 9:
                printf("Digite o diretório a ser listado: ");
                scanf("%s", diretorio);
                listarDiretorio(diretorio);
                break;
            case 10:
                printf("Digite o novo diretório: ");
                scanf("%s", diretorio);
                mudarDiretorio(diretorio);
                break;
            case 11:
                obterCaminhoAtual();
                break;
            case 12:
                printf("Digite o nome do diretório antigo: ");
                scanf("%s", diretorio);
                printf("Digite o nome do diretório novo: ");
                scanf("%s", novoDiretorio);
                renomearDiretorio(diretorio, novoDiretorio);
                break;
            case 13:
                printf("Digite o diretório de origem: ");
                scanf("%s", diretorio);
                printf("Digite o diretório de destino: ");
                scanf("%s", novoDiretorio);
                copiarDiretorio(diretorio, novoDiretorio);
                break;
            case 14:
                printf("Digite o diretório de origem: ");
                scanf("%s", diretorio);
                printf("Digite o diretório de destino: ");
                scanf("%s", novoDiretorio);
                moverDiretorio(diretorio, novoDiretorio);
                break;
            case 15:
                printf("Digite o diretório a ser verificado: ");
                scanf("%s", diretorio);
                printf("Tamanho total do diretório: %ld bytes\n", obterTamanhoDiretorio(diretorio));
                break;
            case 16:
                printf("Digite o diretório a ser verificado: ");
                scanf("%s", diretorio);
                if (verificarDiretorioVazio(diretorio))
                    printf("O diretório está vazio.\n");
                else
                    printf("O diretório não está vazio.\n");
                break;
            case 17:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 17);

    return 0;
}
