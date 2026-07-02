#ifndef REPORTES_H
#define REPORTES_H

#include "zonas.h"

/**
 * Genera reporte consolidado de todas las zonas
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 * @param incluir_predicciones Si 1, incluir predicciones
 * @param incluir_alertas Si 1, incluir alertas
 * @return 0 si exito, -1 si error
 */
int generarReporteConsolidado(const ZonaUrbana* zonas, int cantidad, 
                               int incluir_predicciones, int incluir_alertas);

/**
 * Exporta datos de una zona a un archivo
 * @param zona Puntero a ZonaUrbana
 * @param archivo Puntero a FILE abierto
 */
void exportarZonaAArchivo(const ZonaUrbana* zona, FILE* archivo);

#endif
