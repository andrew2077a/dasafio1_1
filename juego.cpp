#include "juego.h"
#include "bits.h"
#include "tablero.h"
#include <cstdlib> // Contiene rand() y srand()
#include <iostream>
using namespace std;

// 3.1
unsigned char generarFichaAleatoria(){
    return 1+ rand()%6; // rango 1-6
}

// 3.2
bool eliminarFichaUsuario(unsigned char* datos, int filas, int columnas, int fila, int columna){
    if (fila >= 0 && fila < filas && columna >= 0 && columna < columnas) { // valida límites
        int posicion = calcularIndice(fila, columna, columnas);
        if (obtenerFicha(datos, posicion) == 0) return false; // ya estaba vacia, no hay nada que eliminar
        establecerFicha(datos, posicion, 0); // marca como vacío
        return true;
    }
    return false;
}

// 3.3
int detectarCombinacionesHorizontales(unsigned char* datos, int filas, int columnas, unsigned char* marcas){
    int cunt = 0;
    for (int i = 0; i < filas; i++){
        int cont = 1; // la primera ficha de la fila ya cuenta como 1
        unsigned char comparar = obtenerFicha(datos, calcularIndice(i, 0, columnas));
        for (int j = 1; j < columnas; j++){
            unsigned char ficha = obtenerFicha(datos, calcularIndice(i, j, columnas));
            if (ficha == comparar && ficha != 0){ // CORRECCION: != 0, "vacio" nunca cuenta como combo
                cont++;
            }
            else {
                if (cont >= 3 && comparar != 0){ // CORRECCION: comparar != 0
                    for (int e = 0; e < cont; e++){
                        marcas[calcularIndice(i, j-1-e, columnas)] = 1;
                    }
                    cunt++;
                }
                cont = 1; // se reinicia con la ficha actual, que empieza una racha nueva
            }
            comparar = ficha;
        }
        if (cont >= 3 && comparar != 0){ // CORRECCION: comparar != 0
            for (int e = 0; e < cont; e++){
                marcas[calcularIndice(i, columnas-1-e, columnas)] = 1;
            }
            cunt++;
        }
    }
    return cunt;
}

// 3.4
int detectarCombinacionesVerticales(unsigned char* datos, int filas, int columnas, unsigned char* marcas){
    int combinacionesDetectadas = 0;
    int c = 0 ;
    while (c < columnas){
        int f = 0 ;
        while (f < filas - 2){
            int indice1 = calcularIndice(f, c, columnas);
            int indice2 = calcularIndice(f + 1, c, columnas);
            int indice3 = calcularIndice(f + 2, c, columnas);
            unsigned char ficha1 = obtenerFicha(datos, indice1);
            unsigned char ficha2 = obtenerFicha(datos, indice2);
            unsigned char ficha3 = obtenerFicha(datos, indice3);
            if (ficha1 != 0 && ficha1 == ficha2 && ficha2 == ficha3) { // 3 iguales seguidas
                if (marcas[indice1] == 0) { // evita contar de nuevo una racha ya marcada
                    combinacionesDetectadas++;
                }
                marcas[indice1] = 1;
                marcas[indice2] = 1;
                marcas[indice3] = 1;
            }
            f++;
        }
        c++;
    }
    return combinacionesDetectadas;
}

// 3.5
int calcularPuntuacion(int fichasEliminadas, int cascadas){
    if (fichasEliminadas == 0){ // sin puntos si no eliminó nada
        return 0;
    }
    int PuntosAcumulados = fichasEliminadas * 10;
    int PuntosFinales = PuntosAcumulados;
    if ( cascadas > 0  ){ // multiplicador solo si hubo cascada
        PuntosFinales = PuntosAcumulados *(cascadas + 2);
    }
    return PuntosFinales;
}

// 3.6
void mostrarEstadoJuego(int filas, int columnas, int eliminacionesUsuario,
                        int fichasEliminadasTotal, int combinacionesDetectadas,
                        int cascadasActuales, int puntuacion){
    cout << "+--------------------------------------------+" << endl;
    cout << "|              ESTADO DEL JUEGO               |" << endl;
    cout << "+--------------------------------------------+" << endl;
    cout << "| Tamano del tablero      : " << filas << " x " << columnas << endl;
    cout << "| Eliminaciones usuario   : " << eliminacionesUsuario << endl;
    cout << "| Fichas eliminadas total : " << fichasEliminadasTotal << endl;
    cout << "| Combinaciones detectadas: " << combinacionesDetectadas << endl;
    cout << "| Cascadas actuales       : " << cascadasActuales << endl;
    cout << "| Puntuacion              : " << puntuacion << endl;
    cout << "+--------------------------------------------+" << endl;
}

// 3.7
void generarFichasIniciales(unsigned char* datos, int filas, int columnas){
    int f = 0 ;
    while ( f < filas  ){
        int c = 0 ;
        while (c < columnas){
            int posicion = calcularIndice(f, c, columnas);
            unsigned char FichaAleatoria = generarFichaAleatoria();
            establecerFicha(datos, posicion, FichaAleatoria);
            c++;
        }
        f++;
    }
}

