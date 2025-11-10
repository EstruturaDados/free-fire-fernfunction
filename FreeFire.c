#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

// Dependência para a correção de locale para consoles cp-850 no windows
#ifdef _WIN32
#include <windows.h>
#endif

// Globais
#define CAPACIDADE 10
#define TAM_TEXTO 64

// Estruturas
typedef struct
{
    char nome[TAM_TEXTO];
    char tipo[TAM_TEXTO];
    int quantidade;
} Item;

// Estado
/**
 * Estrutura de dados para armazenar os itens na mochila. O vetor possui o tamanho limite de @see CAPACIDADE.
 */
static Item mochila[CAPACIDADE];
static int total_itens = 0;

/**
 * Imprime linhas em branco para "limpar" a tela do console.
 */
static void limpar_tela()
{
    for (int i = 0; i < 30; i++)
        puts("");
}

/**
 * Lê uma linha de texto do stdin, removendo o caractere de nova linha final se presente.
 */
static void ler_linha(char *destino, size_t tamanho)
{
    if (fgets(destino, (int)tamanho, stdin))
    {
        size_t n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n')
            destino[n - 1] = '\0';
    }
    else
    {
        if (tamanho > 0)
            destino[0] = '\0';
        clearerr(stdin);
    }
}

/**
 * Lê um valor inteiro do stdin.
 */
static int ler_inteiro()
{
    char buffer[64];
    ler_linha(buffer, sizeof buffer);
    char *fim = NULL;
    long v = strtol(buffer, &fim, 10);
    if (fim == buffer)
        return 0;
    if (v < INT_MIN)
        v = INT_MIN;
    if (v > INT_MAX)
        v = INT_MAX;
    return (int)v;
}

/**
 * Exibe o menu de opções.
 */
static void exibir_menu()
{
    printf("1. Adicionar item\n");
    printf("2. Remover item por nome\n");
    printf("3. Listar itens\n");
    printf("4. Buscar item por nome\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

/**
 * Insere um novo item na mochila.
 */
static void inserir_item()
{
    if (total_itens >= CAPACIDADE)
    {
        printf("Mochila cheia (capacidade %d).\n", CAPACIDADE);
        return;
    }
    Item novo;
    printf("Nome do item: ");
    ler_linha(novo.nome, sizeof novo.nome);
    printf("Tipo do item: ");
    ler_linha(novo.tipo, sizeof novo.tipo);
    printf("Quantidade: ");
    novo.quantidade = ler_inteiro();
    if (novo.quantidade < 0)
        novo.quantidade = 0;
    mochila[total_itens++] = novo;
    printf("Item adicionado.\n");
}

/**
 * Remove um item da mochila pelo nome.
 */
static void remover_item()
{
    if (total_itens == 0)
    {
        printf("Mochila vazia.\n");
        return;
    }
    char alvo[TAM_TEXTO];
    printf("Nome do item a remover: ");
    ler_linha(alvo, sizeof alvo);
    int indice = -1;
    for (int i = 0; i < total_itens; i++)
    {
        if (strcmp(mochila[i].nome, alvo) == 0)
        {
            indice = i;
            break;
        }
    }
    if (indice == -1)
    {
        printf("Item nao encontrado.\n");
        return;
    }
    for (int i = indice; i < total_itens - 1; i++)
    {
        mochila[i] = mochila[i + 1];
    }
    total_itens--;
    printf("Item removido.\n");
}

/**
 * Lista todos os itens na mochila.
 */
static void listar_itens()
{
    if (total_itens == 0)
    {
        printf("Mochila vazia.\n");
        return;
    }
    printf("%-30s %-20s %10s\n", "Nome", "Tipo", "Quantidade");
    for (int i = 0; i < total_itens; i++)
    {
        printf("%-30s %-20s %10d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

/**
 * Busca um item na mochila pelo nome e exibe seus detalhes.
 */
static void buscar_item_por_nome()
{
    if (total_itens == 0)
    {
        printf("Mochila vazia.\n");
        return;
    }
    char alvo[TAM_TEXTO];
    printf("Nome do item para buscar: ");
    ler_linha(alvo, sizeof alvo);
    bool encontrado = false;
    for (int i = 0; i < total_itens; i++)
    {
        if (strcmp(mochila[i].nome, alvo) == 0)
        {
            printf("Item encontrado:\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            encontrado = true;
            break;
        }
    }
    if (!encontrado)
    {
        printf("Item nao encontrado.\n");
    }
}

/**
 * Função principal do programa. Contém o loop principal do menu.
 */
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Limpeza inicial para fixar o menu na aprte inferior
    limpar_tela();

    int opcao;
    do
    {
        exibir_menu();
        opcao = ler_inteiro();
        switch (opcao)
        {
        case 1:
            inserir_item();
            break;
        case 2:
            remover_item();
            break;
        case 3:
            listar_itens();
            break;
        case 4:
            buscar_item_por_nome();
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }
        if (opcao != 0)
        {
            printf("\nPressione ENTER para continuar...");
            char pausa[8];
            ler_linha(pausa, sizeof pausa);
            limpar_tela();
        }
    } while (opcao != 0);
    return 0;
}