#include "bits.h"
using namespace std;

int calcularBytesNecesarios(const int posiciones){
    return ((posiciones*3)+7)/8;
}
int calcularDesplazamientoBit(const int indicePosicion){
    return (indicePosicion*3);
}

int calcularIndice(const int fila,const int columna,const int columnas ){
    return((fila*columnas) + columna);
}
unsigned char obtenerFicha(unsigned char* datos, int indicePosicion) {
    int despla, posibtye, posicionDentroBtye;
    unsigned char ficha;
    unsigned char parte1, parte2;

    despla = calcularDesplazamientoBit(indicePosicion);
    posibtye = despla / 8;
    posicionDentroBtye = despla - (posibtye * 8);

    if (posicionDentroBtye == 6) {
        parte1 = (datos[posibtye] & 0b11000000) >> 6;
        parte2 = (datos[posibtye + 1] & 0b00000001) << 2;
        ficha = parte1 | parte2;
    }
    else if (posicionDentroBtye == 7) {
        parte1 = (datos[posibtye] & 0b10000000) >> 7;
        parte2 = (datos[posibtye + 1] & 0b00000011) << 1;
        ficha = parte1 | parte2;
    }
    else {
        unsigned char mascara = 0b00000111 << posicionDentroBtye;
        ficha = (datos[posibtye] & mascara) >> posicionDentroBtye;
    }

    return ficha;
}

void establecerFicha(unsigned char* datos, int indicePosicion, unsigned char valor){
    int despla, posibtye, posicionDentroBtye;

    despla = calcularDesplazamientoBit(indicePosicion);
    posibtye = despla / 8;
    posicionDentroBtye = despla - (posibtye * 8);

    if (posicionDentroBtye == 6) {
        // Se reemplazan las 2 máscaras fijas por (unsigned char) para evitar
        // que C++ las promueva a int sin necesidad; en un unsigned char no cambia
        // el resultado, pero deja explícito que se está trabajando en 8 bits.
        datos[posibtye] = datos[posibtye] & (unsigned char)0b00111111;
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << 6);
        datos[posibtye + 1] = datos[posibtye + 1] & (unsigned char)0b11111110;
        datos[posibtye + 1] = datos[posibtye + 1] | (unsigned char)(valor >> 2);
    }
    else if (posicionDentroBtye == 7) {
        // Mismo ajuste de cast que en el caso anterior, por consistencia.
        datos[posibtye] = datos[posibtye] & (unsigned char)0b01111111;
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << 7);
        datos[posibtye + 1] = datos[posibtye + 1] & (unsigned char)0b11111100;
        datos[posibtye + 1] = datos[posibtye + 1] | (unsigned char)(valor >> 1);
    }
    else {
        // Se calcula la máscara UNA sola vez y se guarda en una variable,
        // en vez de recalcular "mascara1 << posicionDentroBtye" dos veces
        // (antes se repetía el corrimiento implícitamente al usarlo solo una vez,
        // pero aquí se deja explícito y cacheado para que quede claro que es
        // un solo cálculo reutilizado, no una operación repetida).
        unsigned char mascaraFicha = (unsigned char)(0b00000111 << posicionDentroBtye);
        datos[posibtye] = datos[posibtye] & (unsigned char)(~mascaraFicha);
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << posicionDentroBtye);
    }
}