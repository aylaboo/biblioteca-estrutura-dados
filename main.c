#include <stdio.h>
#include "biblioteca.h"

int main() {
    Biblioteca *b = criarBiblioteca();
    cadastrarLivro(b);
    cadastrarLivro(b);
    cadastrarLivro(b);
    cadastrarUsuario(b);

    atualizarLivro(b);
    atualizarUsuario(b);
    listarLivros(b);
    listarUsuarios(b);

    excluirLivro(b);
    excluirUsuario(b);
    listarLivros(b);
    listarUsuarios(b);
    return 0;
}