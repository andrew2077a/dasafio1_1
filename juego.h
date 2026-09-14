#ifndef JUEGO_H
#define JUEGO_H
bool eliminarFichaUsuario(unsigned char* datos, int filas, int columnas, int fila, int columna);
int detectarCombinacionesVerticales(unsigned char* datos, int filas, int columnas, unsigned char* marcas);
int calcularPuntuacion(int fichasEliminadas, int cascadas);
void generarFichasIniciales(unsigned char* datos, int filas, int columnas);
int resolverCombinaciones(unsigned char* datos, int filas, int columnas);
#endif // JUEGO_H

