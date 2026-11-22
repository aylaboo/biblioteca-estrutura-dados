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
Biblioteca *criarBiblioteca(void);
void inserirLivroOrdenado(Biblioteca *b, Livro *novo);
void cadastrarLivro(Biblioteca *b);
void exibirLivro(const Livro *l);
void listarLivros(const Biblioteca *b);
Usuario *buscarUsuarioEmail(const Biblioteca *b, const char *email);
void cadastrarUsuario(Biblioteca *b);
void exibirUsuario(const Usuario *u);
void listarUsuarios(const Biblioteca *b);
Livro *buscarLivroID(const Biblioteca *b, int id);
void consultarLivroID(const Biblioteca *b);
void consultarLivrosAutor(const Biblioteca *b);
void consultarUsuarioEmail(const Biblioteca *b);
void consultarUsuariosNome(const Biblioteca *b);
void consultarEmprestimos(const Biblioteca *b);
void atualizarLivro(Biblioteca *b);
void atualizarUsuario(Biblioteca *b);
void excluirLivro(Biblioteca *b);
void excluirUsuario(Biblioteca *b);
int usuarioTemLivros(const Biblioteca *b, const char *email);