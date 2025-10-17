#include "banco.h"
#include "utils.h"
#include "codificador.h"
#include "decodificador.h"
#include "binario_utils.h"
#include "input_utils.h"
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <cstdlib>

/* Constantes */
#define COSTO_OPERACION 1000
#define MAX_SALDO 999999999  // Soportar hasta 999 millones

/* Variables globales para método */
static int metodo_global = 1;  // 1 o 2
static int semilla_global = 4;

/* Funciones para persistir configuración */
static void guardar_configuracion() {
    FILE* f = fopen("config.txt", "w");
    if (f) {
        fprintf(f, "METODO=%d\n", metodo_global);
        fprintf(f, "SEMILLA=%d\n", semilla_global);
        fclose(f);
        printf("Configuracion guardada: Metodo %d, Semilla %d\n", metodo_global, semilla_global);
    }
}

static int cargar_configuracion() {
    FILE* f = fopen("config.txt", "r");
    if (!f) return 0;

    char linea[256];
    int metodo_temp = -1, semilla_temp = -1;

    while (fgets(linea, sizeof(linea), f)) {
        if (strncmp(linea, "METODO=", 7) == 0) {
            metodo_temp = atoi(linea + 7);
        } else if (strncmp(linea, "SEMILLA=", 8) == 0) {
            semilla_temp = atoi(linea + 8);
        }
    }
    fclose(f);

    if (metodo_temp > 0 && semilla_temp > 0) {
        metodo_global = metodo_temp;
        semilla_global = semilla_temp;
        return 1;
    }
    return 0;
}

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

