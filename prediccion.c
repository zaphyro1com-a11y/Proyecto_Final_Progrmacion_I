#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "prediccion.h"
#include "limites.h"

/**
 * Calcula promedio ponderado de un historico
 * Pesos lineales: dia 1 = peso 1, dia 30 = peso 30
 */
float calcularPromedioPonderado(const float* historico) {
    int i;
    float suma_ponderada = 0.0;
    float suma_pesos = 0.0;
    
    if (historico == NULL) return 0.0;
    
    for (i = 0; i < DIAS_HISTORICO; i++) {
        float peso = (float)(i + 1); /* Pesos: 1, 2, 3, ..., 30 */
        suma_ponderada += historico[i] * peso;
        suma_pesos += peso;
    }
    
    return (suma_pesos > 0.0) ? (suma_ponderada / suma_pesos) : 0.0;
}

/**
 * Aplica modificadores climaticos a una prediccion
 * Viento > 20 km/h: reduce 15-20% (dispersión)
 * Humedad > 80% + PM2.5: aumenta 10-15%
 */
void aplicarModificadoresClimaticos(float* prediccion, int tipo_contaminante,
                                     float viento, float humedad) {
    float factor;
    
    if (prediccion == NULL) return;
    
    /* Modificador por viento (dispersión) */
    if (viento > 20.0) {
        /* Reduce 15-20% segun intensidad del viento */
        factor = 1.0 - (0.15 + 0.05 * ((viento - 20.0) / 20.0));
        if (factor < 0.8) factor = 0.8; /* Piso de 80% */
        *prediccion *= factor;
    }
    
    /* Modificador por humedad (para PM2.5 principalmente) */
    if (humedad > 80.0 && tipo_contaminante == 3) { /* tipo 3 = PM2.5 */
        /* Aumenta 10-15% segun humedad */
        factor = 1.0 + (0.10 + 0.05 * ((humedad - 80.0) / 20.0));
        if (factor > 1.15) factor = 1.15; /* Techo de 115% */
        *prediccion *= factor;
    }
}

/**
 * Calcula prediccion de contaminantes para 24h
 */
int ejecutarPrediccionPonderada(ZonaUrbana* zona) {
    if (zona == NULL) return -1;
    
    /* Calcular promedios ponderados de historicos */
    zona->prediccion.co = calcularPromedioPonderado(
        (const float*)&zona->historico[0].co);
    zona->prediccion.so2 = calcularPromedioPonderado(
        (const float*)&zona->historico[0].so2);
    zona->prediccion.no2 = calcularPromedioPonderado(
        (const float*)&zona->historico[0].no2);
    zona->prediccion.pm25 = calcularPromedioPonderado(
        (const float*)&zona->historico[0].pm25);
    
    /* Aplicar modificadores climaticos */
    aplicarModificadoresClimaticos(&zona->prediccion.co, 0,
                                    zona->condiciones_clima.velocidad_viento,
                                    zona->condiciones_clima.humedad);
    aplicarModificadoresClimaticos(&zona->prediccion.so2, 1,
                                    zona->condiciones_clima.velocidad_viento,
                                    zona->condiciones_clima.humedad);
    aplicarModificadoresClimaticos(&zona->prediccion.no2, 2,
                                    zona->condiciones_clima.velocidad_viento,
                                    zona->condiciones_clima.humedad);
    aplicarModificadoresClimaticos(&zona->prediccion.pm25, 3,
                                    zona->condiciones_clima.velocidad_viento,
                                    zona->condiciones_clima.humedad);
    
    return 0;
}

/**
 * Calcula desviacion porcentual respecto a limite OMS
 */
static float calcularDesviacion(float valor, float limite) {
    if (limite <= 0.0) return 0.0;
    return ((valor - limite) / limite) * 100.0;
}

/**
 * Muestra predicciones de una zona
 */
void mostrarPrediccionesZona(const ZonaUrbana* zona) {
    float desv_co, desv_so2, desv_no2, desv_pm25;
    
    if (zona == NULL) return;
    
    desv_co = calcularDesviacion(zona->prediccion.co, LIMITE_CO);
    desv_so2 = calcularDesviacion(zona->prediccion.so2, LIMITE_SO2);
    desv_no2 = calcularDesviacion(zona->prediccion.no2, LIMITE_NO2);
    desv_pm25 = calcularDesviacion(zona->prediccion.pm25, LIMITE_PM25);
    
    printf("\n%s - PREDICCION 24H\n", zona->nombre);
    printf("Contaminante | Prediccion | Limite  | Desviacion\n");
    printf("  CO         |    %6.2f  | %7.2f | %+7.1f%%\n",
           zona->prediccion.co, LIMITE_CO, desv_co);
    printf("  SO2        |    %6.2f  | %7.2f | %+7.1f%%\n",
           zona->prediccion.so2, LIMITE_SO2, desv_so2);
    printf("  NO2        |    %6.2f  | %7.2f | %+7.1f%%\n",
           zona->prediccion.no2, LIMITE_NO2, desv_no2);
    printf("  PM2.5      |    %6.2f  | %7.2f | %+7.1f%%\n",
           zona->prediccion.pm25, LIMITE_PM25, desv_pm25);
}

/**
 * Muestra predicciones de todas las zonas
 */
void mostrarPrediccionesTodasLasZonas(const ZonaUrbana* zonas, int cantidad) {
    int i;
    
    if (zonas == NULL) return;
    
    printf("\n");
    printf("ÔòöÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòù\n");
    printf("Ôòæ      PREDICCION DE CONTAMINACION - PROXIMAS 24H               Ôòæ\n");
    printf("Ôòæ                 QUITO, ECUADOR                          Ôòæ\n");
    printf("ÔòÜÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòØ\n");
    
    for (i = 0; i < cantidad; i++) {
        mostrarPrediccionesZona(&zonas[i]);
    }
    
    printf("\n");
}
