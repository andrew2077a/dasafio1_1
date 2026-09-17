#ifndef JUEGO_H
#define JUEGO_H
unsigned char generarFichaAleatoria();
bool eliminarFichaUsuario(unsigned char* datos, int filas, int columnas, int fila, int columna);
int detectarCombinacionesHorizontales(unsigned char* datos, int filas, int columnas, unsigned char* marcas);
int detectarCombinacionesVerticales(unsigned char* datos, int filas, int columnas, unsigned char* marcas);
int calcularPuntuacion(int fichasEliminadas, int cascadas);
void mostrarEstadoJuego(int filas, int columnas, int eliminacionesUsuario,
                        int fichasEliminadasTotal, int combinacionesDetectadas,
                        int cascadasActuales, int puntuacion);
void generarFichasIniciales(unsigned char* datos, int filas, int columnas);
int resolverCombinaciones(unsigned char* datos, int filas, int columnas, int& combosDetectados);
void reorganizarTablero(unsigned char* datos, int filas, int columnas);
int procesarCascadas(unsigned char* datos, int filas, int columnas, int& combinacionesDetectadas, int& fichasEliminadas);
void liberarMarcas();
void ImprimirMenu();
void ImprimirLogo();
void ImprimirFinal();

#endif
