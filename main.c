#include <stdio.h>
#include "biblioteca.h"

int main() {
    Biblioteca *b = criarBiblioteca();
    cadastrarLivro(b);
    cadastrarLivro(b);
    cadastrarUsuario(b);

    consultarLivroID(b);
    consultarLivrosAutor(b);
    consultarUsuarioEmail(b);
    consultarUsuariosNome(b);
    consultarEmprestimos(b);
    return 0;
}