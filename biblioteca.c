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