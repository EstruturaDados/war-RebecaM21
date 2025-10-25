#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOME_SIZE 30
#define COR_SIZE 10

/* Estrutura para armazenar informações de um território */
typedef struct {
    char nome[NOME_SIZE];
    char cor[COR_SIZE];
    int tropas;
} Territorio;

/* Função para cadastrar os territórios dinamicamente */
Territorio* cadastrarTerritorios(int quantidade) {
    Territorio* mapa = (Territorio*) malloc(quantidade * sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para os territórios.\n");
        exit(1);
    }

    printf("\n=== Cadastro de %d Territórios ===\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
        printf("\n---- Território %d ----\n", i + 1);

        printf("Nome (máx %d caracteres): ", NOME_SIZE - 1);
        scanf(" %29[^\n]", mapa[i].nome);

        printf("Cor do exército (máx %d caracteres): ", COR_SIZE - 1);
        scanf(" %9[^\n]", mapa[i].cor);

        printf("Número de tropas (inteiro): ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida. Digite um número inteiro: ");
        }
    }

    return mapa;
}

/* Função para exibir os territórios */
void exibirTerritorios(Territorio* mapa, int quantidade) {
    printf("\n=== Territórios Cadastrados ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("  Nome           : %s\n", mapa[i].nome);
        printf("  Cor do exército: %s\n", mapa[i].cor);
        printf("  Tropas         : %d\n", mapa[i].tropas);
    }
}

/* Função para simular ataque entre dois territórios */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\nAtaque de %s (%s) -> %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: não é permitido atacar um território da mesma cor.\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("O território atacante precisa de pelo menos 2 tropas para atacar.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1; // 1 a 6
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu a batalha!\n");

        // Transferir cor e metade das tropas do atacante
        int tropasTransferidas = atacante->tropas / 2;

        defensor->tropas = tropasTransferidas;
        strcpy(defensor->cor, atacante->cor);

        // Reduz o número de tropas do atacante
        atacante->tropas -= tropasTransferidas;

        printf("O território %s agora pertence a %s com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
    } else {
        printf("Defensor resistiu ao ataque.\n");
        atacante->tropas -= 1;
        printf("O território atacante perdeu 1 tropa. Tropas restantes: %d\n", atacante->tropas);
    }
}

/* Função para liberar a memória alocada */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

int main(void) {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int totalTerritorios;
    printf("Digite o número de territórios a cadastrar: ");
    while (scanf("%d", &totalTerritorios) != 1 || totalTerritorios <= 1) {
        while (getchar() != '\n');
        printf("Entrada inválida. Digite um número inteiro maior que 1: ");
    }

    Territorio* mapa = cadastrarTerritorios(totalTerritorios);

    int continuar = 1;
    while (continuar) {
        exibirTerritorios(mapa, totalTerritorios);

        int idxAtacante, idxDefensor;

        printf("\nDigite o número do território ATACANTE (1 a %d): ", totalTerritorios);
        scanf("%d", &idxAtacante);

        printf("Digite o número do território DEFENSOR (1 a %d): ", totalTerritorios);
        scanf("%d", &idxDefensor);

        if (idxAtacante < 1 || idxAtacante > totalTerritorios ||
            idxDefensor < 1 || idxDefensor > totalTerritorios ||
            idxAtacante == idxDefensor) {
            printf("Índices inválidos. Tente novamente.\n");
            continue;
        }

        atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);

        printf("\nDeseja realizar outro ataque? (1 - Sim, 0 - Não): ");
        scanf("%d", &continuar);
    }

    liberarMemoria(mapa);

    printf("\nFim do programa. Obrigado por jogar!\n");
    return 0;
}