#include "tablero.h"
#include "bits.h"

double calcularPorcentajeUso(int filas, int columnas, int bytesReservadosActuales){
    int BytesNecesarios = calcularBytesNecesarios( filas * columnas);
    return (BytesNecesarios * 100.0) / bytesReservadosActuales;
}
void redimensionarTablero(unsigned char*& datos, int filasViejas, int columnasViejas, int filasNuevas, int columnasNuevas){
    int BytesNuevos = calcularBytesNecesarios( filasNuevas * columnasNuevas);
    unsigned char* BloqueNuevo = new unsigned char[BytesNuevos];
    for (int i = 0; i < BytesNuevos; i++) {
        BloqueNuevo[i] = 0;
    }
    int LimiteFilas;
    int LimiteColumnas;
    if (filasViejas < filasNuevas) {
        LimiteFilas = filasViejas;
    }
    else {
        LimiteFilas = filasNuevas;
    }
    if (columnasViejas < columnasNuevas) {
        LimiteColumnas = columnasViejas;
    }
    else {
        LimiteColumnas = columnasNuevas;
    }
    int f = 0;
    while (f < LimiteFilas){
        int c = 0;
        while (c < LimiteColumnas){
            int indiceViejo = calcularIndice(f, c, columnasViejas);
            unsigned char ficha = obtenerFicha(datos, indiceViejo);
            int indiceNuevo = calcularIndice(f, c, columnasNuevas);
            establecerFicha(BloqueNuevo, indiceNuevo, ficha);

            c++;
        }
         f++;
    }
    delete[] datos;
    datos = BloqueNuevo;
}

void agregarFila(unsigned char*& datos, int& filas, int columnas, int posicion , int& bytesReservadosActuales){
    int filasViejas = filas;
    int filasNuevas = filas + 1;
    int bytesNecesarios = calcularBytesNecesarios(filasNuevas * columnas);
    if (bytesNecesarios > bytesReservadosActuales) {
        redimensionarTablero(datos, filasViejas, columnas, filasNuevas, columnas);
        bytesReservadosActuales = bytesNecesarios;
    }
    filas = filasNuevas;
    if (posicion == filasViejas){

    }
    if (posicion<filasViejas){
        int f = filasViejas -  1;
        while (f >= posicion) {
        int c = 0 ;
        while ( c < columnas){
          int indiceOrigen = calcularIndice(f, c, columnas);
          unsigned char ficha = obtenerFicha(datos, indiceOrigen);
          int indiceDestino = calcularIndice(f + 1, c, columnas);
          establecerFicha(datos, indiceDestino, ficha);
          c++;
          }
         f--;
          }
     }
   int c = 0;
    while (c < columnas) {
        int indice = calcularIndice(posicion, c, columnas);
        unsigned char nuevaFicha = 1;
        establecerFicha(datos, indice, nuevaFicha);
        c++;
    }
 }



void eliminarColumna(unsigned char*& datos, int filas, int& columnas, int posicion,int& bytesReservadosActuales){
    int columnasViejas = columnas;
    int columnasNuevas = columnas - 1;
    int f = 0;
    while(f < filas){
        int c = 0;
        while(c < columnasViejas){
            if (c == posicion) {
                c++;
                continue;

            int columnaNueva;
            if (c < posicion) {
                columnaNueva = c;
            }
            else
            {
                columnaNueva = c - 1;
            }
            int indiceViejo = calcularIndice(f, c, columnasViejas);
            unsigned char ficha = obtenerFicha(datos, indiceViejo);

            int indiceNuevo = calcularIndice(f, columnaNueva, columnasNuevas);
            establecerFicha(datos, indiceNuevo, ficha);
            c++;
        }
         f++;

    }
}
     columnas = columnasNuevas;
     int porcentajeUso = calcularPorcentajeUso(filas,columnas, bytesReservadosActuales);
     if (porcentajeUso< 65 ){
         redimensionarTablero(datos, filas, columnas, filas, columnas);
         int calculo = calcularBytesNecesarios(filas * columnas);
         bytesReservadosActuales = calculo;
     }
     else{

     }

}