#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void lerTexto(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\n")] = '\0';
}

//essa função existem pois no scanf o enter fica no buffer, e uma próxima leitura de texto pega esse enter sozinho e pode dar erro
//a função recebe o inteiro e utiliza a função lerTexto e depois converte para inteiro novamente
int lerInteiro(const char *mensagem) {
    char inteiro[32];
    lerTexto(mensagem, inteiro, sizeof(inteiro));
    return atoi(inteiro);
}

//exit(1) invés de return 1 pois o exit encerra o programa inteiro imediatamente, e o return só sairia da função
Biblioteca *criarBiblioteca(void) {
    Biblioteca *b = malloc(sizeof(Biblioteca));
    if (b == NULL) {
        printf("Erro: sem memória.\n");
        exit(1);
    }
    b->livros = NULL;
    b->usuarios = NULL;
    b->raizIds = NULL;
    b->proximoId = 1;
    b->diasSimulados = 0;
    return b;
}

// Uso de LLM: A função é static porque só o biblioteca.c precisa dela, e o main nunca a chama.
// Assim ela fica fora do header. Como o ID é sequencial, o novo sempre cai no fim, e a função percorre a lista toda a cada cadastro
static void inserirLivroOrdenado(Biblioteca *b, Livro *novo) {
    // no início da lista: vazia ou novo livro vem antes do primeiro
    if (b->livros == NULL || b->livros->id > novo->id) {
        novo->prox = b->livros;
        b->livros = novo;
        return;
    }
    Livro *atual = b->livros;
    while (atual->prox != NULL && atual->prox->id < novo->id) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

void cadastrarLivro(Biblioteca *b) {
    Livro *novo = malloc(sizeof(Livro));
    if (novo == NULL) {
        printf("Erro: sem memória.\n");
        return;
    }

    lerTexto("Título: ", novo->titulo, TAM_TEXTO);
    lerTexto("Autor: ", novo->autor, TAM_TEXTO);
    if (novo->titulo[0] == '\0' || novo->autor[0] == '\0') {
        printf("Título e/ou autor não podem ficar vazios.\n");
        free(novo);
        return;
    }
    novo->ano = lerInteiro("Ano de publicação: ");

    novo->id = b->proximoId++;
    novo->status = DISPONIVEL;
    novo->emailEmprestimo[0] = '\0';
    novo->dataEmprestimo = 0;
    novo->qtdEmprestimos = 0;
    novo->prox = NULL;

    inserirLivroOrdenado(b, novo);
    printf("Livro cadastrado com o código %d.\n", novo->id);
}

void exibirLivro(const Livro *l) {
    printf("[%d] %s | %s | %d | %s", l->id, l->titulo, l->autor, l->ano,
           l->status == DISPONIVEL ? "Disponível" : "Emprestado");
    if (l->status == EMPRESTADO) {
        printf(" para %s", l->emailEmprestimo);
    }
    printf("\n");
}

void listarLivros(const Biblioteca *b) {
    if (b->livros == NULL) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        exibirLivro(l);
    }
}

Usuario *buscarUsuarioEmail(const Biblioteca *b, const char *email) {
    for (Usuario *u = b->usuarios; u != NULL; u = u->prox) {
        if (strcmp(u->email, email) == 0) {
            return u;
        }
    }
    return NULL;
}

void cadastrarUsuario(Biblioteca *b) {
    char email[TAM_EMAIL];
    lerTexto("Email: ", email, TAM_EMAIL);
// serve para saber se tem @ no email
    if (strchr(email, '@') == NULL) {
        printf("Email inválido.\n");
        return;
    }
    if (buscarUsuarioEmail(b, email) != NULL) {
        printf("Já existe um usuário cadastrado com esse email.\n");
        return;
    }

    Usuario *novo = malloc(sizeof(Usuario));
    if (novo == NULL) {
        printf("Erro: sem memória.\n");
        return;
    }
    strcpy(novo->email, email);
    lerTexto("Nome: ", novo->nome, TAM_TEXTO);
    if (novo->nome[0] == '\0') {
        printf("O nome não pode ficar vazio.\n");
        free(novo);
        return;
    }
    novo->prox = b->usuarios;
    b->usuarios = novo;
    printf("Usuário cadastrado.\n");
}

void exibirUsuario(const Usuario *u) {
    printf("%s | %s\n", u->email, u->nome);
}

void listarUsuarios(const Biblioteca *b) {
    if (b->usuarios == NULL) {
        printf("Nenhum usuário cadastrado.\n");
        return;
    }
    for (const Usuario *u = b->usuarios; u != NULL; u = u->prox) {
        exibirUsuario(u);
    }
}