#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOME_SIZE 30
#define COR_SIZE 10
#define TOTAL_MISSOES 5
#define TOTAL_TERRITORIOS 5

typedef struct {
    char nome[NOME_SIZE];
    char cor[COR_SIZE];
    int tropas;
} Territorio;

const char* missoes[TOTAL_MISSOES] = {
    "Conquistar 3 territorios",
    "Eliminar todas as tropas inimigas",
    "Controlar todos os territorios com mais de 5 tropas",
    "Ter pelo menos 3 territorios com mais de 10 tropas",
    "Reduzir tropas de um inimigo para menos de 3 em qualquer territorio"
};

Territorio* cadastrarTerritorios(const int quantidade) {
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        exit(1);
    }

    char buffer[100];
    printf("=== Cadastro de %d Territorios ===\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Nome (ate %d chars): ", NOME_SIZE - 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) buffer[0] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(mapa[i].nome, buffer, NOME_SIZE);
        mapa[i].nome[NOME_SIZE - 1] = '\0';

        printf("Cor do exercito (ate %d chars): ", COR_SIZE - 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) buffer[0] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(mapa[i].cor, buffer, COR_SIZE);
        mapa[i].cor[COR_SIZE - 1] = '\0';

        printf("Numero de tropas (inteiro >= 0): ");
        while (1) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
            int n;
            if (sscanf(buffer, "%d", &n) == 1 && n >= 0) {
                mapa[i].tropas = n;
                break;
            }
            printf("Entrada invalida. Digite um inteiro >= 0: ");
        }
    }

    return mapa;
}

void exibirTerritorios(const Territorio* mapa, const int quantidade) {
    printf("\n=== Estado Atual dos Territorios ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome   : %s\n", mapa[i].nome);
        printf("  Cor    : %s\n", mapa[i].cor);
        printf("  Tropas : %d\n", mapa[i].tropas);
    }
}

void atribuirMissao(char** missaoDestino, const char* missoes[], int total) {
    int idx = rand() % total;
    size_t len = strlen(missoes[idx]);
    *missaoDestino = (char*) malloc((len + 1) * sizeof(char));
    if (*missaoDestino == NULL) {
        printf("Erro ao alocar memoria para missao.\n");
        exit(1);
    }
    strcpy(*missaoDestino, missoes[idx]);
}

void exibirMissao(const char* missao) {
    printf("\nSua missão: %s\n", missao);
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\nAtaque: %s (%s, %d tropas) -> %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: não é permitido atacar um território da mesma cor.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Erro: atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return;
    }
    if (defensor->tropas == 0) {
        printf("Erro: não é possível atacar um território vazio.\n");
        return;
    }

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;
    printf("Dado do atacante: %d | dado do defensor: %d\n", dadoA, dadoD);

    if (dadoA >= dadoD) {
        printf("Atacante vence!\n");
        int tropasTrans = atacante->tropas / 2;
        if (tropasTrans < 1) tropasTrans = 1;
        if (atacante->tropas - tropasTrans < 1)
            tropasTrans = atacante->tropas - 1;

        defensor->tropas = tropasTrans;
        strcpy(defensor->cor, atacante->cor);
        atacante->tropas -= tropasTrans;

        printf("%s foi conquistado. Agora pertence a %s com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
    } else {
        printf("Defensor resiste. Atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        printf("Tropas restantes do atacante: %d\n", atacante->tropas);
    }
}

int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador) {
    int i, cont;

    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        cont = 0;
        for (i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
        return (cont >= 3);
    }
    else if (strcmp(missao, "Eliminar todas as tropas inimigas") == 0) {
        for (i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) != 0 && mapa[i].tropas > 0) return 0;
        return 1;
    }
    else if (strcmp(missao, "Controlar todos os territorios com mais de 5 tropas") == 0) {
        for (i = 0; i < tamanho; i++)
            if (mapa[i].tropas > 5 && strcmp(mapa[i].cor, corJogador) != 0) return 0;
        return 1;
    }
    else if (strcmp(missao, "Ter pelo menos 3 territorios com mais de 10 tropas") == 0) {
        cont = 0;
        for (i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 10) cont++;
        return (cont >= 3);
    }
    else if (strcmp(missao, "Reduzir tropas de um inimigo para menos de 3 em qualquer territorio") == 0) {
        for (i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) != 0 && mapa[i].tropas < 3 && mapa[i].tropas > 0) return 1;
        return 0;
    }

    return 0;
}

int verificarVitoria(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador) {
    return verificarMissao(missao, mapa, tamanho, corJogador);
}

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

int main(void) {
    srand((unsigned) time(NULL));
    Territorio* mapa = cadastrarTerritorios(TOTAL_TERRITORIOS);

    printf("\nEscolha seu território inicial (1 a %d): ", TOTAL_TERRITORIOS);
    int idxJog;
    while (scanf("%d", &idxJog) != 1 || idxJog < 1 || idxJog > TOTAL_TERRITORIOS) {
        while (getchar() != '\n');
        printf("Entrada inválida. Digite entre 1 e %d: ", TOTAL_TERRITORIOS);
    }
    while (getchar() != '\n');
    idxJog -= 1;

    char corJogador[COR_SIZE];
    strcpy(corJogador, mapa[idxJog].cor);

    char* missaoJogador = NULL;
    atribuirMissao(&missaoJogador, missoes, TOTAL_MISSOES);
    exibirMissao(missaoJogador);

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        while (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida. Escolha 0, 1 ou 2: ");
        }
        while (getchar() != '\n');

        if (opcao == 1) {
            exibirTerritorios(mapa, TOTAL_TERRITORIOS);
            int a, d;
            printf("Território ATACANTE (1-%d): ", TOTAL_TERRITORIOS);
            scanf("%d", &a);
            while (getchar() != '\n');
            printf("Território DEFENSOR (1-%d): ", TOTAL_TERRITORIOS);
            scanf("%d", &d);
            while (getchar() != '\n');

            if (a < 1 || a > TOTAL_TERRITORIOS || d < 1 || d > TOTAL_TERRITORIOS || a == d) {
                printf("Índices inválidos!\n");
            } else {
                atacar(&mapa[a - 1], &mapa[d - 1]);
            }
        }
        else if (opcao == 2) {
            if (verificarVitoria(missaoJogador, mapa, TOTAL_TERRITORIOS, corJogador)) {
                printf("\n🎉 Você cumpriu sua missão: %s\n", missaoJogador);
                break;
            } else {
                printf("\nMissão ainda não cumprida.\n");
            }
        }
        else if (opcao == 0) {
            printf("\nSaindo do jogo...\n");
        }
        else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador);
    printf("\nFim do programa.\n");
    return 0;
}