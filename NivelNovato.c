/*
 * territorios.c
 * Programa simples para cadastro de 5 territórios usando uma struct composta.
 * Requisitos:
 *  - struct Territorio com campos: char nome[30], char cor[10], int tropas.
 *  - Cadastro de 5 territórios via entrada do usuário.
 *  - Exibição imediata dos dados após cadastro.
 *
 * Observações sobre entrada:
 *  - Usamos scanf com scanset ("%29[^\n]" e "%9[^\n]") para permitir nomes/cores com espaços.
 *  - Validamos a leitura do número de tropas.
 */

#include <stdio.h>
#include <string.h>

#define MAX_TERRITORIOS 5
#define NOME_SIZE 30    /* inclui o caractere nulo '\0' */
#define COR_SIZE 10     /* inclui o caractere nulo '\0' */

/* Definição da struct Territorio:
 * Agrupa dados relacionados a um território: nome, cor do exército e número de tropas.
 */
typedef struct {
    char nome[NOME_SIZE];
    char cor[COR_SIZE];
    int tropas;
} Territorio;

int main(void) {
    Territorio territorios[MAX_TERRITORIOS]; /* vetor para armazenar 5 territórios */

    printf("=== Sistema de Cadastro de Territórios ===\n");
    printf("Você deverá cadastrar %d territórios.\n\n", MAX_TERRITORIOS);

    /* Laço de entrada: preenche cada elemento do vetor de structs */
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("---- Território %d de %d ----\n", i + 1, MAX_TERRITORIOS);

        /* Leitura do nome: usamos scanf com scanset para permitir espaços.
         * " %29[^\n]" -> lê até 29 caracteres ou até '\n' (o espaço inicial descarta '\n' residual).
         */
        printf("Digite o nome do território (máx %d caracteres): ", NOME_SIZE - 1);
        if (scanf(" %29[^\n]", territorios[i].nome) != 1) {
            /* Em caso improvável de erro, garantir string vazia */
            territorios[i].nome[0] = '\0';
        }

        /* Leitura da cor do exército (máx 9 caracteres) */
        printf("Digite a cor do exército (máx %d caracteres): ", COR_SIZE - 1);
        if (scanf(" %9[^\n]", territorios[i].cor) != 1) {
            territorios[i].cor[0] = '\0';
        }

        /* Leitura e validação do número de tropas */
        printf("Digite o número de tropas (inteiro): ");
        while (scanf("%d", &territorios[i].tropas) != 1) {
            /* Limpar entrada inválida até o fim da linha */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Entrada inválida. Digite um número inteiro para tropas: ");
        }

        printf("\n"); /* linha em branco para legibilidade entre cadastros */
    }

    /* Exibição imediata dos dados cadastrados */
    printf("\n=== Resumo dos Territórios Cadastrados ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("  Nome           : %s\n", territorios[i].nome);
        printf("  Cor do exército: %s\n", territorios[i].cor);
        printf("  Tropas         : %d\n", territorios[i].tropas);
    }

    printf("\nFim do programa. Obrigado!\n");
    return 0;
}