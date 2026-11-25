#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NORMAL 0
#define PREFERENCIAL 1
#define URGENTE 2

typedef struct {
    char codigo[10];
    int categoria;
    int idade;
    char genero;
} Senha;

typedef struct No {
    Senha senha;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int qtd;
} Fila;

typedef struct {
    Fila urgente;
    Fila preferencial;
    Fila normal;
    int seq_u;
    int seq_p;
    int seq_n;
    int tem_atendimento;
    Senha atual;
} Clinica;

// Utilitarios de entrada

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_int(const char *msg, int min, int max) {
    int n;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &n) == 1) {
            limpar_buffer();
            if (n >= min && n <= max) {
                return n;
            }
            printf("Valor invalido. Digite um numero entre %d e %d.\n", min, max);
        } else {
            limpar_buffer();
            printf("Entrada invalida. Tente novamente.\n");
        }
    }
}

char ler_genero() {
    char g;
    while (1) {
        printf("Genero (M/F/O): ");
        if (scanf(" %c", &g) == 1) {
            limpar_buffer();
            g = toupper((unsigned char)g);
            if (g == 'M' || g == 'F' || g == 'O') {
                return g;
            }
        } else {
            limpar_buffer();
        }
        printf("Opcao invalida. Use M, F ou O.\n");
    }
}

int ler_sim_nao(const char *msg) {
    char r;
    while (1) {
        printf("%s (S/N): ", msg);
        if (scanf(" %c", &r) == 1) {
            limpar_buffer();
            r = toupper((unsigned char)r);
            if (r == 'S') return 1;
            if (r == 'N') return 0;
        } else {
            limpar_buffer();
        }
        printf("Digite S para sim ou N para nao.\n");
    }
}

const char* texto_categoria(int cat) {
    if (cat == URGENTE) return "Urgente";
    if (cat == PREFERENCIAL) return "Preferencial";
    return "Normal";
}

// Operacoes de fila (FIFO)

void iniciar_fila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->qtd = 0;
}

int enfileirar(Fila *f, Senha s) {
    No *novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 0;
    }

    novo->senha = s;
    novo->prox = NULL;

    if (f->inicio == NULL) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }

    f->fim = novo;
    f->qtd++;
    return 1;
}

int desenfileirar(Fila *f, Senha *out) {
    if (f->inicio == NULL) {
        return 0;
    }

    No *aux = f->inicio;
    *out = aux->senha;
    f->inicio = aux->prox;

    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    free(aux);
    f->qtd--;
    return 1;
}

