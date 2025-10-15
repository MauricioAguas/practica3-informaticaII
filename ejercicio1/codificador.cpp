#include "utils.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Validar argumentos de entrada
    if (argc != 5) {
        mostrarError("Uso: ./codificador <semilla> <metodo> <archivo_entrada> <archivo_salida>");
        return 1;
    }
    
    try {
        // Parsear argumentos
        int semilla = std::stoi(argv[1]);
        int metodo = std::stoi(argv[2]);
        std::string archivoEntrada = argv[3];
        std::string archivoSalida = argv[4];
        
        // Validar método
        if (metodo != 1 && metodo != 2) {
            mostrarError("El método debe ser 1 o 2");
            return 1;
        }
        
        // Leer archivo de entrada
        std::string contenido = leerArchivo(archivoEntrada);
        if (contenido.empty()) {
            mostrarError("No se pudo leer el archivo de entrada o está vacío");
            return 1;
        }
        
        std::string contenidoCodificado;
        
        // Aplicar método de codificación
        if (metodo == 1) {
            contenidoCodificado = codificarMetodo1(contenido, semilla);
            std::cout << "Aplicando método 1 (inversión basada en bloques anteriores)..." << std::endl;
        } else {
            contenidoCodificado = codificarMetodo2(contenido, semilla);
            std::cout << "Aplicando método 2 (desplazamiento circular)..." << std::endl;
        }
        
        // Escribir archivo de salida
        escribirArchivoBinario(archivoSalida, contenidoCodificado);
        
        std::cout << "Archivo codificado exitosamente." << std::endl;
        std::cout << "Entrada: " << archivoEntrada << std::endl;
        std::cout << "Salida: " << archivoSalida << std::endl;
        std::cout << "Semilla: " << semilla << std::endl;
        std::cout << "Método: " << metodo << std::endl;
        
    } catch (const std::exception& e) {
        mostrarError("Error durante la codificación: " + std::string(e.what()));
        return 1;
    }
    
    return 0;
}