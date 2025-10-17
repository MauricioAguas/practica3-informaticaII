#include "banco.h"
#include "utils.h"
#include "codificador.h"
#include "decodificador.h"
#include "binario_utils.h"
#include "input_utils.h"
#include <cstdio>
#include <cstring>
#include <ctype.h>

/* Constantes */
#define COSTO_OPERACION 1000

/* Helpers internos */
static int leer_opcion_menu(int* dst){
    printf("Opcion: ");
    if (scanf("%d", dst) != 1){
        printf("Entrada invalida. Intente de nuevo.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();
    return 1;
}

static int cargar_linea(FILE* f, char* dst, int max){
    /* lee una linea hasta \n y la deja en dst sin \n */
    int c, i=0;
    while ((c=fgetc(f))!=EOF && c!='\n'){
        if (i < max-1) dst[i++] = (char)c;
    }
    dst[i]=0; return (i>0 || c=='\n');
}

/* Busca un usuario por cedula (texto) en usuarios.txt. 
   Retorna 1 si existe y llena indices de linea base del registro (cedula/clave/saldo). */
static int buscar_usuario(const char* cedula, int* linea_base){
    FILE* f = fopen("usuarios.txt", "r");
    if (!f) return 0;
    char linea[4096];
    int idx = 0; int found = 0;    
    while (cargar_linea(f, linea, sizeof(linea))){
        if ((idx % 3)==0){ /* linea de cedula codificada */
            char bin[MAX_BUFFER]; strncpy(bin, linea, sizeof(bin));
            decodificarMetodo2(bin, 4);
            char txt[256]; binarioATexto(bin, txt);
            if (strcmp(txt, cedula)==0){
                *linea_base = idx; found = 1; break;
            }
        }
        idx++;
    }
    fclose(f);
    return found;
}

static int leer_registro_usuario(int base, char* out_ced, char* out_clave, int* out_saldo){
    FILE* f = fopen("usuarios.txt", "r"); if(!f) return 0;
    char linea[4096]; int idx=0; int got=0;
    while (cargar_linea(f, linea, sizeof(linea))){
        if (idx==base){
            char bin[MAX_BUFFER]; strncpy(bin, linea, sizeof(bin));
            decodificarMetodo2(bin, 4); binarioATexto(bin, out_ced);
            got++;
        } else if (idx==base+1){
            char bin[MAX_BUFFER]; strncpy(bin, linea, sizeof(bin));
            decodificarMetodo2(bin, 4); binarioATexto(bin, out_clave);
            got++;
        } else if (idx==base+2){
            char bin[MAX_BUFFER]; strncpy(bin, linea, sizeof(bin));
            decodificarMetodo2(bin, 4); char txt[64]; binarioATexto(bin, txt);
            *out_saldo = atoi(txt); got++;
            break;
        }
        idx++;
    }
    fclose(f);
    return got==3;
}

static int escribir_registro_saldo(int base, int nuevo_saldo){
    /* reescribe solo la linea base+2 (saldo) manteniendo el resto */
    FILE* in = fopen("usuarios.txt", "r"); if(!in) return 0;
    FILE* out = fopen("usuarios.tmp", "w"); if(!out){ fclose(in); return 0; }
    char linea[4096]; int idx=0;
    while (cargar_linea(in, linea, sizeof(linea))){
        if (idx==base+2){
            char txt[64]; sprintf(txt, "%d", nuevo_saldo);
            char bin[MAX_BUFFER]; textoABinario(txt, bin); codificarMetodo2(bin, 4);
            fprintf(out, "%s\n", bin);
        } else {
            fprintf(out, "%s\n", linea);
        }
        idx++;
    }
    fclose(in); fclose(out);
    remove("usuarios.txt"); rename("usuarios.tmp", "usuarios.txt");
    return 1;
}

void inicializarSistema() {
    int semilla = 4; /* TODO: persistir META n */
    char adminClave[64];
    printf("Ingrese la clave de administrador: ");
    if (scanf("%63s", adminClave) != 1){ limpiar_buffer(); return; }
    limpiar_buffer();

    char binario[MAX_BUFFER], codificado[MAX_BUFFER];
    textoABinario(adminClave, binario);
    strcpy(codificado, binario);
    codificarMetodo2(codificado, semilla);
    guardarArchivoTexto("sudo.txt", codificado);

    menuPrincipal(semilla);
}

void menuPrincipal(int semilla) {
    int opcion = 0;
    for (;;) {
        printf("\n--- SISTEMA BANCARIO ---\n");
        printf("1. Administrador\n2. Usuario\n3. Salir\n");
        if (!leer_opcion_menu(&opcion)) continue;

        if (opcion == 1) {
            if (validarAdministrador(semilla))
                menuAdministrador(semilla);
        } else if (opcion == 2) {
            menuUsuario(semilla);
        } else if (opcion == 3) {
            printf("Gracias por usar el sistema.\n");
            break;
        } else {
            printf("Opcion invalida.\n");
        }
    }
}

bool validarAdministrador(int semilla) {
    char claveGuardada[MAX_BUFFER];
    int len = 0;
    if (!leerArchivoTexto("sudo.txt", claveGuardada, len)) {
        mostrarError("No se encontro sudo.txt");
        return false;
    }
    decodificarMetodo2(claveGuardada, semilla);
    char texto[64];
    binarioATexto(claveGuardada, texto);

    char claveIngresada[64];
    printf("Ingrese clave admin: ");
    if (scanf("%63s", claveIngresada) != 1){ limpiar_buffer(); return false; }
    limpiar_buffer();

    if (strcmp(texto, claveIngresada) != 0){
        printf("clave errada, intente de nuevo\n");
        return false;
    }
    return true;
}

void menuAdministrador(int semilla) {
    int op = 0;
    for (;;) {
        printf("\n--- MENU ADMINISTRADOR ---\n1. Crear usuario\n2. Salir\n");
        if (!leer_opcion_menu(&op)) continue;
        if (op == 1)
            registrarUsuario(semilla);
        else if (op == 2)
            break;
        else
            printf("Opcion invalida.\n");
    }
}

void registrarUsuario(int semilla) {
    FILE* f = fopen("usuarios.txt", "a");
    if (!f) { mostrarError("No se puede abrir usuarios.txt"); return; }
    char cedula[32], clave[32]; int saldo;

    printf("Ingrese cedula: ");
    if (scanf("%31s", cedula) != 1){ limpiar_buffer(); fclose(f); return; }
    limpiar_buffer();

    printf("Ingrese clave: ");
    if (scanf("%31s", clave) != 1){ limpiar_buffer(); fclose(f); return; }
    limpiar_buffer();

    printf("Ingrese saldo inicial: ");
    if (scanf("%d", &saldo) != 1){ limpiar_buffer(); fclose(f); return; }
    limpiar_buffer();

    char bin[MAX_BUFFER];
    textoABinario(cedula, bin); codificarMetodo2(bin, 4); fprintf(f, "%s\n", bin);
    textoABinario(clave,  bin); codificarMetodo2(bin, 4); fprintf(f, "%s\n", bin);
    char saldoStr[32]; sprintf(saldoStr, "%d", saldo);
    textoABinario(saldoStr, bin); codificarMetodo2(bin, 4); fprintf(f, "%s\n", bin);

    fclose(f);
    printf("Usuario creado exitosamente.\n");
}

static int validar_credenciales_usuario(const char* cedula, const char* clave){
    int base=0; char ced[64], pass[64]; int saldo=0;
    if (!buscar_usuario(cedula, &base)) return 0;
    if (!leer_registro_usuario(base, ced, pass, &saldo)) return 0;
    if (strcmp(pass, clave)!=0){
        printf("clave errada, intente de nuevo\n");
        return 0;
    }
    return 1;
}

void menuUsuario(int semilla) {
    char cedula[32], clave[32];
    printf("Ingrese su cedula: ");
    if (scanf("%31s", cedula) != 1){ limpiar_buffer(); return; }
    limpiar_buffer();
    printf("Ingrese su clave: ");
    if (scanf("%31s", clave) != 1){ limpiar_buffer(); return; }
    limpiar_buffer();

    if (!validar_credenciales_usuario(cedula, clave)) return;

    int base=0; char c[64], p[64]; int saldo=0; buscar_usuario(cedula, &base); leer_registro_usuario(base, c, p, &saldo);

    for(;;){
        int op=0;
        printf("\n--- MENU USUARIO ---\n1. Ver saldo\n2. Retirar dinero\n3. Salir\n");
        if (!leer_opcion_menu(&op)) continue;

        if (op==1){
            if (saldo < COSTO_OPERACION){
                printf("fondos insuficientes, por favor intente de nuevo\n");
                continue;
            }
            saldo -= COSTO_OPERACION;
            printf("Saldo disponible: %d\n", saldo);
            escribir_registro_saldo(base, saldo);
        } else if (op==2){
            int monto=0; printf("Ingrese monto a retirar: ");
            if (!leer_opcion_menu(&monto)) continue;
            if (monto<=0){ printf("Monto invalido\n"); continue; }
            if (saldo < monto + COSTO_OPERACION){
                printf("fondos insuficientes, por favor intente de nuevo\n");
                continue;
            }
            saldo -= (monto + COSTO_OPERACION);
            printf("Retiro exitoso. Nuevo saldo: %d\n", saldo);
            escribir_registro_saldo(base, saldo);
        } else if (op==3){
            break;
        } else {
            printf("Opcion invalida.\n");
        }
    }
}
