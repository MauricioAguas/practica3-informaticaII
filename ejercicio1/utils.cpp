#include "utils.h"
#include <bitset>
#include <sstream>
#include <stdexcept>

// Implementación de funciones de manipulación de archivos
std::string leerArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
    }
    
    std::string contenido;
    std::string linea;
    while (std::getline(archivo, linea)) {
        contenido += linea;
        if (!archivo.eof()) {
            contenido += "\n";
        }
    }
    archivo.close();
    return contenido;
}

void escribirArchivoBinario(const std::string& nombreArchivo, const std::string& contenido) {
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo: " + nombreArchivo);
    }
    
    // Convertir string binario a bytes y escribir
    for (size_t i = 0; i < contenido.length(); i += 8) {
        if (i + 8 <= contenido.length()) {
            std::string byte = contenido.substr(i, 8);
            char c = binaryToChar(byte);
            archivo.write(&c, 1);
        }
    }
    archivo.close();
}

std::string leerArchivoBinario(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
    }
    
    std::string contenido;
    char byte;
    while (archivo.read(&byte, 1)) {
        contenido += charToBinary(byte);
    }
    archivo.close();
    return contenido;
}

// Implementación de funciones de conversión binaria
std::string charToBinary(char c) {
    return std::bitset<8>(static_cast<unsigned char>(c)).to_string();
}

std::string stringToBinary(const std::string& texto) {
    std::string binario;
    for (char c : texto) {
        binario += charToBinary(c);
    }
    return binario;
}

char binaryToChar(const std::string& binario) {
    if (binario.length() != 8) {
        throw std::invalid_argument("La cadena binaria debe tener exactamente 8 bits");
    }
    return static_cast<char>(std::stoi(binario, nullptr, 2));
}

std::string binaryToString(const std::string& binario) {
    std::string texto;
    for (size_t i = 0; i < binario.length(); i += 8) {
        if (i + 8 <= binario.length()) {
            texto += binaryToChar(binario.substr(i, 8));
        }
    }
    return texto;
}

// Implementación de utilidades para codificación
std::string invertirBits(const std::string& bits) {
    std::string resultado;
    for (char bit : bits) {
        resultado += (bit == '0') ? '1' : '0';
    }
    return resultado;
}

int contarBits(const std::string& bits, char bit) {
    int contador = 0;
    for (char b : bits) {
        if (b == bit) {
            contador++;
        }
    }
    return contador;
}

// TODO: Implementar funciones de codificación método 1 y 2
// Esta es una implementación parcial para comenzar el proyecto

std::string codificarMetodo1(const std::string& contenido, int semilla) {
    // Implementación pendiente - Primera parte del proyecto
    std::string binario = stringToBinary(contenido);
    // ... lógica de codificación método 1
    return binario; // Placeholder temporal
}

std::string codificarMetodo2(const std::string& contenido, int semilla) {
    // Implementación pendiente - Primera parte del proyecto  
    std::string binario = stringToBinary(contenido);
    // ... lógica de codificación método 2
    return binario; // Placeholder temporal
}

// Funciones de decodificación (pendientes)
std::string decodificarMetodo1(const std::string& contenidoCodificado, int semilla) {
    // TODO: Implementar decodificación método 1
    return "";
}

std::string decodificarMetodo2(const std::string& contenidoCodificado, int semilla) {
    // TODO: Implementar decodificación método 2
    return "";
}

// Utilidades generales
void mostrarError(const std::string& mensaje) {
    std::cerr << "Error: " << mensaje << std::endl;
}

bool validarArgumentos(int argc, char* argv[]) {
    return argc == 5;
}