#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include "zonas.h"

/**
 * Estadisticas de un contaminante
 */
typedef struct {
    float promedio;
    float minimo;
    float maximo;
    float desviacion_estandar;
} EstadisticasContaminante;

/**
 * Calcula promedio de un arreglo de valores
 * @param valores Arreglo de floats
 * @param cantidad Numero de elementos
 * @return Promedio calculado
 */
float calcularPromedio(const float* valores, int cantidad);

/**
 * Calcula valor minimo de un arreglo
 * @param valores Arreglo de floats
 * @param cantidad Numero de elementos
 * @return Minimo valor
 */
float calcularMinimo(const float* valores, int cantidad);

/**
 * Calcula valor maximo de un arreglo
 * @param valores Arreglo de floats
 * @param cantidad Numero de elementos
 * @return Maximo valor
 */
float calcularMaximo(const float* valores, int cantidad);

/**
 * Calcula desviacion estandar de un arreglo
 * @param valores Arreglo de floats
 * @param cantidad Numero de elementos
 * @return Desviacion estandar
 */
float calcularDesviacionEstandar(const float* valores, int cantidad);

/**
 * Calcula desviacion porcentual respecto a un limite
 * @param valor Valor actual
 * @param limite Limite de referencia
 * @return Porcentaje de desviacion
 */
float calcularDesviacionPorcentual(float valor, float limite);

/**
 * Genera estadisticas completas de un contaminante en una zona
 * @param zona Puntero a ZonaUrbana
 * @param tipo_contaminante 0=CO, 1=SO2, 2=NO2, 3=PM25
 * @param stats Estructura para almacenar resultado
 */
void calcularEstadisticasContaminante(const ZonaUrbana* zona, int tipo_contaminante,
                                      EstadisticasContaminante* stats);

/**
 * Muestra estadisticas de una zona en formato tabla
 * @param zona Puntero a ZonaUrbana
 */
void mostrarEstadisticasZona(const ZonaUrbana* zona);

#endif
