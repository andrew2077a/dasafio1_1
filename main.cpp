#include "juego.h"
#include "bits.h"
#include "tablero.h"
#include <cstdlib> // Contiene rand() y srand()
#include <iostream>
#include <ctime>
using namespace std;
int main() {
    ImprimirLogo();
    srand(time(0));

    int filas, columnas, opcion, fila, columna, posicion, bytesReservados;
    int eliminacionesUsuario = 0, fichasEliminadasTotal = 0, combinacionesDetectadas = 0, cascadasActuales = 0, puntuacionTotal = 0;

    cout << "Ingrese las filas y columnas del tablero para iniciar el juego" << endl;
    while(true){
        cout << "Filas: ";
        cin >> filas;
        if (filas<=0){
            cout<<"Numero invalido: ingrese otro"<<endl;
        }
        else{
            break;
        }
    }
    while(true){
        cout << "Columnas: ";
        cin >> columnas;
        if (columnas<=0||columnas>36){ // limite maximo de columnas
            cout<<"Numero invalido: ingrese otro"<<endl;
        }
        else{
            break;
        }
    }
    int fichasEliminadas = 0;
    unsigned char* datos = nullptr;
    crearTablero(datos, filas, columnas);
    bytesReservados = calcularBytesNecesarios(filas * columnas);
    generarFichasIniciales(datos, filas, columnas);
    procesarCascadas(datos, filas, columnas, combinacionesDetectadas, fichasEliminadas); // resuelve combos que salieron ya armados al azar
    combinacionesDetectadas=0;


    bool jugando = true;
    while (jugando) {
        cout<<endl<<"TABLERO BINARIO"<<endl;
        mostrarTableroBinario(datos,filas,columnas);
        cout<<endl<<"TABLERO JUEGO"<<endl;
        mostrarTablero(datos, filas, columnas);
        ImprimirMenu();
        cin >> opcion;

        fichasEliminadas = 0; // se resetea en cada jugada
        bool huboAccion = false; // indica si de verdad se modifico el tablero esta vuelta

        switch (opcion) {
        case 1: // Eliminar ficha
            cout << "Ingrese la fila: ";
            cin >> fila;
            cout << "Ingrese la columna: ";
            cin >> columna;
            if (eliminarFichaUsuario(datos, filas, columnas, fila, columna)) {
                eliminacionesUsuario++;
                huboAccion = true;
            } else {
                cout << "Movimiento invalido (fuera de rango o casilla vacia)." << endl;
            }
            break;

        case 2: // Agregar fila
            cout << "Ingrese la posicion en donde deberia estar (0 a " << filas << "): ";
            cin >> posicion;
            if (posicion < 0 || posicion > filas) { // valida rango antes de tocar el tablero
                cout << "Posicion invalida." << endl;
            } else {
                agregarFila(datos, filas, columnas, posicion, bytesReservados);
                huboAccion = true;
            }
            break;

        case 3: // Eliminar fila
            if (filas <= 1) { // no dejar el tablero sin filas
                cout << "No se puede eliminar: el tablero se quedaria sin filas." << endl;
                break;
            }
            cout << "Ingrese la fila a eliminar (0 a " << filas-1 << "): ";
            cin >> posicion;
            if (posicion < 0 || posicion >= filas) {
                cout << "Posicion invalida." << endl;
            } else {
                eliminarFila(datos, filas, columnas, posicion, bytesReservados);
                huboAccion = true;
            }
            break;

        case 4: // Agregar columna
            cout << "Ingrese la posicion en donde deberia estar (0 a " << columnas << "): ";
            cin >> posicion;
            if (posicion < 0 || posicion > columnas) {
                cout << "Posicion invalida." << endl;
            } else {
                agregarColumna(datos, filas, columnas, posicion, bytesReservados);
                huboAccion = true;
            }
            break;

        case 5: // Eliminar columna
            if (columnas <= 1) { // no dejar el tablero sin columnas
                cout << "No se puede eliminar: el tablero se quedaria sin columnas." << endl;
                break;
            }
            cout << "Ingrese la columna a eliminar (0 a " << columnas-1 << "): ";
            cin >> posicion;
            if (posicion < 0 || posicion >= columnas) {
                cout << "Posicion invalida." << endl;
            } else {
                eliminarColumna(datos, filas, columnas, posicion, bytesReservados);
                huboAccion = true;
            }
            break;

        case 6: // Salir
            jugando = false;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

        // solo se procesan cascadas si de verdad hubo una accion que cambio el tablero
        if (jugando && huboAccion) {
            cascadasActuales = procesarCascadas(datos, filas, columnas, combinacionesDetectadas, fichasEliminadas);
            fichasEliminadasTotal += fichasEliminadas;
            puntuacionTotal += calcularPuntuacion(fichasEliminadas, cascadasActuales); // se usa solo lo de esta jugada
        } else if (jugando) {
            cascadasActuales = 0; // no hubo accion real, no hay cascada que mostrar
        }

        if (jugando) {
            mostrarEstadoJuego(filas, columnas, eliminacionesUsuario,
                               fichasEliminadasTotal, combinacionesDetectadas,
                               cascadasActuales, puntuacionTotal);
        }
    }

    ImprimirFinal();
    liberarMarcas();
    destruirTablero(datos);
    return 0;
}
