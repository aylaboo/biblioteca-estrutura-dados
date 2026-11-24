#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

//As três funções a seguir foram feitas com ajuda de LLM para deixar o código mais correto e menos bugado, corrigindo erros de buffer
//fgets para ler textos com espaço e strcspn para tirar o \n 
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
//função para usar nos menus, garante que as entradas sejam números inteiros e não letras
int lerOpcao(const char *mensagem) {
    char opt[32];
    char *fim;

    lerTexto(mensagem, opt, sizeof(opt));
    long valor = strtol(opt, &fim, 10);
    if (fim == opt || *fim != '\0') {
        return -1;
    }
    return (int)valor;
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
    b->qtdLivros = 0;
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
//Uso de LLM: para resolver o problema dos IDs sequenciais e a árvore binária ficar totalmente degenerada, tive a ideia de usar números aleatórios
//pedi ajuda da IA para realizar a implementação no código
int gerarIdUnico(const Biblioteca *b) {
    int id;
    do {
        id = rand() % MAX_ID + 1;
    } while (idExiste(b->raizIds, id));
    return id;
}

void cadastrarLivro(Biblioteca *b) {
    if (b->qtdLivros >= MAX_ID) {
        printf("Limite de %d livros atingido.\n", MAX_ID);
        return;
    }
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
    if (novo->ano <= 0) {
        printf("Ano inválido.\n");
        free(novo);
        return;
    }

    novo->id = gerarIdUnico(b);
    novo->status = DISPONIVEL;
    novo->emailEmprestimo[0] = '\0';
    novo->qtdEmprestimos = 0;
    novo->prox = NULL;
    //adiciona na árvore binária também, como se estivesse preenchendo duas estruturas ao mesmo tempo, a lista encadeada simples e a árvore somente com os IDs
    b->raizIds = inserirId(b->raizIds, novo->id, novo);
    b->qtdLivros++;
    inserirLivroOrdenado(b, novo);
    printf("Livro cadastrado com o código %d.\n", novo->id);
}

void exibirLivro(const Livro *l) {
    printf("[%03d] %s | %s | %d | %s", l->id, l->titulo, l->autor, l->ano,
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
    NoId *no = buscarNoId(b->raizIds, id);
    if (no == NULL) {
        return NULL;
    }
    return no->livro;
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

void listarEmprestimos(const Biblioteca *b) {
    int encontrados = 0;
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (l->status == EMPRESTADO) {
            exibirLivro(l);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("Nenhum livro emprestado no momento.\n");
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
//função auxiliar para a função de excluir usuários
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
    b->raizIds = removerId(b->raizIds, id);
    b->qtdLivros--;
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

void emprestarLivro(Biblioteca *b) {
    int id = lerInteiro("Código do livro a emprestar: ");
    Livro *l = buscarLivroID(b, id);
    if (l == NULL) {
        printf("Livro não encontrado.\n");
        return;
    }
    if (l->status == EMPRESTADO) {
        printf("O livro já está emprestado.\n");
        return;
    }

    char email[TAM_EMAIL];
    lerTexto("Email do usuário: ", email, TAM_EMAIL);
    if (buscarUsuarioEmail(b, email) == NULL) {
        printf("Usuário não cadastrado. Empréstimo cancelado.\n");
        return;
    }

    l->status = EMPRESTADO;
    strcpy(l->emailEmprestimo, email);
    l->qtdEmprestimos++;
    printf("Livro \"%s\" emprestado para %s.\n", l->titulo, email);
}

void devolverLivro(Biblioteca *b) {
    int id = lerInteiro("Código do livro a devolver: ");
    Livro *l = buscarLivroID(b, id);
    if (l == NULL) {
        printf("Livro não encontrado.\n");
        return;
    }
    if (l->status == DISPONIVEL) {
        printf("O livro não estava emprestado.\n");
        return;
    }

    l->status = DISPONIVEL;
    l->emailEmprestimo[0] = '\0';
    printf("Livro \"%s\" devolvido.\n", l->titulo);
}
//conta quantos livros um usuário tem emprestado naquele momento
int contarEmprestimosAtivos(const Biblioteca *b, const char *email) {
    int total = 0;
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (l->status == EMPRESTADO && strcmp(l->emailEmprestimo, email) == 0) {
            total++;
        }
    }
    return total;
}
//percorre todos os livros e salva aquele que foi emprestado mais vezes
void livroMaisEmprestado(const Biblioteca *b) {
    const Livro *maior = NULL;
    for (const Livro *l = b->livros; l != NULL; l = l->prox) {
        if (l->qtdEmprestimos > 0 &&
            (maior == NULL || l->qtdEmprestimos > maior->qtdEmprestimos)) {
            maior = l;
        }
    }

    if (maior == NULL) {
        printf("Livro mais emprestado: nenhum empréstimo foi realizado ainda.\n");
        return;
    }
    printf("Livro mais emprestado (%d vezes): ", maior->qtdEmprestimos);
    exibirLivro(maior);
}
//percorre todos os usuários e salva aquele que tem mais empréstimos no momento
void usuarioMaisEmprestimos(const Biblioteca *b) {
    const Usuario *melhor = NULL;
    int maior = 0;
    for (const Usuario *u = b->usuarios; u != NULL; u = u->prox) {
        int qtd = contarEmprestimosAtivos(b, u->email); //vê a quantidade de empréstimos de cada usuário com seu email no momento e guarda o maior
        if (qtd > maior) {
            maior = qtd;
            melhor = u;
        }
    }

    if (melhor == NULL) {
        printf("Usuário com mais empréstimos ativos: nenhum usuário com livros emprestados.\n");
        return;
    }
    printf("Usuário com mais empréstimos ativos (%d): ", maior);
    exibirUsuario(melhor);
}
//imprime as duas funções acima
void exibirEstatisticas(const Biblioteca *b) {
    printf("\n--- Estatísticas ---\n");
    livroMaisEmprestado(b);
    usuarioMaisEmprestimos(b);
}

void liberarBiblioteca(Biblioteca *b) {
    Livro *l = b->livros;
    while (l != NULL) {
        Livro *proximo = l->prox;
        free(l);
        l = proximo;
    }

    Usuario *u = b->usuarios;
    while (u != NULL) {
        Usuario *proximo = u->prox;
        free(u);
        u = proximo;
    }
    liberarArvore(b->raizIds);
    free(b);
}
//o extra da árvore binária foi implementada como uma BST comum, mas com o único propósito de guardar os IDs dos livros somente
NoId *inserirId(NoId *raiz, int id, Livro *livro) {
 if (raiz == NULL) {
        NoId *novo = malloc(sizeof(NoId));
        if (novo == NULL) {
            printf("Erro: sem memória.\n");
            return NULL;
        }
        novo->id = id;
        novo->livro = livro;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    if (id < raiz->id) {
        raiz->esq = inserirId(raiz->esq, id, livro);
    } else if (id > raiz->id) {
        raiz->dir = inserirId(raiz->dir, id, livro);
    }
    return raiz;
}

int idExiste(const NoId *raiz, int id) {
    if (raiz == NULL) {
        return 0;
    }
    if (id == raiz->id) {
        return 1;
    }
    if (id < raiz->id) {
        return idExiste(raiz->esq, id);
    }
    return idExiste(raiz->dir, id);
}
NoId *buscarNoId(NoId *raiz, int id) {
    while (raiz != NULL && raiz->id != id) {
        raiz = (id < raiz->id) ? raiz->esq : raiz->dir;
    }
    return raiz;
}

 NoId *minimoId(NoId *no) {
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

NoId *removerId(NoId *raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }
    if (id < raiz->id) {
        raiz->esq = removerId(raiz->esq, id);
    } else if (id > raiz->id) {
        raiz->dir = removerId(raiz->dir, id);
    } else {
        //só o filho da direita
        if (raiz->esq == NULL) {
            NoId *filho = raiz->dir;
            free(raiz);
            return filho;
        }
        //só o filho da esquerda
        if (raiz->dir == NULL) {
            NoId *filho = raiz->esq;
            free(raiz);
            return filho;
        }
        // dois filhos: troca pelo menor da subárvore direita
        NoId *sucessor = minimoId(raiz->dir);
        raiz->id = sucessor->id;
        raiz->livro = sucessor->livro;
        raiz->dir = removerId(raiz->dir, sucessor->id);
    }
    return raiz;
}

void liberarArvore(NoId *raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}