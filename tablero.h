#ifndef TABLERO_H
#define TABLERO_H

void crearTablero(unsigned char*& datos, int filas, int columnas);
void destruirTablero(unsigned char*& datos);
void redimensionarTablero(unsigned char*& datos, int filasViejas, int columnasViejas, int filasNuevas, int columnasNuevas);
double calcularPorcentajeUso(int filas, int columnas, int bytesReservadosActuales);
void mostrarTablero(unsigned char* datos, int filas, int columnas);
void mostrarTableroBinario(unsigned char* datos, int filas, int columnas);
void agregarFila(unsigned char*& datos, int& filas, int columnas, int posicion, int& bytesReservadosActuales);
void eliminarFila(unsigned char*& datos, int& filas, int columnas, int posicion, int& bytesReservados);
void agregarColumna(unsigned char*& datos, int filas, int& columnas, int posicion, int& bytesReservados);
void eliminarColumna(unsigned char*& datos, int filas, int& columnas, int posicion, int& bytesReservadosActuales);

#endif // TABLERO_H
