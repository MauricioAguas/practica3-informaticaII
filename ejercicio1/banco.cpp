#include "banco.h"
#include "utils.h"
#include "codificador.h"
#include "decodificador.h"
#include "binario_utils.h"
#include <cstdio>
#include <cstring>

void inicializarSistema() {
    int semilla = 4;
    char adminClave[64];
    printf("Ingrese la clave de administrador: ");
    scanf("%63s", adminClave);

    char binario[MAX_BUFFER], codificado[MAX_BUFFER];
    textoABinario(adminClave, binario);
    strcpy(codificado, binario);
    codificarMetodo2(codificado, semilla);
    guardarArchivoTexto("sudo.txt", codificado);

    menuPrincipal(semilla);
}

void menuPrincipal(int semilla) {
    int opcion = 0;
    while (1) {
        printf("\n--- SISTEMA BANCARIO ---\n");
        printf("1. Administrador\n2. Usuario\n3. Salir\nOpción: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            if (validarAdministrador(semilla))
                menuAdministrador(semilla);
        } else if (opcion == 2) {
            menuUsuario(semilla);
        } else if (opcion == 3) {
            printf("Gracias por usar el sistema.\n");
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }
}

bool validarAdministrador(int semilla) {
    char claveGuardada[MAX_BUFFER];
    int len = 0;
    if (!leerArchivoTexto("sudo.txt", claveGuardada, len)) {
        mostrarError("No se encontró sudo.txt");
        return false;
    }
    decodificarMetodo2(claveGuardada, semilla);
    char texto[64];
    binarioATexto(claveGuardada, texto);

    char claveIngresada[64];
    printf("Ingrese clave admin: ");
    scanf("%63s", claveIngresada);
    return strcmp(texto, claveIngresada) == 0;
}

void menuAdministrador(int semilla) {
    int op = 0;
    while (1) {
        printf("\n--- MENÚ ADMINISTRADOR ---\n1. Crear usuario\n2. Salir\nOpción: ");
        scanf("%d", &op);
        if (op == 1)
            registrarUsuario(semilla);
        else if (op == 2)
            break;
        else
            printf("Opción inválida.\n");
    }
}

void registrarUsuario(int semilla) {
    FILE* f = fopen("usuarios.txt", "a");
    if (!f) {
        mostrarError("No se puede abrir usuarios.txt");
        return;
    }
    char cedula[32], clave[32];
    int saldo;
    printf("Ingrese cédula: ");
    scanf("%31s", cedula);
    printf("Ingrese clave: ");
    scanf("%31s", clave);
    printf("Ingrese saldo inicial: ");
    scanf("%d", &saldo);

    char binario[MAX_BUFFER];
    textoABinario(cedula, binario);
    codificarMetodo2(binario, semilla);
    fprintf(f, "%s\n", binario);

    textoABinario(clave, binario);
    codificarMetodo2(binario, semilla);
    fprintf(f, "%s\n", binario);

    char saldoStr[32];
    sprintf(saldoStr, "%d", saldo);
    textoABinario(saldoStr, binario);
    codificarMetodo2(binario, semilla);
    fprintf(f, "%s\n", binario);

    fclose(f);
    printf("Usuario creado exitosamente.\n");
}

void menuUsuario(int semilla) {
    char cedula[32];
    printf("Ingrese su cédula: ");
    scanf("%31s", cedula);
    // Aquí puedes implementar la validación de usuario y las operaciones:
    // consultarSaldo, retirarDinero, etc.
    printf("(Función de usuario aún en desarrollo)\n");
}
