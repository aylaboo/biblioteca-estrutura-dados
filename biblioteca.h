#define BIBLIOTECA_H

#include <time.h>

#define TAM_TEXTO 100
#define TAM_EMAIL 60

#define DISPONIVEL 0
#define EMPRESTADO 1

#define PRAZO_DIAS 7
#define SEGUNDOS_DIA 86400

typedef struct Usuario {
    char email[TAM_EMAIL];
    char nome[TAM_TEXTO];
    struct Usuario *prox;
} Usuario;

typedef struct Livro {
    int id;
    char titulo[TAM_TEXTO];
    char autor[TAM_TEXTO];
    int ano;
    int status;
    char emailEmprestimo[TAM_EMAIL];
    time_t dataEmprestimo;
    int qtdEmprestimos;
    struct Livro *prox;
} Livro;

typedef struct NoId {
    int id;
    struct NoId *esq;
    struct NoId *dir;
} NoId;

typedef struct {
    Livro *livros;
    Usuario *usuarios;
    NoId *raizIds;
    int proximoId;
    int diasSimulados;
} Biblioteca;

void lerTexto(const char *mensagem, char *destino, int tamanho);
int lerInteiro(const char *mensagem);
