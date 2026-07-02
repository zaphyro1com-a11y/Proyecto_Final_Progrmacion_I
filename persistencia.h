#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "zonas.h"

/* Archivos de persistencia */
#define ARCHIVO_HISTORICO       "datos/historico_zonas.txt"
#define ARCHIVO_REPORTE         "datos/reporte_consolidado.txt"

/**
 * Carga historicos de contaminacion desde archivo
 * @param zonas Array de ZonaUrbana a llenar
 * @param cantidad Numero de zonas
 * @return 0 si exito, -1 si error
 */
int cargarHistoricoDesdeArchivo(ZonaUrbana* zonas, int cantidad);

/**
 * Guarda historicos de contaminacion en archivo
 * @param zonas Array de ZonaUrbana a guardar
 * @param cantidad Numero de zonas
 * @return 0 si exito, -1 si error
 */
int guardarHistoricoEnArchivo(const ZonaUrbana* zonas, int cantidad);

/**
 * Verifica si el archivo de historico existe
 * @return 1 si existe, 0 si no
 */
int existeArchivoHistorico(void);

/**
 * Crea directorio datos si no existe
 * @return 0 si exito, -1 si error
 */
int crearDirectorioDatos(void);

#endif
