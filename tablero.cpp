#include "tablero.h"
#include "bits.h"
#include <iostream>
using namespace std;

// 2.1
void crearTablero(unsigned char*& datos, int& filas, int& columnas, int filasIniciales, int columnasIniciales){
    filas = filasIniciales;
    columnas = columnasIniciales;
    int posiciones = filas * columnas;
    int totalBytes = calcularBytesNecesarios(posiciones);
    datos = new unsigned char[totalBytes]; // reserva memoria exacta, sin sobras
}

// 2.2
void destruirTablero(unsigned char*& datos){
    delete[] datos;
    datos = nullptr; // evita puntero colgante
}

// 2.3
void redimensionarTablero(unsigned char*& datos, int filasViejas, int columnasViejas, int filasNuevas, int columnasNuevas){
    int BytesNuevos = calcularBytesNecesarios(filasNuevas * columnasNuevas);
    unsigned char* BloqueNuevo = new unsigned char[BytesNuevos];
    for (int i = 0; i < BytesNuevos; i++) {
        BloqueNuevo[i] = 0; // arranca en 0 para no dejar basura en bits sin usar
    }

    // solo se copia lo que cabe en ambos tamaños (viejo y nuevo)
    int LimiteFilas = (filasViejas < filasNuevas) ? filasViejas : filasNuevas;
    int LimiteColumnas = (columnasViejas < columnasNuevas) ? columnasViejas : columnasNuevas;

    int f = 0;
    while (f < LimiteFilas){
        int c = 0;
        while (c < LimiteColumnas){
            int indiceViejo = calcularIndice(f, c, columnasViejas);
            unsigned char ficha = obtenerFicha(datos, indiceViejo);
            int indiceNuevo = calcularIndice(f, c, columnasNuevas);
            establecerFicha(BloqueNuevo, indiceNuevo, ficha); // recoloca cada ficha en su nuevo índice
            c++;
        }
        f++;
    }

    delete[] datos;
    datos = BloqueNuevo; // el tablero ya apunta al arreglo nuevo
}

// 2.4
double calcularPorcentajeUso(int filas, int columnas, int bytesReservadosActuales){
    int BytesNecesarios = calcularBytesNecesarios(filas * columnas);
    return (BytesNecesarios * 100.0) / bytesReservadosActuales; // % de lo reservado que de verdad se usa
}

// 2.5
void mostrarTablero(unsigned char* datos, int filas, int columnas){
    unsigned char ficha;
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            cout << "|";
            ficha = obtenerFicha(datos, calcularIndice(i, j, columnas));
            switch (ficha) { // cada valor de ficha -> un carácter decorativo
            case 0b00000001: cout << (char)254; break;
            case 0b00000011: cout << (char)205; break;
            case 0b00000111: cout << (char)206; break;
            case 0b00000101: cout << (char)36;  break;
            case 0b00000110: cout << (char)35;  break;
            case 0b00000100: cout << (char)64;  break;
            default: break;
            }
            cout << "|";
        }
        cout << endl;
    }
}

// 2.6
void agregarFila(unsigned char*& datos, int& filas, int columnas, int posicion, int& bytesReservadosActuales){
    int filasViejas = filas;
    int filasNuevas = filas + 1;
    int bytesNecesarios = calcularBytesNecesarios(filasNuevas * columnas);

    if (bytesNecesarios > bytesReservadosActuales) { // solo se agranda si ya no cabe
        redimensionarTablero(datos, filasViejas, columnas, filasNuevas, columnas);
        bytesReservadosActuales = bytesNecesarios;
    }

    filas = filasNuevas;

    if (posicion < filasViejas){ // hay que abrir espacio en medio
        int f = filasViejas - 1;
        while (f >= posicion) { // de atrás hacia adelante para no pisar datos sin leer
            int c = 0;
            while (c < columnas){
                int indiceOrigen = calcularIndice(f, c, columnas);
                unsigned char ficha = obtenerFicha(datos, indiceOrigen);
                int indiceDestino = calcularIndice(f + 1, c, columnas);
                establecerFicha(datos, indiceDestino, ficha); // corre la fila una posición abajo
                c++;
            }
            f--;
        }
    }

    int c = 0;
    while (c < columnas) {
        int indice = calcularIndice(posicion, c, columnas);
        establecerFicha(datos, indice, 1); // llena la fila nueva con ficha por defecto
        c++;
    }
}

// 2.7
void eliminarFila(unsigned char*& datos, int& filas, int columnas, int posicion, int& bytesReservados){

    for(int fila = posicion; fila < filas - 1; fila++){ // corre las filas siguientes una posición arriba
        for(int columna = 0; columna < columnas; columna++){
            int indiceOrigen = calcularIndice(fila + 1, columna, columnas);
            unsigned char valor = obtenerFicha(datos, indiceOrigen);
            int indiceDestino = calcularIndice(fila, columna, columnas);
            establecerFicha(datos, indiceDestino, valor);
        }
    }

    int filasViejas = filas;
    filas--;

    double porcentajeUso = calcularPorcentajeUso(filas, columnas, bytesReservados);

    if(porcentajeUso < 65.0){ // solo se achica la memoria si ya sobra mucho espacio
        redimensionarTablero(datos, filasViejas, columnas, filas, columnas);
        bytesReservados = calcularBytesNecesarios(filas * columnas);
    }
}

// 2.8
void agregarColumna(unsigned char*& datos, int filas, int& columnas, int posicion, int& bytesReservados){
    int columnasNuevas = columnas + 1;
    int bytesNuevos = calcularBytesNecesarios(filas * columnasNuevas);
    unsigned char* datosNuevo = new unsigned char[bytesNuevos]; // ancho nuevo obliga a reconstruir todo

    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            int columnaNueva = (j < posicion) ? j : j + 1; // desplaza las columnas después de "posicion"

            int indiceViejo = calcularIndice(i, j, columnas);
            unsigned char valor = obtenerFicha(datos, indiceViejo);

            int indiceNuevo = calcularIndice(i, columnaNueva, columnasNuevas);
            establecerFicha(datosNuevo, indiceNuevo, valor);
        }

        int indiceColumnaNueva = calcularIndice(i, posicion, columnasNuevas);
        establecerFicha(datosNuevo, indiceColumnaNueva, 0); // columna nueva, vacía por ahora
    }

    delete[] datos;
    datos = datosNuevo;
    columnas = columnasNuevas;
    bytesReservados = bytesNuevos;
}

// 2.9
void eliminarColumna(unsigned char*& datos, int filas, int& columnas, int posicion, int& bytesReservadosActuales){
    int columnasViejas = columnas;
    int columnasNuevas = columnas - 1;

    int f = 0;
    while(f < filas){
        int c = 0;
        while(c < columnasViejas){
            if (c == posicion) { c++; continue; } // se salta la columna que se elimina

            int columnaNueva = (c < posicion) ? c : c - 1;

            int indiceViejo = calcularIndice(f, c, columnasViejas);
            unsigned char ficha = obtenerFicha(datos, indiceViejo);

            int indiceNuevo = calcularIndice(f, columnaNueva, columnasNuevas);
            establecerFicha(datos, indiceNuevo, ficha); // compacta dentro del mismo arreglo
            c++;
        }
        f++;
    }

    columnas = columnasNuevas;
    double porcentajeUso = calcularPorcentajeUso(filas, columnas, bytesReservadosActuales);
    if (porcentajeUso < 65){ // solo se libera memoria física si ya sobra mucho
        redimensionarTablero(datos, filas, columnas, filas, columnas);
        bytesReservadosActuales = calcularBytesNecesarios(filas * columnas);
    }
}