static int leer_entero_positivo(int* dst, const char* prompt){
    printf("%s", prompt);
    if (scanf("%d", dst) != 1 || *dst < 0){
        printf("Entrada invalida. Debe ser un numero positivo.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();
    return 1;
}

static int leer_cadena_segura(char* dst, int max, const char* prompt){
    printf("%s", prompt);
    char formato[32];
    sprintf(formato, "%%%ds", max-1);  // %31s para max=32
    if (scanf(formato, dst) != 1){
        printf("Entrada invalida.\n");
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
    dst[i]=0;
    return (i>0 || c=='\n');
}

static void codificar_con_metodo(char* binario){
    if (metodo_global == 1) {
        codificarMetodo1(binario, semilla_global);
    } else {
        codificarMetodo2(binario, semilla_global);
    }
}

static void decodificar_con_metodo(char* binario){
    if (metodo_global == 1) {
        decodificarMetodo1(binario, semilla_global);
    } else {
        decodificarMetodo2(binario, semilla_global);
    }
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
            char bin[MAX_BUFFER];
            strncpy(bin, linea, sizeof(bin));
            bin[sizeof(bin)-1] = 0;
            decodificar_con_metodo(bin);
            char txt[256];
            binarioATexto(bin, txt);
            if (strcmp(txt, cedula)==0){
                *linea_base = idx;
                found = 1;
                break;
            }
        }
        idx++;
    }
    fclose(f);
    return found;
}

static int leer_registro_usuario(int base, char* out_ced, char* out_clave, long long* out_saldo){
    FILE* f = fopen("usuarios.txt", "r");
    if(!f) return 0;
    char linea[4096];
    int idx=0;
    int got=0;

    while (cargar_linea(f, linea, sizeof(linea))){
        if (idx==base){
            char bin[MAX_BUFFER];
            strncpy(bin, linea, sizeof(bin));
            bin[sizeof(bin)-1] = 0;
            decodificar_con_metodo(bin);
            binarioATexto(bin, out_ced);
            got++;
        } else if (idx==base+1){
            char bin[MAX_BUFFER];
            strncpy(bin, linea, sizeof(bin));
            bin[sizeof(bin)-1] = 0;
            decodificar_con_metodo(bin);
            binarioATexto(bin, out_clave);
            got++;
        } else if (idx==base+2){
            char bin[MAX_BUFFER];
            strncpy(bin, linea, sizeof(bin));
            bin[sizeof(bin)-1] = 0;
            decodificar_con_metodo(bin);
            char txt[64];
            binarioATexto(bin, txt);
            *out_saldo = atoll(txt);  // Usar atoll para long long
            got++;
            break;
        }
        idx++;
    }
    fclose(f);
    return got==3;
}

static int escribir_registro_saldo(int base, long long nuevo_saldo){
    /* reescribe solo la linea base+2 (saldo) manteniendo el resto */
    FILE* in = fopen("usuarios.txt", "r");
    if(!in) return 0;
    FILE* out = fopen("usuarios.tmp", "w");
    if(!out){
        fclose(in);
        return 0;
    }
    char linea[4096];
    int idx=0;

    while (cargar_linea(in, linea, sizeof(linea))){
        if (idx==base+2){
            char txt[64];
            sprintf(txt, "%lld", nuevo_saldo);  // long long format
            char bin[MAX_BUFFER];
            textoABinario(txt, bin);
            codificar_con_metodo(bin);
            fprintf(out, "%s\n", bin);
        } else {
            fprintf(out, "%s\n", linea);
        }
        idx++;
    }
    fclose(in);
    fclose(out);
    remove("usuarios.txt");
    rename("usuarios.tmp", "usuarios.txt");
    return 1;
}

void seleccionarMetodo() {
    int metodo = 0;
    int semilla = 0;

    printf("\n=== CONFIGURACION INICIAL ===\n");

    // Seleccionar método
    printf("Seleccione el metodo de codificacion:\n");
    printf("1. Metodo 1 (Inversion condicional)\n");
    printf("2. Metodo 2 (Desplazamiento circular)\n");

    while (1) {
        if (leer_opcion_menu(&metodo)) {
            if (metodo == 1 || metodo == 2) {
                metodo_global = metodo;
                printf("Metodo %d seleccionado.\n", metodo);
                break;
            } else {
                printf("Opcion invalida. Debe ser 1 o 2.\n");
            }
        }
    }

    // Ingresar semilla
    printf("\nIngrese la semilla de codificacion:\n");
    while (1) {
        if (leer_entero_positivo(&semilla, "Semilla: ")) {
            if (semilla > 0) {
                semilla_global = semilla;
                printf("Semilla %d configurada.\n", semilla);
                break;
            } else {
                printf("La semilla debe ser un numero positivo mayor a 0.\n");
            }
        }
    }

    printf("\nConfiguracion completa: Metodo %d, Semilla %d\n", metodo_global, semilla_global);
}

void inicializarSistema() {
    // Primero intentar cargar configuración existente
    if (cargar_configuracion()) {
        printf("\n=== CONFIGURACION ENCONTRADA ===\n");
        printf("Metodo: %d | Semilla: %d\n", metodo_global, semilla_global);
        printf("¿Desea usar esta configuracion? (1=Si, 0=No): ");
        int usar_existente = 0;
        if (leer_opcion_menu(&usar_existente) && usar_existente == 1) {
            printf("Usando configuracion existente.\n");
        } else {
            seleccionarMetodo();
            guardar_configuracion();
        }
    } else {
        printf("Primera ejecucion del sistema.\n");
        seleccionarMetodo();  // Primera vez
        guardar_configuracion();
    }

    // Verificar si ya existe sudo.txt
    FILE* test = fopen("sudo.txt", "r");
    if (test) {
        fclose(test);
        printf("\nSistema ya inicializado. Accediendo al menu principal.\n");
        menuPrincipal(semilla_global);
        return;
    }

    // Primera inicialización del admin
    printf("\n=== CONFIGURACION DE ADMINISTRADOR ===\n");
    char adminClave[64];
    while (1) {
        if (leer_cadena_segura(adminClave, sizeof(adminClave), "Ingrese la clave de administrador (primera vez): ")) {
            if (strlen(adminClave) >= 3) {
                break;
            } else {
                printf("La clave debe tener al menos 3 caracteres.\n");
            }
        }
    }

    char binario[MAX_BUFFER], codificado[MAX_BUFFER];
    textoABinario(adminClave, binario);
    strcpy(codificado, binario);
    codificar_con_metodo(codificado);
    guardarArchivoTexto("sudo.txt", codificado);

    printf("Sistema inicializado correctamente.\n");
    printf("Clave de admin guardada con Metodo %d, Semilla %d\n", metodo_global, semilla_global);
    menuPrincipal(semilla_global);
}

void menuPrincipal(int semilla) {
    // Asegurarse de tener la configuración correcta
    if (!cargar_configuracion()) {
        printf("Error: No se encontro configuracion. Reinicie el sistema.\n");
        return;
    }

    int opcion = 0;
    for (;;) {
        printf("\n--- SISTEMA BANCARIO ---\n");
        printf("Metodo actual: %d | Semilla: %d\n", metodo_global, semilla_global);
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
            printf("Opcion invalida. Debe ser 1, 2 o 3.\n");
        }
    }
}

