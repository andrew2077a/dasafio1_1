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
        if (columnas<=0){
            cout<<"Numero invalido: ingrese otro"<<endl;
        }
        else{
            break;
        }
    }
    int fichasEstaJugada = 0;
    unsigned char* datos = nullptr;
    crearTablero(datos, filas, columnas);
    bytesReservados = calcularBytesNecesarios(filas * columnas);
    generarFichasIniciales(datos, filas, columnas);
    procesarCascadas(datos, filas, columnas, combinacionesDetectadas, fichasEstaJugada);


    bool jugando = true;
    while (jugando) {
        mostrarTablero(datos, filas, columnas);
        ImprimirMenu();
        cin >> opcion;

        // ---- CAMBIO EMPIEZA AQUI ----
        fichasEstaJugada = 0;
        bool huboAccion = false;
        // ---- CAMBIO TERMINA AQUI ----

        switch (opcion) {
        case 1: // Eliminar ficha
            cout << "Ingrese la fila: ";
            cin >> fila;
            cout << "Ingrese la columna: ";
            cin >> columna;
            if (eliminarFichaUsuario(datos, filas, columnas, fila, columna)) {
                eliminacionesUsuario++;
                // ---- CAMBIO EMPIEZA AQUI ----
                huboAccion = true;
                // ---- CAMBIO TERMINA AQUI ----
            } else {
                cout << "Movimiento invalido (fuera de rango o casilla vacia)." << endl;
            }
            break;

        // ---- CAMBIO EMPIEZA AQUI (case 2 completo) ----
        case 2: // Agregar fila
            cout << "Ingrese la posicion en donde deberia estar (0 a " << filas << "): ";
            cin >> posicion;
            if (posicion < 0 || posicion > filas) {
                cout << "Posicion invalida." << endl;
            } else {
                agregarFila(datos, filas, columnas, posicion, bytesReservados);
                huboAccion = true;
            }
            break;

        case 3: // Eliminar fila
            if (filas <= 1) {
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
            if (columnas <= 1) {
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
            // ---- CAMBIO TERMINA AQUI (case 2 a 5 completos) ----

        case 6: // Salir
            jugando = false;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

        // ---- CAMBIO EMPIEZA AQUI ----
        if (jugando && huboAccion) {
            cascadasActuales = procesarCascadas(datos, filas, columnas, combinacionesDetectadas, fichasEstaJugada);
            fichasEliminadasTotal += fichasEstaJugada;
            puntuacionTotal += calcularPuntuacion(fichasEstaJugada, cascadasActuales);
        } else if (jugando) {
            cascadasActuales = 0;
        }
        // ---- CAMBIO TERMINA AQUI ----

        if (jugando) {
            mostrarEstadoJuego(filas, columnas, eliminacionesUsuario,
                               fichasEliminadasTotal, combinacionesDetectadas,
                               cascadasActuales, puntuacionTotal);
        }
    }

    ImprimirFinal();
    destruirTablero(datos);
    return 0;
}
