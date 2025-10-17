#include "decodificador.h"
#include "binario_utils.h"
#include <cstring>

void decodificarMetodo1(char* binario, int n) {
    int len = (int)strlen(binario);
    if (n <= 0 || n > MAX_SEMILLA-1) return;

    // Hacemos copia del codificado y completamos a múltiplo de n por si fue rellenado en codificación
    char cod[MAX_BUFFER];
    strncpy(cod, binario, sizeof(cod));
    cod[sizeof(cod)-1] = 0;
    int cod_len = (int)strlen(cod);
    int rem = cod_len % n;
    if (rem != 0) {
        int to_add = n - rem;
        for (int i = 0; i < to_add && (cod_len + i) < (int)sizeof(cod)-1; ++i)
            cod[cod_len + i] = '0';
        cod_len += (n - rem);
        cod[cod_len] = '\0';
    }

    char resultado[MAX_BUFFER];
    resultado[0] = '\0';

    // Decodificar primer bloque: invertir todos los bits (es su propia inversa)
    char primer_cod[MAX_SEMILLA];
    strncpy(primer_cod, cod, n);
    primer_cod[n] = '\0';
    invertirBits(primer_cod, n); // invertir de nuevo para recuperar original
    strncat(resultado, primer_cod, n);

    // prev_original guardará el bloque ORIGINAL que vamos reconstruyendo (ya decodificado)
    char prev_original[MAX_SEMILLA];
    strncpy(prev_original, primer_cod, n);
    prev_original[n] = '\0';

    // Procesar bloques siguientes: misma regla pero usando prev_original (el original ya decodificado)
    for (int pos = n; pos < cod_len; pos += n) {
        char cur_cod[MAX_SEMILLA];
        strncpy(cur_cod, cod + pos, n);
        cur_cod[n] = '\0';

        // Para decodificar, aplicamos las mismas inversiones sobre cur_cod
        // porque la inversión es reversible aplicándola de nuevo (XOR con 1).
        char bloque_decodificado[MAX_SEMILLA];
        strncpy(bloque_decodificado, cur_cod, n);
        bloque_decodificado[n] = '\0';

        int unos = contarBits(prev_original, n, '1');
        int ceros = contarBits(prev_original, n, '0');

        if (unos == ceros) {
            invertirBits(bloque_decodificado, n);
        } else if (ceros > unos) {
            for (int i = 1; i < n; i += 2)
                bloque_decodificado[i] = (bloque_decodificado[i] == '1') ? '0' : '1';
        } else {
            for (int i = 2; i < n; i += 3)
                bloque_decodificado[i] = (bloque_decodificado[i] == '1') ? '0' : '1';
        }

        strncat(resultado, bloque_decodificado, n);

        // actualizar prev_original al bloque decodificado (que ahora es el original)
        strncpy(prev_original, bloque_decodificado, n);
        prev_original[n] = '\0';
    }

    // Copiar resultado (decodificado) sobre binario de entrada
    strncpy(binario, resultado, MAX_BUFFER-1);
    binario[MAX_BUFFER-1] = '\0';
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
