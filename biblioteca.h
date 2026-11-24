#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
#define TAM_TEXTO 100
#define TAM_EMAIL 60
#define DISPONIVEL 0
#define EMPRESTADO 1
#define MAX_ID 999

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
    int qtdEmprestimos;
    struct Livro *prox;
} Livro;

typedef struct NoId {
    int id;
    Livro *livro;
    struct NoId *esq;
    struct NoId *dir;
} NoId;

typedef struct {
    Livro *livros;
    Usuario *usuarios;
    NoId *raizIds;
    int qtdLivros;
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
void emprestarLivro(Biblioteca *b);
void devolverLivro(Biblioteca *b);
int lerOpcao(const char *mensagem);
void liberarBiblioteca(Biblioteca *b);
NoId *inserirId(NoId *raiz, int id, Livro *livro);
int idExiste(const NoId *raiz, int id);
NoId *minimoId(NoId *no);
NoId *removerId(NoId *raiz, int id);
void liberarArvore(NoId *raiz);
NoId *buscarNoId(NoId *raiz, int id);
void exibirEstatisticas(const Biblioteca *b);
int contarEmprestimosAtivos(const Biblioteca *b, const char *email);
void livroMaisEmprestado(const Biblioteca *b);
void usuarioMaisEmprestimos(const Biblioteca *b);
int gerarIdUnico(const Biblioteca *b);
void listarEmprestimos(const Biblioteca *b);
#endif