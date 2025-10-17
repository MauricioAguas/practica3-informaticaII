#ifndef CODIFICACIONES_H
#define CODIFICACIONES_H
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>

using namespace std;

//Esta función aplica las reglas de la decodificación uno
void aplicarReglasInv(string &grupo, string &grupoAnterior){

    int unosAnterior = 0, cerosAnterior = 0;

    for(char bit : grupoAnterior){
        if (bit == '1'){
            unosAnterior++;
        } else{
            cerosAnterior++;
        }
    }

    if(unosAnterior == cerosAnterior){

        for (char &bit : grupo){
            bit = (bit == '1') ? '0' : '1';
        }

    } else if(unosAnterior > cerosAnterior){
        int tamaño= grupo.length();
        for (int i = 2; i < tamaño; i += 3){
            grupo[i] = (grupo[i] == '1') ? '0' : '1';
        }
    } else{
        int tamaño= grupo.length();
        for (int i = 1; i < tamaño; i += 2){
            grupo[i] = (grupo[i] == '1') ? '0' : '1';
        }
    }
}


//Esta función decodifica bajo el método uno
void decodificarM1(string &cadena, int n){

    int longitud = cadena.length();
    string resultado = "";

    string primerBloque = cadena.substr(0, n);

    for(char &bit : primerBloque){
        bit = (bit == '1') ? '0' : '1';
    }

    resultado += primerBloque;

    string grupoAnterior = primerBloque;

    for(int i = n; i < longitud; i += n){
        string grupo = cadena.substr(i, n);
        aplicarReglasInv(grupo, grupoAnterior);
        resultado += grupo;
        grupoAnterior = grupo;
    }
    cadena = resultado;
}

//Esta función decodifica bajo el método dos
string decodificarM2(string cadena, int n){
    int longitud = cadena.length();
    string resultado = "";

    for (int i = 0; i < longitud; i += n){
        string grupo = cadena.substr(i, n);

        char primerBit = grupo[0];

        for (int j = 1; j < n; j++){
            grupo[j - 1] = grupo[j];
        }
        grupo[n - 1] = primerBit;

        resultado += grupo;
    }

    return resultado;
}


/*Esta función recibe una de esas cadenas de bits, luego toma por bloques y decodifica cada caracter,
añadiendolo al string que representa esa cadena de bits*/
string convBinInt(string linea){
    string recibido = "";
    int len = linea.length();
    int residuo = len % 8;
    for(int i = 0; i < (len - residuo); i += 8){
        string sublinea = "";
        sublinea = linea.substr(i, 8);
        int entero = 0;
        for(int j = 0; j < 8; j++){
            int aux = sublinea[j] - 48;
            aux = (pow(2, (7- j))) * aux;
            entero += aux;
        }
        recibido += char(entero);
    }
    return recibido;

}


//Esta función devuelve el arreglo de caracteres que va a recibir el entero para convertir en números binarios
void convIntBin(char *linea, int ent){
    for (int i = 7; i >= 0; i--) {
        linea[i] = (ent % 2) + '0';
        ent = ent / 2;
    }
    linea[8]= '\0';
}


//Esta función aplica las reglas del método 1 de codificación
void aplicarReglas(string &grupo, string &grupoAnterior){

    int unosAnterior = 0, cerosAnterior = 0;

    for (char bit : grupoAnterior){
        if (bit == '1'){
            unosAnterior++;
        } else{
            cerosAnterior++;
        }
    }

    if(unosAnterior == cerosAnterior){

        for(char &bit : grupo){
            bit = (bit == '1') ? '0' : '1';
        }

    }else if(unosAnterior > cerosAnterior){
        int tamaño= grupo.length();
        for(int i = 2; i < tamaño; i += 3){
            grupo[i] = (grupo[i] == '1') ? '0' : '1';
        }
    }else{
        int tamaño= grupo.length();
        for (int i = 1; i < tamaño; i += 2){
            grupo[i] = (grupo[i] == '1') ? '0' : '1';
        }
    }
}


//Esta función se encarga de codificar bajo el segundo método
string codificarM2(string &cadena, int n){
    int longitud = cadena.length();
    string resultado = "";

    for (int i = 0; i < longitud; i += n){
        string grupo = cadena.substr(i, n);

        char ultimoBit = grupo[n - 1];
        for (int j = n - 2; j >= 0; j--){
            grupo[j + 1] = grupo[j];
        }
        grupo[0] = ultimoBit;

        resultado += grupo;
    }
    return resultado;

}


//Esta función se encarga de codificar bajo el primer método
void codificarM1(string &cadena, int n){
    int longitud = cadena.length();
    string resultado = "";

    string primerBloque = cadena.substr(0, n);

    string grupoAnterior = primerBloque;

    for(char &bit : primerBloque){
        bit = (bit == '1') ? '0' : '1';
    }
    resultado += primerBloque;

    for (int i = n; i < longitud; i += n){
        string grupo = cadena.substr(i, n);
        aplicarReglas(grupo, grupoAnterior);
        resultado += grupo;
        grupoAnterior = cadena.substr(i,n);
    }
    cadena = resultado;
}

string quitarCeros(string linea, int cantidad_bytes){
    string sincero = "";
    sincero = linea.substr(0,8*cantidad_bytes);

    return sincero;
}
#endif // CODIFICACIONES_H
