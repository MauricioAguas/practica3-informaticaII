#pragma once
#include <stdio.h>

/* Consume todo hasta fin de linea o EOF para evitar bucles de entrada */
static inline void limpiar_buffer(void){
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
}
