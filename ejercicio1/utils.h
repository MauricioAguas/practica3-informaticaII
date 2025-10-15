#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

// Funciones para manipulación de archivos
std::string leerArchivo(const std::string& nombreArchivo);
void escribirArchivoBinario(const std::string& nombreArchivo, const std::string& contenido);
std::string leerArchivoBinario(const std::string& nombreArchivo);

// Funciones para conversión binaria
std::string charToBinary(char c);
std::string stringToBinary(const std::string& texto);
char binaryToChar(const std::string& binario);
std::string binaryToString(const std::string& binario);

// Funciones para codificación - Método 1
std::string invertirBits(const std::string& bits);
std::string aplicarReglaBloqueAnterior(const std::string& bits, const std::string& bloqueAnterior);
int contarBits(const std::string& bits, char bit);

// Funciones para codificación - Método 2
std::string desplazarBitsCircular(const std::string& bits);

// Funciones de codificación principal
std::string codificarMetodo1(const std::string& contenido, int semilla);
std::string codificarMetodo2(const std::string& contenido, int semilla);

// Funciones de decodificación
std::string decodificarMetodo1(const std::string& contenidoCodificado, int semilla);
std::string decodificarMetodo2(const std::string& contenidoCodificado, int semilla);

// Utilidades generales
void mostrarError(const std::string& mensaje);
bool validarArgumentos(int argc, char* argv[]);

#endif // UTILS_H