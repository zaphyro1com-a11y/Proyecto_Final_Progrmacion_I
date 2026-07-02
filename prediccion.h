#ifndef PREDICCION_H
#define PREDICCION_H

#include "zonas.h"

/**
 * Calcula prediccion de contaminantes para las proximas 24 horas
 * Utiliza promedio ponderado de 30 dias con modificadores climaticos
 * @param zona Puntero a ZonaUrbana con historico y condiciones
 * @return 0 si exito, -1 si error
 */
int ejecutarPrediccionPonderada(ZonaUrbana* zona);

/**
 * Calcula promedio ponderado de un historico
 * Pesos lineales: dia 1 = peso 1, dia 30 = peso 30
 * @param historico Arreglo de valores historicos (30 elementos)
 * @return Promedio ponderado
 */
float calcularPromedioPonderado(const float* historico);

/**
 * Aplica modificadores climaticos a una prediccion
 * @param prediccion Puntero a valor de prediccion (sera modificado)
 * @param tipo_contaminante 0=CO, 1=SO2, 2=NO2, 3=PM25
 * @param viento Velocidad del viento en km/h
 * @param humedad Humedad relativa en %
 */
void aplicarModificadoresClimaticos(float* prediccion, int tipo_contaminante,
                                     float viento, float humedad);

/**
 * Muestra predicciones de una zona
 * @param zona Puntero a ZonaUrbana
 */
void mostrarPrediccionesZona(const ZonaUrbana* zona);

/**
 * Muestra predicciones de todas las zonas
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 */
void mostrarPrediccionesTodasLasZonas(const ZonaUrbana* zonas, int cantidad);

#endif
