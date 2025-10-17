#include "codificador.h"
#include "binario_utils.h"
#include <cstring>

void codificarMetodo1(char* binario, int n) {
    int len = (int)strlen(binario);
    if (n <= 0 || n > MAX_SEMILLA-1) return;

    // Hacemos una copia del binario original y lo completamos con ceros para que su longitud sea múltiplo de n
    char original[MAX_BUFFER];
    strncpy(original, binario, sizeof(original));
    original[sizeof(original)-1] = 0;

    int orig_len = (int)strlen(original);
    int rem = orig_len % n;
    if (rem != 0) {
        int to_add = n - rem;
        for (int i = 0; i < to_add && (orig_len + i) < (int)sizeof(original)-1; ++i) {
            original[orig_len + i] = '0';
        }
        orig_len += (n - rem);
        original[orig_len] = '\0';
    }

    char resultado[MAX_BUFFER];
    resultado[0] = '\0';

    // Primer bloque: invertir todos los bits del primer bloque original
    char primer[MAX_SEMILLA];
    strncpy(primer, original, n);
    primer[n] = '\0';
    invertirBits(primer, n);
    strncat(resultado, primer, n);

    // Mantener prev_original como el bloque original (sin codificar) para tomar decisiones
    char prev_original[MAX_SEMILLA];
    strncpy(prev_original, original, n);
    prev_original[n] = '\0';

    // Procesar los bloques siguientes
    for (int pos = n; pos < orig_len; pos += n) {
        char cur_original[MAX_SEMILLA];
        strncpy(cur_original, original + pos, n);
        cur_original[n] = '\0';

        // crear bloque de salida basado en cur_original
        char bloque_codificado[MAX_SEMILLA];
        strncpy(bloque_codificado, cur_original, n);
        bloque_codificado[n] = '\0';

        int unos = contarBits(prev_original, n, '1');
        int ceros = contarBits(prev_original, n, '0');

        if (unos == ceros) {
            invertirBits(bloque_codificado, n);
        } else if (ceros > unos) {
            // invertir cada 2 bits: índices 1,3,5,... (0-based)
            for (int i = 1; i < n; i += 2)
                bloque_codificado[i] = (bloque_codificado[i] == '1') ? '0' : '1';
        } else { // unos > ceros
            // invertir cada 3 bits: índices 2,5,8,... (0-based)
            for (int i = 2; i < n; i += 3)
                bloque_codificado[i] = (bloque_codificado[i] == '1') ? '0' : '1';
        }

        strncat(resultado, bloque_codificado, n);

        // actualizar prev_original a cur_original (siempre la versión SIN codificar)
        strncpy(prev_original, cur_original, n);
        prev_original[n] = '\0';
    }

    // Copiar resultado (codificado) sobre binario de entrada
    strncpy(binario, resultado, MAX_BUFFER-1);
    binario[MAX_BUFFER-1] = '\0';
}


void codificarMetodo2(char* binario, int n) {
    int len = strlen(binario);
    int resto = len % n;
    if (resto != 0) {
        for (int i = 0; i < n - resto; i++)
            strcat(binario, "0");
        len = strlen(binario);
    }

    char resultado[MAX_BUFFER] = {0};
    for (int i = 0; i < len; i += n) {
        char bloque[MAX_SEMILLA];
        strncpy(bloque, binario + i, n);
        bloque[n] = '\0';
        desplazarCircularDerecha(bloque, n);
        strncat(resultado, bloque, n);
    }
    strcpy(binario, resultado);
}
