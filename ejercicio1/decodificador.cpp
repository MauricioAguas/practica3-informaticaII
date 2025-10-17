#include "decodificador.h"
#include "binario_utils.h"
#include <cstring>

void decodificarMetodo1(char* binario, int n) {
    int len = strlen(binario);
    char resultado[MAX_BUFFER] = {0};
    char bloqueAnterior[MAX_SEMILLA] = {0};

    strncpy(bloqueAnterior, binario, n);
    invertirBits(bloqueAnterior, n);
    strncpy(resultado, bloqueAnterior, n);

    int pos = n;
    while (pos < len) {
        char bloque[MAX_SEMILLA];
        strncpy(bloque, binario + pos, n);
        bloque[n] = '\0';

        int unos = contarBits(bloqueAnterior, n, '1');
        int ceros = contarBits(bloqueAnterior, n, '0');

        if (unos == ceros) {
            invertirBits(bloque, n);
        } else if (ceros > unos) {
            invertirCadaKBits(bloque, 2, n);
        } else {
            invertirCadaKBits(bloque, 3, n);
        }

        strncat(resultado, bloque, n);
        strncpy(bloqueAnterior, binario + pos, n);
        pos += n;
    }

    strcpy(binario, resultado);
}

void decodificarMetodo2(char* binario, int n) {
    int len = strlen(binario);
    char resultado[MAX_BUFFER] = {0};
    for (int i = 0; i < len; i += n) {
        char bloque[MAX_SEMILLA];
        strncpy(bloque, binario + i, n);
        bloque[n] = '\0';
        desplazarCircularIzquierda(bloque, n);
        strncat(resultado, bloque, n);
    }
    strcpy(binario, resultado);
}
