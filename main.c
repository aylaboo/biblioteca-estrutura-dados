#include <stdio.h>
#include "biblioteca.h"

int main() {
    Biblioteca *b = criarBiblioteca();
    // cadastrarLivro(b);
    // cadastrarLivro(b);
    // listarLivros(b);
    cadastrarUsuario(b);
    cadastrarUsuario(b);
    listarUsuarios(b);
    return 0;
}