bool validarAdministrador(int semilla) {
    // Asegurar que tenemos la configuración correcta
    cargar_configuracion();

    char claveGuardada[MAX_BUFFER];
    int len = 0;
    if (!leerArchivoTexto("sudo.txt", claveGuardada, len)) {
        mostrarError("No se encontro sudo.txt. Ejecute inicializacion primero.");
        return false;
    }

    printf("Validando con metodo %d, semilla %d\n", metodo_global, semilla_global);

    decodificar_con_metodo(claveGuardada);
    char texto[64];
    binarioATexto(claveGuardada, texto);

    char claveIngresada[64];
    if (!leer_cadena_segura(claveIngresada, sizeof(claveIngresada), "Ingrese clave admin: ")) {
        return false;
    }

    if (strcmp(texto, claveIngresada) != 0){
        printf("clave errada, intente de nuevo\n");
        return false;
    }
    printf("Acceso de administrador concedido.\n");
    return true;
}

void menuAdministrador(int semilla) {
    int op = 0;
    for (;;) {
        printf("\n--- MENU ADMINISTRADOR ---\n");
        printf("1. Crear usuario\n2. Salir\n");

        if (!leer_opcion_menu(&op)) continue;

        if (op == 1)
            registrarUsuario(semilla);
        else if (op == 2)
            break;
        else
            printf("Opcion invalida. Debe ser 1 o 2.\n");
    }
}

void registrarUsuario(int semilla) {
    // Asegurar configuración correcta
    cargar_configuracion();

    FILE* f = fopen("usuarios.txt", "a");
    if (!f) {
        mostrarError("No se puede abrir usuarios.txt");
        return;
    }

    char cedula[64], clave[64];
    long long saldo = 0;

    // Leer cédula con validación
    while (1) {
        if (leer_cadena_segura(cedula, sizeof(cedula), "Ingrese cedula: ")) {
            // Verificar que solo contenga números
            int valida = 1;
            for (int i = 0; cedula[i]; i++) {
                if (!isdigit(cedula[i])) {
                    valida = 0;
                    break;
                }
            }
            if (valida && strlen(cedula) > 0) {
                break;
            } else {
                printf("Cedula debe contener solo numeros.\n");
            }
        }
    }

    // Leer clave
    while (1) {
        if (leer_cadena_segura(clave, sizeof(clave), "Ingrese clave: ")) {
            if (strlen(clave) >= 3) {  // Mínimo 3 caracteres
                break;
            } else {
                printf("Clave debe tener al menos 3 caracteres.\n");
            }
        }
    }

    // Leer saldo inicial
    int saldo_temp;
    while (1) {
        if (leer_entero_positivo(&saldo_temp, "Ingrese saldo inicial: ")) {
            saldo = (long long)saldo_temp;
            break;
        }
    }

    // Guardar usuario codificado
    char bin[MAX_BUFFER];
    textoABinario(cedula, bin);
    codificar_con_metodo(bin);
    fprintf(f, "%s\n", bin);

    textoABinario(clave, bin);
    codificar_con_metodo(bin);
    fprintf(f, "%s\n", bin);

    char saldoStr[64];
    sprintf(saldoStr, "%lld", saldo);
    textoABinario(saldoStr, bin);
    codificar_con_metodo(bin);
    fprintf(f, "%s\n", bin);

    fclose(f);
    printf("Usuario creado exitosamente.\n");
    printf("Cedula: %s | Saldo inicial: %lld\n", cedula, saldo);
    printf("Guardado con metodo %d, semilla %d\n", metodo_global, semilla_global);
}

