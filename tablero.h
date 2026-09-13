#ifndef TABLERO_H
#define TABLERO_H

double calcularPorcentajeUso(int filas, int columnas, int bytesReservadosActuales);
void redimensionarTablero(unsigned char*& datos, int filasViejas, int columnasViejas, int filasNuevas, int columnasNuevas);
void agregarFila(unsigned char*& datos, int& filas, int columnas, int posicion);
void eliminarColumna(unsigned char*& datos, int filas, int& columnas, int posicion,int& bytesReservadosActuales);
#endif // TABLERO_H
