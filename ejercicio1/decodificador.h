#ifndef DECODIFICADOR_H
#define DECODIFICADOR_H

#include "utils.h"
#include "binario_utils.h"

/**
 * Métodos de decodificación
 * Método 1: Inversión condicional inversa
 * Método 2: Desplazamiento circular izquierda por bloque
 */
void decodificarMetodo1(char* binario, int n);
void decodificarMetodo2(char* binario, int n);

#endif
