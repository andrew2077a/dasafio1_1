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
    int eliminacionesUsuario = 0, fichasEliminadasTotal = 0, combinacionesTotales = 0, cascadasActuales = 0, puntuacion = 0;
    cout << "Ingrese las filas y columnas del tablero para iniciar el juego" << endl;
    while(true){
        cout << "Filas: ";
        cin >> filas;
        if (filas<=0){
            cout<<"Numero invalido: ingrese otro";
        }
        else{
            break;
        }
    }
    while(true){
        cout << "Columnas: ";
        cin >> columnas;
        if (columnas<=0){
            cout<<"Numero invalido: ingrese otro";
        }
        else{
            break;
        }
    }
    unsigned char* datos = nullptr; // CORRECCION: datos debe ser puntero, no unsigned char suelto
    crearTablero(datos, filas, columnas);
    bytesReservados = calcularBytesNecesarios(filas * columnas); // CORRECCION: se inicializa aqui, antes no existia el valor inicial
    generarFichasIniciales(datos, filas, columnas);

    bool jugando = true; // CORRECCION: bandera para poder salir del while(true) con la opcion 6
    while (jugando) {
        mostrarTablero(datos, filas, columnas);
        ImprimirMenu();
        cin >> opcion;

        switch (opcion) {
        case 1: // Eliminar ficha
            cout << "Ingrese la fila" << endl;
            cin >> fila;
            cout << "Ingrese la columna" << endl;
            cin >> columna;

            if (eliminarFichaUsuario(datos, filas, columnas, fila, columna)) { // CORRECCION: se revisa el resultado antes de seguir
                eliminacionesUsuario++; // CORRECCION: ahora si se cuenta
                cascadasActuales = procesarCascadas(datos, filas, columnas, combinacionesTotales); // CORRECCION: ya no se llama resolverCombinaciones aparte, procesarCascadas lo hace internamente
            } else {
                cout << "Movimiento invalido (fuera de rango o casilla vacia)." << endl;
                cascadasActuales = 0;
                combinacionesTotales = 0;
            }
            puntuacion = calcularPuntuacion(fichasEliminadasTotal, cascadasActuales); // fichasEliminadasTotal se mantiene en 0 por ahora, procesarCascadas no expone ese dato
            break;

        case 2: // Agregar fila
            cout << "Ingrese la posicion en donde deberia estar" << endl;
            cin >> posicion;
            agregarFila(datos, filas, columnas, posicion, bytesReservados); // CORRECCION: era eliminarFila, la opcion 2 del menu es "Agregar fila"
            break;

        case 3: // Eliminar fila
            cout << "Ingrese la posicion a eliminar" << endl;
            cin >> posicion;
            eliminarFila(datos, filas, columnas, posicion, bytesReservados);
            break;

        case 4: // Agregar columna
            cout << "Ingrese la posicion en donde deberia estar" << endl;
            cin >> posicion;
            agregarColumna(datos, filas, columnas, posicion, bytesReservados);
            break;

        case 5: // Eliminar columna
            cout << "Ingrese la posicion a eliminar" << endl;
            cin >> posicion;
            eliminarColumna(datos, filas, columnas, posicion, bytesReservados);
            break;

        case 6: // Salir
            jugando = false; // CORRECCION: ahora si se puede salir del bucle
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

        if (jugando) {
            mostrarEstadoJuego(filas, columnas, eliminacionesUsuario,
                               fichasEliminadasTotal, combinacionesTotales,
                               cascadasActuales, puntuacion);
        }
    }

    ImprimirFinal();
    destruirTablero(datos); // CORRECCION: se libera la memoria antes de terminar
    return 0;
}
