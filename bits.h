#ifndef BITS_H
#define BITS_H
int calcularBytesNecesarios(const int posiciones);
int calcularDesplazamientoBit(const int indicePosicion);
int calcularIndice(const int fila,const int columna,const int columnas );
unsigned char obtenerFicha(unsigned char* datos, int indicePosicion);
void establecerFicha(unsigned char* datos, int indicePosicion, unsigned char valor);
#endif // BITS_H
