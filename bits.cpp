#include "bits.h"

int calcularBytesNecesarios(int posiciones){
    return ((posiciones*3)+7)/8; // bits totales /8, redondeado hacia arriba
}

int calcularDesplazamientoBit(int indicePosicion){
    return (indicePosicion*3); // cada ficha ocupa 3 bits
}

int calcularIndice(int fila, int columna, int columnas){
    return((fila*columnas) + columna); // (fila,columna) -> índice 1D
}

unsigned char obtenerFicha(unsigned char* datos, int indicePosicion) {
    int despla, posibtye, posicionDentroBtye;
    unsigned char ficha;
    unsigned char parte1, parte2;

    despla = calcularDesplazamientoBit(indicePosicion);
    posibtye = despla / 8;
    posicionDentroBtye = despla - (posibtye * 8);

    if (posicionDentroBtye == 6) { // ficha partida: 2 bits aquí + 1 en el siguiente byte
        parte1 = (datos[posibtye] & 0b11000000) >> 6;
        parte2 = (datos[posibtye + 1] & 0b00000001) << 2;
        ficha = parte1 | parte2;
    }
    else if (posicionDentroBtye == 7) { // ficha partida: 1 bit aquí + 2 en el siguiente byte
        parte1 = (datos[posibtye] & 0b10000000) >> 7;
        parte2 = (datos[posibtye + 1] & 0b00000011) << 1;
        ficha = parte1 | parte2;
    }
    else { // ficha completa en un solo byte
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

    if (posicionDentroBtye == 6) { // ficha partida: 2 bits aquí + 1 en el siguiente byte
        datos[posibtye] = datos[posibtye] & (unsigned char)0b00111111;
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << 6);
        datos[posibtye + 1] = datos[posibtye + 1] & (unsigned char)0b11111110;
        datos[posibtye + 1] = datos[posibtye + 1] | (unsigned char)(valor >> 2);
    }
    else if (posicionDentroBtye == 7) { // ficha partida: 1 bit aquí + 2 en el siguiente byte
        datos[posibtye] = datos[posibtye] & (unsigned char)0b01111111;
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << 7);
        datos[posibtye + 1] = datos[posibtye + 1] & (unsigned char)0b11111100;
        datos[posibtye + 1] = datos[posibtye + 1] | (unsigned char)(valor >> 1);
    }
    else { // ficha completa en un solo byte
        unsigned char mascaraFicha = (unsigned char)(0b00000111 << posicionDentroBtye);
        datos[posibtye] = datos[posibtye] & (unsigned char)(~mascaraFicha);
        datos[posibtye] = datos[posibtye] | (unsigned char)(valor << posicionDentroBtye);
    }
}