#include "binario_utils.h"
#include <cstring>

void textoABinario(const char* texto, char* binario) {
    int k = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char)texto[i];
        for (int j = 7; j >= 0; j--) {
            binario[k++] = (c & (1 << j)) ? '1' : '0';
        }
    }
    binario[k] = '\0';
}

void binarioATexto(const char* binario, char* texto) {
    int len = strlen(binario);
    int k = 0;
    for (int i = 0; i + 7 < len; i += 8) {
        unsigned char c = 0;
        for (int j = 0; j < 8; j++) {
            if (binario[i + j] == '1')
                c |= (1 << (7 - j));
        }
        texto[k++] = c;
    }
    texto[k] = '\0';
}

void invertirBits(char* bits, int n) {
    for (int i = 0; i < n; i++) {
        bits[i] = (bits[i] == '1') ? '0' : '1';
    }
}

void invertirCadaKBits(char* bits, int k, int n) {
    for (int i = k - 1; i < n; i += k) {
        bits[i] = (bits[i] == '1') ? '0' : '1';
    }
}

void desplazarCircularDerecha(char* bloque, int n) {
    char ultimo = bloque[n - 1];
    for (int i = n - 1; i > 0; i--)
        bloque[i] = bloque[i - 1];
    bloque[0] = ultimo;
}

void desplazarCircularIzquierda(char* bloque, int n) {
    char primero = bloque[0];
    for (int i = 0; i < n - 1; i++)
        bloque[i] = bloque[i + 1];
    bloque[n - 1] = primero;
}

int contarBits(const char* bits, int n, char bit) {
    int contador = 0;
    for (int i = 0; i < n; i++) {
        if (bits[i] == bit)
            contador++;
    }
    return contador;
}
