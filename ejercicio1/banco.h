#ifndef BANCO_H
#define BANCO_H

#include "utils.h"
#include "binario_utils.h"
#include "codificador.h"
#include "decodificador.h"

/**
 * Funciones del sistema bancario
 */
void inicializarSistema();
void menuPrincipal(int semilla);

// Modo administrador
bool validarAdministrador(int semilla);
void menuAdministrador(int semilla);
void registrarUsuario(int semilla);

// Modo usuario
void menuUsuario(int semilla);

#endif