static int validar_credenciales_usuario(const char* cedula, const char* clave){
    // Asegurar configuración correcta
    cargar_configuracion();

    int base=0;
    char ced[64], pass[64];
    long long saldo=0;

    if (!buscar_usuario(cedula, &base)) {
        printf("Usuario no encontrado.\n");
        return 0;
    }

    if (!leer_registro_usuario(base, ced, pass, &saldo)) {
        printf("Error leyendo datos del usuario.\n");
        return 0;
    }

    if (strcmp(pass, clave)!=0){
        printf("clave errada, intente de nuevo\n");
        return 0;
    }

    printf("Acceso de usuario concedido. Saldo actual: %lld\n", saldo);
    return 1;
}

void menuUsuario(int semilla) {
    // Asegurar configuración correcta
    cargar_configuracion();

    char cedula[64], clave[64];

    // Login del usuario
    if (!leer_cadena_segura(cedula, sizeof(cedula), "Ingrese su cedula: ")) return;
    if (!leer_cadena_segura(clave, sizeof(clave), "Ingrese su clave: ")) return;

    if (!validar_credenciales_usuario(cedula, clave)) return;

    // Obtener datos del usuario
    int base=0;
    char c[64], p[64];
    long long saldo=0;

    buscar_usuario(cedula, &base);
    leer_registro_usuario(base, c, p, &saldo);

    // Menú de operaciones
    for(;;){
        int op=0;
        printf("\n--- MENU USUARIO ---\n");
        //printf("Saldo disponible: %lld\n", saldo);
        printf("1. Ver saldo (Costo: %d)\n", COSTO_OPERACION);
        printf("2. Retirar dinero (Costo: monto + %d)\n", COSTO_OPERACION);
        printf("3. Salir\n");

        if (!leer_opcion_menu(&op)) continue;

        if (op==1){
            if (saldo < COSTO_OPERACION){
                printf("fondos insuficientes, por favor intente de nuevo\n");
                printf("Necesita al menos %d para esta operacion.\n", COSTO_OPERACION);
                continue;
            }
            saldo -= COSTO_OPERACION;
            printf("=== CONSULTA DE SALDO ===\n");
            printf("Saldo disponible: %lld\n", saldo);
            printf("Se ha descontado %d por la consulta.\n", COSTO_OPERACION);
            escribir_registro_saldo(base, saldo);

        } else if (op==2){
            int monto=0;
            if (!leer_entero_positivo(&monto, "Ingrese monto a retirar: ")) continue;

            if (monto <= 0) {
                printf("Monto invalido. Debe ser mayor a 0.\n");
                continue;
            }

            if (saldo < (long long)monto + COSTO_OPERACION){
                printf("fondos insuficientes, por favor intente de nuevo\n");
                printf("Necesita %lld (monto + costo), pero tiene %lld\n",
                       (long long)monto + COSTO_OPERACION, saldo);
                continue;
            }

            saldo -= ((long long)monto + COSTO_OPERACION);
            printf("=== RETIRO EXITOSO ===\n");
            printf("Monto retirado: %d\n", monto);
            printf("Costo operacion: %d\n", COSTO_OPERACION);
            printf("Nuevo saldo: %lld\n", saldo);
            escribir_registro_saldo(base, saldo);

        } else if (op==3){
            printf("Gracias por usar nuestros servicios.\n");
            break;
        } else {
            printf("Opcion invalida. Debe ser 1, 2 o 3.\n");
        }
    }
}
