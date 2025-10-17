#ifndef BINARIO_UTILS_H
#define BINARIO_UTILS_H

#include "utils.h"

/**
 * Conversión entre texto y binario
 */
void textoABinario(const char* texto, char* binario);
void binarioATexto(const char* binario, char* texto);

/**
 * Operaciones bit a bit
 */
void invertirBits(char* bits, int n);
void invertirCadaKBits(char* bits, int k, int n);
void desplazarCircularDerecha(char* bloque, int n);
void desplazarCircularIzquierda(char* bloque, int n);
int contarBits(const char* bits, int n, char bit);

#endif
