#include "juego.h"
#include "bits.h"


// 3.2
bool eliminarFichaUsuario(unsigned char* datos, int filas, int columnas, int fila, int columna){
    if (fila >= 0 && fila < filas && columna >= 0 && columna < columnas) {
        int posicion = calcularIndice(fila, columna, columnas);
        establecerFicha(datos, posicion, 0);
        return true;
    }
    return false;
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
            if (ficha1 != 0 && ficha1 == ficha2 && ficha2 == ficha3) {
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
//3.5
int calcularPuntuacion(int fichasEliminadas, int cascadas){
    if (fichasEliminadas == 0){
        return 0;
    }
    int PuntosAcumulados = fichasEliminadas * 10;
    int PuntosFinales = PuntosAcumulados;
    if ( cascadas > 0  ){
        PuntosFinales = PuntosAcumulados *(cascadas + 2);
    }
    return PuntosFinales;
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

//3.8
int resolverCombinaciones(unsigned char* datos, int filas, int columnas){
    static unsigned char* marcas = nullptr;
    static int capacidadActual = 0;
    int posiciones = filas * columnas;

    if (posiciones > capacidadActual) {          // solo reserva si el tablero creció
        delete[] marcas;
        marcas = new unsigned char[posiciones];
        capacidadActual = posiciones;
    }
    for (int i = 0; i < posiciones; i++) marcas[i] = 0; // igual hay que limpiarlo cada vez

    int fichasBorradas = 0;
    detectarCombinacionesHorizontales(datos, filas, columnas, marcas);
    detectarCombinacionesVerticales(datos, filas, columnas, marcas);

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int indice = calcularIndice(f, c, columnas);
            if (marcas[indice] == 1) {
                establecerFicha(datos, indice, 0);
                fichasBorradas++;
            }
        }
    }
    return fichasBorradas;
}





















