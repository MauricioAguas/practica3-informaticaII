#ifndef CODIFICADOR_H
#define CODIFICADOR_H

#include "utils.h"
#include "binario_utils.h"

/**
 * Métodos de codificación
 * Método 1: Inversión condicional según bloque anterior
 * Método 2: Desplazamiento circular derecha por bloque
 */
void codificarMetodo1(char* binario, int n);
void codificarMetodo2(char* binario, int n);

#endif
