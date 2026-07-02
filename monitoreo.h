#ifndef MONITOREO_H
#define MONITOREO_H

#include "zonas.h"

/**
 * Estado de un contaminante respecto al limite
 */
typedef struct {
    float valor_actual;
    float limite;
    float desviacion_porcentual;
    int supera_limite;  /* 1 si supera, 0 si no */
    const char* nivel_alerta;  /* NORMAL, MODERADA, CRITICA */
} EstadoContaminante;

/**
 * Diagnostico completo de una zona
 */
typedef struct {
    const char* nombre_zona;
    EstadoContaminante co;
    EstadoContaminante so2;
    EstadoContaminante no2;
    EstadoContaminante pm25;
    int alertas_activas;  /* Numero de alertas activas */
} DiagnosticoZona;

/**
 * Procesa monitoreo actual de una zona
 * @param zona Puntero a ZonaUrbana
 * @param diagnostico Estructura para almacenar resultado
 */
void procesarMonitoreoActual(const ZonaUrbana* zona, DiagnosticoZona* diagnostico);

/**
 * Muestra diagnostico de una zona en consola
 * @param diagnostico DiagnosticoZona a mostrar
 */
void mostrarDiagnosticoZona(const DiagnosticoZona* diagnostico);

/**
 * Muestra diagnosticos de todas las zonas
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 */
void mostrarMonitoreoTodasLasZonas(const ZonaUrbana* zonas, int cantidad);

#endif
