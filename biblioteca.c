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

int lerInteiro(const char *mensagem) {
    char buffer[32];
    lerTexto(mensagem, buffer, sizeof(buffer));
    return atoi(buffer);
}

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

static void inserirLivroOrdenado(Biblioteca *b, Livro *novo) {
    /* início da lista: vazia ou novo vem antes do primeiro */
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
        printf("Título e autor não podem ficar vazios.\n");
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