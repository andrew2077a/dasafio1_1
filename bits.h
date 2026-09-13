#ifndef BITS_H
#define BITS_H
int calcularBytesNecesarios(int posiciones);
int calcularDesplazamientoBit(int indicePosicion);
int calcularIndice(int fila, int columna, int columnas);
unsigned char obtenerFicha(unsigned char* datos, int indicePosicion);
void establecerFicha(unsigned char* datos, int indicePosicion, unsigned char valor);
#endif // BITS_H