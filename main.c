#include <stdio.h>
#include "biblioteca.h"
#include <stdlib.h>
#include <time.h>

void menuCadastro(Biblioteca *b) {
    int opcao;
    do {
        printf("\n--- Cadastro ---\n");
        printf("1. Livros\n2. Usuários\n0. Voltar\n");
        opcao = lerOpcao("Opção: ");
        switch (opcao) {
            case 1: cadastrarLivro(b); break;
            case 2: cadastrarUsuario(b); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

 void menuConsultaLivros(Biblioteca *b) {
    printf("\n1. Por código\n2. Por autor\n3. Todos os livros\n");
    int opcao = lerOpcao("Opção: ");
    if (opcao == 1) {
        consultarLivroID(b);
    } else if (opcao == 2) {
        consultarLivrosAutor(b);
    } else if (opcao == 3) {
        listarLivros(b);
    } else {
        printf("Opção inválida.\n");
    }
}

 void menuConsultaUsuarios(Biblioteca *b) {
   printf("\n1. Por email\n2. Por nome\n3. Todos os usuários\n");
    int opcao = lerOpcao("Opção: ");
    if (opcao == 1) {
        consultarUsuarioEmail(b);
    } else if (opcao == 2) {
        consultarUsuariosNome(b);
    } else if (opcao == 3) {
        listarUsuarios(b);
    } else {
        printf("Opção inválida.\n");
    }
}
void menuConsultaEmprestimos(Biblioteca *b) {
    printf("\n1. De um usuário\n2. Todos os empréstimos\n");
    int opcao = lerOpcao("Opção: ");
    if (opcao == 1) {
        consultarEmprestimos(b);
    } else if (opcao == 2) {
        listarEmprestimos(b);
    } else {
        printf("Opção inválida.\n");
    }
}


void menuConsulta(Biblioteca *b) {
    int opcao;
    do {
        printf("\n--- Consulta ---\n");
        printf("1. Livros\n2. Usuários\n3. Empréstimos\n0. Voltar\n");
        opcao = lerOpcao("Opção: ");
        switch (opcao) {
            case 1: menuConsultaLivros(b); break;
            case 2: menuConsultaUsuarios(b); break;
            case 3: menuConsultaEmprestimos(b); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void menuAtualizacao(Biblioteca *b) {
    int opcao;
    do {
        printf("\n--- Atualização ---\n");
        printf("1. Livros\n2. Usuários\n0. Voltar\n");
        opcao = lerOpcao("Opção: ");
        switch (opcao) {
            case 1: atualizarLivro(b); break;
            case 2: atualizarUsuario(b); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void menuExclusao(Biblioteca *b) {
    int opcao;
    do {
        printf("\n--- Exclusão ---\n");
        printf("1. Livros\n2. Usuários\n0. Voltar\n");
        opcao = lerOpcao("Opção: ");
        switch (opcao) {
            case 1: excluirLivro(b); break;
            case 2: excluirUsuario(b); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

int main(void) {
    srand((unsigned)time(NULL));
    Biblioteca *b = criarBiblioteca();
    int opcao;

    do {
        printf("\n=== Biblioteca ===\n");
        printf("1. Cadastro\n2. Consulta\n3. Atualização\n4. Exclusão\n");
        printf("5. Empréstimo\n6. Devolução\n7. Estatísticas\n0. Sair\n");
        opcao = lerOpcao("Opção: ");
        switch (opcao) {
            case 1: menuCadastro(b); break;
            case 2: menuConsulta(b); break;
            case 3: menuAtualizacao(b); break;
            case 4: menuExclusao(b); break;
            case 5: emprestarLivro(b); break;
            case 6: devolverLivro(b); break;
            case 7: exibirEstatisticas(b); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    liberarBiblioteca(b);
    printf("Programa encerrado.\n");
    return 0;
}