// 3.8
int resolverCombinaciones(unsigned char* datos, int filas, int columnas, int& combosDetectados){
    int posiciones = filas * columnas;
    unsigned char* marcas = new unsigned char[posiciones]; // CAMBIO: se quitó capacidadActual y el if, ya no tenían proposito sin static
    for (int i = 0; i < posiciones; i++) marcas[i] = 0; // igual hay que limpiarlo cada vez

    int fichasBorradas = 0;
    int combosH = detectarCombinacionesHorizontales(datos, filas, columnas, marcas);
    int combosV = detectarCombinacionesVerticales(datos, filas, columnas, marcas);
    combosDetectados = combosH + combosV; // NUEVO: ya no se descarta

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int indice = calcularIndice(f, c, columnas);
            if (marcas[indice] == 1) {
                establecerFicha(datos, indice, 0); // vacía las marcadas
                fichasBorradas++;
            }
        }
    }
    delete[] marcas; // se libera aquí mismo, ya no hay fuga
    return fichasBorradas;
}

// 3.9
void reorganizarTablero(unsigned char* datos, int filas, int columnas){
    for(int j=0;j<columnas;j++){
        int escritura=filas-1;
        for(int i=filas-1;i>=0;i--){
            int indi=calcularIndice(i, j, columnas);
            unsigned char ficha = obtenerFicha(datos, indi);
            if(ficha!=0){
                if(escritura != i){                     // solo escribe si la posición cambió de verdad
                    int idxEscritura = calcularIndice(escritura, j, columnas);
                    establecerFicha(datos, idxEscritura, ficha);
                }
                escritura--;
            }
        }
        while(escritura>=0){ // rellena huecos restantes arriba
            int idxEscritura = calcularIndice(escritura, j, columnas);
            establecerFicha(datos, idxEscritura, generarFichaAleatoria());
            escritura--;
        }
    }
}

// 3.10
int procesarCascadas(unsigned char* datos, int filas, int columnas, int& combinacionesTotales){
    int cascadas = 0;
    combinacionesTotales = 0;
    int combosDetectados = 0;

    int eliminadas = resolverCombinaciones(datos, filas, columnas, combosDetectados);
    combinacionesTotales += combosDetectados;

    while(eliminadas != 0){ // repite mientras haya combinaciones nuevas
        reorganizarTablero(datos, filas, columnas);
        cascadas++;
        eliminadas = resolverCombinaciones(datos, filas, columnas, combosDetectados);
        combinacionesTotales +=combosDetectados;
    }

    return cascadas;
}
void ImprimirMenu(){
    cout << "\n========================================" << endl;
    cout << "           OPCIONES DE JUEGO            " << endl;
    cout << "========================================" << endl;
    cout << "1. Eliminar ficha" << endl;
    cout << "2. Agregar fila" << endl;
    cout << "3. Eliminar fila" << endl;
    cout << "4. Agregar columna" << endl;
    cout << "5. Eliminar columna" << endl;
    cout << "6. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
}
void ImprimirLogo(){
    cout << "   *  .  o  O  o  .  *      C A N D Y   C R U S H      *  .  o  O  o  .  *" << endl;
    cout << "==============================================================================" << endl;
    cout << "  ____    _    _   _  ____ __   __   ____ ____  _   _ ____  _   _ " << endl;
    cout << " / ___|  / \\  | \\ | ||  _ \\\\ \\ / /  / ___|  _ \\| | | / ___|| | | |" << endl;
    cout << "| |     / _ \\ |  \\| || | | |\\ V /  | |   | |_) | | | \\___ \\| |_| |" << endl;
    cout << "| |___ / ___ \\| |\\  || |_| | | |   | |___|  _ <| |_| |___) |  _  |" << endl;
    cout << " \\____/_/   \\_\\_| \\_||____/  |_|    \\____|_| \\_\\\\___/|____/|_| |_|" << endl;
    cout << "==============================================================================" << endl;
    cout << "   *  .  o  O  o  .  *                                  *  .  o  O  o  .  *" << endl<<endl;
}
void ImprimirFinal(){
    cout << "==============================================================================" << endl;
    cout << "            ____    _    __  __ _____     _____     _______ ____  " << endl;
    cout << "           / ___|  / \\  |  \\/  | ____|   / _ \\ \\   / / ____|  _ \\ " << endl;
    cout << "          | |  _  / _ \\ | |\\/| |  _|    | | | \\ \\ / /|  _| | |_) |" << endl;
    cout << "          | |_| |/ ___ \\| |  | | |___   | |_| |\\ V / | |___|  _ < " << endl;
    cout << "           \\____/_/   \\_\\_|  |_|_____|   \\___/  \\_/  |_____|_| \\_\\" << endl;
    cout << "==============================================================================" << endl;
}
