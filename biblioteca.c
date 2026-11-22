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

void inserirLivroOrdenado(Biblioteca *b, Livro *novo) {
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

Livro *buscarLivroID(const Biblioteca *b, int id) {
    for (Livro *l = b->livros; l != NULL && l->id <= id; l = l->prox) {
        if (l->id == id) {
            return l;
        }
    }
    return NULL;
}

void consultarLivroID(const Biblioteca *b) {
    int id = lerInteiro("Código do livro: ");
    Livro *l = buscarLivroID(b, id);
    if (l == NULL) {
        printf("Livro não encontrado.\n");
        return;
    }
    exibirLivro(l);
}

void consultarLivrosAutor(const Biblioteca *b) {
    char autor[TAM_TEXTO];
    lerTexto("Autor: ", autor, TAM_TEXTO);

    int encontrados = 0;
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (strcmp(l->autor, autor) == 0) {
            exibirLivro(l);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("Livro não encontrado.\n");
    }
}

void consultarUsuarioEmail(const Biblioteca *b) {
    char email[TAM_EMAIL];
    lerTexto("Email: ", email, TAM_EMAIL);

    Usuario *u = buscarUsuarioEmail(b, email);
    if (u == NULL) {
        printf("Usuário não cadastrado.\n");
        return;
    }
    exibirUsuario(u);
}

void consultarUsuariosNome(const Biblioteca *b) {
    char nome[TAM_TEXTO];
    lerTexto("Nome: ", nome, TAM_TEXTO);

    int encontrados = 0;
    for (const Usuario *u = b->usuarios; u != NULL; u = u->prox) {
        if (strcmp(u->nome, nome) == 0) {
            exibirUsuario(u);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("Usuário não cadastrado.\n");
    }
}

void consultarEmprestimos(const Biblioteca *b) {
    char email[TAM_EMAIL];
    lerTexto("Email do usuário: ", email, TAM_EMAIL);

    if (buscarUsuarioEmail(b, email) == NULL) {
        printf("Usuário não cadastrado.\n");
        return;
    }

    int encontrados = 0;
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (l->status == EMPRESTADO && strcmp(l->emailEmprestimo, email) == 0) {
            exibirLivro(l);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("O usuário não possui livros emprestados.\n");
    }
}

void atualizarLivro(Biblioteca *b) {
    int id = lerInteiro("Código do livro a alterar: ");
    Livro *l = buscarLivroID(b, id);
    if (l == NULL) {
        printf("Livro não encontrado.\n");
        return;
    }

    printf("Dados atuais: ");
    exibirLivro(l);
    printf("Deixe em branco para manter o valor atual.\n");

    char texto[TAM_TEXTO];
// a lógica aqui é que se o usuário digitar enter, vai dar uma string vazia, e só vai atualizar a informação se a string tiver algum dado
    lerTexto("Novo título: ", texto, TAM_TEXTO);
    if (texto[0] != '\0') {
        strcpy(l->titulo, texto);
    }

    lerTexto("Novo autor: ", texto, TAM_TEXTO);
    if (texto[0] != '\0') {
        strcpy(l->autor, texto);
    }
//uso de LLM: O ano é lido como texto e convertido com atoi, em vez de usar a função de lerInteiro.
//o lerInteiro devolveria 0 para "em branco" e para caracteres diferentes de números, e não daria para distinguir "manter" de "inválido".
    lerTexto("Novo ano: ", texto, TAM_TEXTO);
    if (texto[0] != '\0') {
        int ano = atoi(texto);
        if (ano > 0) {
            l->ano = ano;
        } else {
            printf("Ano inválido, o valor anterior foi mantido.\n");
        }
    }

    printf("Livro atualizado.\n");
}

void atualizarUsuario(Biblioteca *b) {
    char email[TAM_EMAIL];
    lerTexto("Email do usuário a alterar: ", email, TAM_EMAIL);

    Usuario *u = buscarUsuarioEmail(b, email);
    if (u == NULL) {
        printf("Usuário não cadastrado.\n");
        return;
    }

    char nome[TAM_TEXTO];
    lerTexto("Novo nome: ", nome, TAM_TEXTO);
    if (nome[0] == '\0') {
        printf("O nome não pode ficar vazio. Nada foi alterado.\n");
        return;
    }
    strcpy(u->nome, nome);
    printf("Usuário atualizado.\n");
}

int usuarioTemLivros(const Biblioteca *b, const char *email) {
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (l->status == EMPRESTADO && strcmp(l->emailEmprestimo, email) == 0) {
            return 1;
        }
    }
    return 0;
}

void excluirLivro(Biblioteca *b) {
    int id = lerInteiro("Código do livro a excluir: ");

    Livro *anterior = NULL;
    Livro *atual = b->livros;
    while (atual != NULL && atual->id < id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL || atual->id != id) {
        printf("Livro não encontrado. Nada foi excluído.\n");
        return;
    }
    if (atual->status == EMPRESTADO) {
        printf("O livro está emprestado e não pode ser excluído.\n");
        return;
    }

    // primeiro da lista
    if (anterior == NULL) {
        b->livros = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Livro excluído.\n");
}
// lógica parecida com excluirLivro
void excluirUsuario(Biblioteca *b) {
    char email[TAM_EMAIL];
    lerTexto("Email do usuário a excluir: ", email, TAM_EMAIL);

    Usuario *anterior = NULL;
    Usuario *atual = b->usuarios;
    while (atual != NULL && strcmp(atual->email, email) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Usuário não cadastrado. Nada foi excluído.\n");
        return;
    }
    // saber primeiro se o usuário possui um livro cadastrado para poder ser excluído
    if (usuarioTemLivros(b, email)) {
        printf("O usuário possui livros emprestados e não pode ser excluído.\n");
        return;
    }

    if (anterior == NULL) {
        b->usuarios = atual->prox;
    } else {     
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Usuário excluído.\n");
}