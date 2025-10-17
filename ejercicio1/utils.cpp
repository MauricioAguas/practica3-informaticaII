#include "utils.h"
#include <cstdio>
#include <cstring>

bool leerArchivoTexto(const char* nombre, char* buffer, int& len) {
    FILE* f = fopen(nombre, "r");
    if (!f) return false;
    len = 0;
    while ((buffer[len] = fgetc(f)) != EOF && len < MAX_BUFFER - 1)
        len++;
    buffer[len] = '\0';
    fclose(f);
    return true;
}

bool leerArchivoBinario(const char* nombre, unsigned char* buffer, int& len) {
    FILE* f = fopen(nombre, "rb");
    if (!f) return false;
    len = fread(buffer, 1, MAX_BUFFER, f);
    fclose(f);
    return true;
}

bool guardarArchivoTexto(const char* nombre, const char* contenido) {
    FILE* f = fopen(nombre, "w");
    if (!f) return false;
    fwrite(contenido, 1, strlen(contenido), f);
    fclose(f);
    return true;
}

bool guardarArchivoBinario(const char* nombre, const unsigned char* datos, int len) {
    FILE* f = fopen(nombre, "wb");
    if (!f) return false;
    fwrite(datos, 1, len, f);
    fclose(f);
    return true;
}

void mostrarError(const char* msg) {
    printf("Error: %s\n", msg);
}
