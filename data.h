#ifndef DATA_H
#define DATA_H

#include "zonas.h"

/**
 * Inicializa las 5 parroquias de Quito con datos simulados
 * @param zonas Array de 5 ZonaUrbana
 * @param cantidad Numero de zonas (debe ser >= 5)
 */
void inicializarZonasQuito(ZonaUrbana* zonas, int cantidad);

/**
 * Genera valores aleatorios realistas para historicos
 * @param zona Puntero a ZonaUrbana a llenar
 */
void generarHistoricoSimulado(ZonaUrbana* zona);

/**
 * Asigna condiciones climaticas realistas por zona
 * @param zona Puntero a ZonaUrbana
 */
void asignarCondicionesClimaticas(ZonaUrbana* zona);

/**
 * Imprime un resumen de los datos inicializados
 * @param zonas Array de zonas
 * @param cantidad Numero de zonas
 */
void mostrarResumenDatos(const ZonaUrbana* zonas, int cantidad);

#endif
