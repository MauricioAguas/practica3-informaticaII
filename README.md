# Práctica 3 - Informática II
**Universidad de Antioquia - Facultad de Ingeniería**

## Objetivos
- Aprender a manipular cadenas de caracteres en C++
- Aprender a gestionar archivos en C++
- Conocer cómo manejar excepciones

## Descripción
Esta práctica implementa dos ejercicios principales:

### Ejercicio 1: Codificador de Archivos
Programa que codifica archivos de texto usando dos métodos diferentes:
- **Método 1**: Inversión de bits basada en conteo de bloques anteriores
- **Método 2**: Desplazamiento circular de bits dentro de cada bloque

### Ejercicio 2: Decodificador de Archivos
Programa complementario que decodifica los archivos generados por el Ejercicio 1.

### Aplicación: Sistema de Cajero Electrónico
Sistema completo de registro y gestión de usuarios con:
- Acceso de administrador
- Registro de usuarios
- Operaciones bancarias (consulta y retiro)
- Encriptación de transacciones

## Restricciones Técnicas
La práctica debe implementarse de **dos formas**:
1. Usando manipulación de **arreglos de char**
2. Usando la **clase string**

## Estructura del Proyecto
```
practica3-informaticaII/
├── README.md
├── ejercicio1/
│   ├── codificador.cpp
│   ├── decodificador.cpp
│   ├── utils.h
│   ├── utils.cpp
│   └── tests/
├── ejercicio2/
├── aplicacion/
└── docs/
```

## Compilación
```bash
g++ -o codificador ejercicio1/codificador.cpp ejercicio1/utils.cpp
g++ -o decodificador ejercicio1/decodificador.cpp ejercicio1/utils.cpp
```

## Uso
```bash
./codificador <semilla> <método> <archivo_entrada> <archivo_salida>
./decodificador <semilla> <método> <archivo_codificado> <archivo_salida>
```

## Autor
Mauricio Aguas - Estudiante de Ingeniería Electrónica