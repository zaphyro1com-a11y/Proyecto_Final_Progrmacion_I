#ifndef ALERTAS_H
#define ALERTAS_H

#include "zonas.h"

/**
 * Estructura para una alerta con recomendaciones
 */
typedef struct {
    int tipo;           /* 0=NORMAL, 1=MODERADA, 2=CRITICA */
    char tipo_nombre[20];
    char contaminante[10];
    float valor_actual;
    float limite_oms;
    float desviacion_porcentaje;
    char recomendacion[256];
} TipoAlerta;

/**
 * Genera recomendacion mitigation basada en contaminante y severidad
 * @param alerta Puntero a estructura TipoAlerta
 * @param contaminante 0=CO, 1=SO2, 2=NO2, 3=PM25
 * @param tipo 0=NORMAL, 1=MODERADA, 2=CRITICA
 */
void generarRecomendacion(TipoAlerta* alerta, int contaminante, int tipo);

/**
 * Evalua alertas y genera recomendaciones para una zona
 * @param zona Puntero a ZonaUrbana
 * @param alertas Array de TipoAlerta para almacenar resultados (minimo 4 elementos)
 * @return Numero de alertas generadas
 */
int evaluarAlertasZona(const ZonaUrbana* zona, TipoAlerta* alertas);

/**
 * Muestra alertas y recomendaciones para una zona
 * @param zona Puntero a ZonaUrbana
 */
void mostrarAlertasZona(const ZonaUrbana* zona);

/**
 * Muestra alertas y recomendaciones para todas las zonas
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 * @return Total de alertas en el sistema
 */
int mostrarAlertasTodas(const ZonaUrbana* zonas, int cantidad);

/**
 * Determina nivel de alerta (0=NORMAL, 1=MODERADA, 2=CRITICA)
 * @param desviacion_porcentaje Desviacion en porcentaje
 * @return Tipo de alerta
 */
int determinarTipoAlerta(float desviacion_porcentaje);

#endif