void liberar_fila(Fila *f) {
    No *aux = f->inicio;
    while (aux != NULL) {
        No *prox = aux->prox;
        free(aux);
        aux = prox;
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->qtd = 0;
}

void iniciar_clinica(Clinica *c) {
    iniciar_fila(&c->urgente);
    iniciar_fila(&c->preferencial);
    iniciar_fila(&c->normal);
    c->seq_u = 0;
    c->seq_p = 0;
    c->seq_n = 0;
    c->tem_atendimento = 0;
}

void fechar_clinica(Clinica *c) {
    liberar_fila(&c->urgente);
    liberar_fila(&c->preferencial);
    liberar_fila(&c->normal);
}

// Cadastro e classificacao

int classificar(char genero, int idade, int gestante, int lactante, int nec_esp, int obeso) {
    if (idade > 80) {
        return URGENTE;
    }
    if (gestante || lactante || nec_esp || obeso) {
        return PREFERENCIAL;
    }
    if ((genero == 'F' && idade >= 60) || (genero == 'M' && idade >= 65)) {
        return PREFERENCIAL;
    }
    return NORMAL;
}

void gerar_codigo(Clinica *c, int cat, char *dest) {
    if (cat == URGENTE) {
        c->seq_u++;
        sprintf(dest, "U%03d", c->seq_u);
    } else if (cat == PREFERENCIAL) {
        c->seq_p++;
        sprintf(dest, "P%03d", c->seq_p);
    } else {
        c->seq_n++;
        sprintf(dest, "N%03d", c->seq_n);
    }
}

void cadastrar_paciente(Clinica *c) {
    printf("\n-- Novo Cadastro --\n");

    char genero = ler_genero();
    int idade = ler_int("Idade: ", 0, 130);

    int gestante = 0;
    int lactante = 0;
    if (genero != 'M') {
        gestante = ler_sim_nao("Gestante?");
        lactante = ler_sim_nao("Lactante?");
    }

    int nec_esp = ler_sim_nao("Necessidade especial?");
    int obeso = ler_sim_nao("Obesidade?");

    Senha nova;
    nova.genero = genero;
    nova.idade = idade;
    nova.categoria = classificar(genero, idade, gestante, lactante, nec_esp, obeso);
    gerar_codigo(c, nova.categoria, nova.codigo);

    int ok = 0;
    if (nova.categoria == URGENTE) {
        ok = enfileirar(&c->urgente, nova);
    } else if (nova.categoria == PREFERENCIAL) {
        ok = enfileirar(&c->preferencial, nova);
    } else {
        ok = enfileirar(&c->normal, nova);
    }

    if (ok) {
        printf("\nSenha gerada: %s (%s)\n", nova.codigo, texto_categoria(nova.categoria));
    }
}

// Atendimento

Senha* ver_proxima(Clinica *c) {
    if (c->urgente.inicio != NULL) return &c->urgente.inicio->senha;
    if (c->preferencial.inicio != NULL) return &c->preferencial.inicio->senha;
    if (c->normal.inicio != NULL) return &c->normal.inicio->senha;
    return NULL;
}

void consultar_proxima(Clinica *c) {
    Senha *s = ver_proxima(c);
    if (s == NULL) {
        printf("\nNao ha senhas na fila.\n");
    } else {
        printf("\nProxima senha: %s (%s)\n", s->codigo, texto_categoria(s->categoria));
    }
}

void chamar_paciente(Clinica *c) {
    if (c->tem_atendimento) {
        printf("\nJa existe um atendimento em andamento (%s - %s).\n",
               c->atual.codigo, texto_categoria(c->atual.categoria));
        printf("Finalize o atendimento atual antes de chamar outro paciente.\n");
        return;
    }

    Senha chamada;
    int achou = 0;

    if (desenfileirar(&c->urgente, &chamada)) {
        achou = 1;
    } else if (desenfileirar(&c->preferencial, &chamada)) {
        achou = 1;
    } else if (desenfileirar(&c->normal, &chamada)) {
        achou = 1;
    }

    if (!achou) {
        printf("\nNenhuma senha aguardando chamada.\n");
        return;
    }

    c->atual = chamada;
    c->tem_atendimento = 1;
    printf("\nChamando senha: %s (%s)\n", c->atual.codigo, texto_categoria(c->atual.categoria));
}

void finalizar_atendimento(Clinica *c) {
    if (!c->tem_atendimento) {
        printf("\nNenhum atendimento em andamento.\n");
        return;
    }

    printf("\nAtendimento da senha %s finalizado.\n", c->atual.codigo);
    c->tem_atendimento = 0;
}

// Painel

void imprimir_proximas(Fila *f, int *cont, int max) {
    No *aux = f->inicio;
    while (aux != NULL && *cont < max) {
        printf("%s ", aux->senha.codigo);
        (*cont)++;
        aux = aux->prox;
    }
}

void mostrar_painel(Clinica *c) {
    printf("\n----- PAINEL DE CHAMADA -----\n");

    if (c->tem_atendimento) {
        printf("Em atendimento: %s (%s)\n", c->atual.codigo, texto_categoria(c->atual.categoria));
    } else {
        printf("Em atendimento: Nenhum\n");
    }

    printf("Proximas senhas: ");
    int mostradas = 0;
    imprimir_proximas(&c->urgente, &mostradas, 5);
    imprimir_proximas(&c->preferencial, &mostradas, 5);
    imprimir_proximas(&c->normal, &mostradas, 5);
    if (mostradas == 0) {
        printf("Fila vazia");
    }
    printf("\n");

    printf("Em espera -> Urgente: %d | Preferencial: %d | Normal: %d\n",
           c->urgente.qtd, c->preferencial.qtd, c->normal.qtd);
    printf("-----------------------------\n");
}

void menu() {
    printf("\n1 - Cadastrar paciente\n");
    printf("2 - Consultar proxima senha\n");
    printf("3 - Chamar proximo paciente\n");
    printf("4 - Finalizar atendimento\n");
    printf("5 - Ver painel\n");
    printf("0 - Sair\n");
}

int main() {
    Clinica clinica;
    iniciar_clinica(&clinica);

    int op;
    do {
        menu();
        op = ler_int("Opcao: ", 0, 5);

        switch (op) {
            case 1:
                cadastrar_paciente(&clinica);
                break;
            case 2:
                consultar_proxima(&clinica);
                break;
            case 3:
                chamar_paciente(&clinica);
                break;
            case 4:
                finalizar_atendimento(&clinica);
                break;
            case 5:
                mostrar_painel(&clinica);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
        }
    } while (op != 0);

    fechar_clinica(&clinica);
    return 0;
}
