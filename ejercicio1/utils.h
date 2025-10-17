#ifndef UTILS_H
#define UTILS_H

#define MAX_BUFFER 4096
#define MAX_SEMILLA 64

#include <cstdio>

/**
 * Funciones para lectura y escritura de archivos
 */
bool leerArchivoTexto(const char* nombre, char* buffer, int& len);
bool leerArchivoBinario(const char* nombre, unsigned char* buffer, int& len);
bool guardarArchivoTexto(const char* nombre, const char* contenido);
bool guardarArchivoBinario(const char* nombre, const unsigned char* datos, int len);
void mostrarError(const char* msg);

#endif
