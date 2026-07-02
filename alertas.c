#include <stdio.h>
#include <string.h>
#include "alertas.h"
#include "limites.h"

/**
 * Determina nivel de alerta basado en desviacion
 * NORMAL: <= 0% (dentro del limite)
 * MODERADA: 0% < desv <= 30%
 * CRITICA: desv > 30%
 */
int determinarTipoAlerta(float desviacion_porcentaje) {
    if (desviacion_porcentaje > EXCESO_CRITICA * 100.0) {
        return 2; /* CRITICA */
    } else if (desviacion_porcentaje > EXCESO_MODERADA * 100.0) {
        return 1; /* MODERADA */
    }
    return 0; /* NORMAL */
}

/**
 * Genera recomendacion de mitigacion
 */
void generarRecomendacion(TipoAlerta* alerta, int contaminante, int tipo) {
    if (alerta == NULL) return;
    
    if (tipo == 0) {
        strcpy(alerta->recomendacion, "Sin recomendaciones especiales");
        return;
    }
    
    switch (contaminante) {
        case 0: /* CO */
            if (tipo == 1) {
                strcpy(alerta->recomendacion,
                    "MODERADO: Aumentar vigilancia de emision vehicular");
            } else {
                strcpy(alerta->recomendacion,
                    "CRITICO: RESTRICCION INMEDIATA de transporte pesado y circulacion");
            }
            break;
            
        case 1: /* SO2 */
            if (tipo == 1) {
                strcpy(alerta->recomendacion,
                    "MODERADO: Inspeccionar industrias con chimeneas altas");
            } else {
                strcpy(alerta->recomendacion,
                    "CRITICO: CIERRE INMEDIATO de industrias contaminantes");
            }
            break;
            
        case 2: /* NO2 */
            if (tipo == 1) {
                strcpy(alerta->recomendacion,
                    "MODERADO: Limitar actividades al aire libre en horas pico");
            } else {
                strcpy(alerta->recomendacion,
                    "CRITICO: SUSPENSION de actividades deportivas al aire libre");
            }
            break;
            
        case 3: /* PM2.5 */
            if (tipo == 1) {
                strcpy(alerta->recomendacion,
                    "MODERADO: Recomendar uso de mascarillas N95 para poblacion vulnerable");
            } else {
                strcpy(alerta->recomendacion,
                    "CRITICO: CIERRE de escuelas/espacios publicos - Evacuacion recomendada");
            }
            break;
            
        default:
            strcpy(alerta->recomendacion, "Desconocido");
    }
}

/**
 * Calcula desviacion porcentual
 */
static float calcularDesviacion(float valor, float limite) {
    if (limite <= 0.0) return 0.0;
    return ((valor - limite) / limite) * 100.0;
}

/**
 * Evalua alertas de una zona
 */
int evaluarAlertasZona(const ZonaUrbana* zona, TipoAlerta* alertas) {
    int contador = 0;
    int tipo_alerta;
    
    if (zona == NULL || alertas == NULL) return 0;
    
    /* Evaluar CO */
    alertas[contador].desviacion_porcentaje = calcularDesviacion(zona->actual.co, LIMITE_CO);
    tipo_alerta = determinarTipoAlerta(alertas[contador].desviacion_porcentaje);
    alertas[contador].tipo = tipo_alerta;
    strcpy(alertas[contador].tipo_nombre, 
        (tipo_alerta == 0) ? "NORMAL" : (tipo_alerta == 1) ? "MODERADA" : "CRITICA");
    strcpy(alertas[contador].contaminante, "CO");
    alertas[contador].valor_actual = zona->actual.co;
    alertas[contador].limite_oms = LIMITE_CO;
    generarRecomendacion(&alertas[contador], 0, tipo_alerta);
    if (tipo_alerta > 0) contador++;
    
    /* Evaluar SO2 */
    alertas[contador].desviacion_porcentaje = calcularDesviacion(zona->actual.so2, LIMITE_SO2);
    tipo_alerta = determinarTipoAlerta(alertas[contador].desviacion_porcentaje);
    alertas[contador].tipo = tipo_alerta;
    strcpy(alertas[contador].tipo_nombre,
        (tipo_alerta == 0) ? "NORMAL" : (tipo_alerta == 1) ? "MODERADA" : "CRITICA");
    strcpy(alertas[contador].contaminante, "SO2");
    alertas[contador].valor_actual = zona->actual.so2;
    alertas[contador].limite_oms = LIMITE_SO2;
    generarRecomendacion(&alertas[contador], 1, tipo_alerta);
    if (tipo_alerta > 0) contador++;
    
    /* Evaluar NO2 */
    alertas[contador].desviacion_porcentaje = calcularDesviacion(zona->actual.no2, LIMITE_NO2);
    tipo_alerta = determinarTipoAlerta(alertas[contador].desviacion_porcentaje);
    alertas[contador].tipo = tipo_alerta;
    strcpy(alertas[contador].tipo_nombre,
        (tipo_alerta == 0) ? "NORMAL" : (tipo_alerta == 1) ? "MODERADA" : "CRITICA");
    strcpy(alertas[contador].contaminante, "NO2");
    alertas[contador].valor_actual = zona->actual.no2;
    alertas[contador].limite_oms = LIMITE_NO2;
    generarRecomendacion(&alertas[contador], 2, tipo_alerta);
    if (tipo_alerta > 0) contador++;
    
    /* Evaluar PM2.5 */
    alertas[contador].desviacion_porcentaje = calcularDesviacion(zona->actual.pm25, LIMITE_PM25);
    tipo_alerta = determinarTipoAlerta(alertas[contador].desviacion_porcentaje);
    alertas[contador].tipo = tipo_alerta;
    strcpy(alertas[contador].tipo_nombre,
        (tipo_alerta == 0) ? "NORMAL" : (tipo_alerta == 1) ? "MODERADA" : "CRITICA");
    strcpy(alertas[contador].contaminante, "PM2.5");
    alertas[contador].valor_actual = zona->actual.pm25;
    alertas[contador].limite_oms = LIMITE_PM25;
    generarRecomendacion(&alertas[contador], 3, tipo_alerta);
    if (tipo_alerta > 0) contador++;
    
    return contador;
}

/**
 * Muestra alertas y recomendaciones de una zona
 */
void mostrarAlertasZona(const ZonaUrbana* zona) {
    TipoAlerta alertas[4];
    int num_alertas;
    int i;
    
    if (zona == NULL) return;
    
    printf("\n%s - ALERTAS Y RECOMENDACIONES\n", zona->nombre);
    printf("----------------------------------------------\n");
    
    num_alertas = evaluarAlertasZona(zona, alertas);
    
    if (num_alertas == 0) {
        printf("  [OK] SIN ALERTAS - Condiciones dentro de limites OMS\n");
    } else {
        for (i = 0; i < num_alertas; i++) {
            printf("  [%s] %s (%6.2f / %6.2f) %+6.1f%%\n",
                alertas[i].tipo_nombre,
                alertas[i].contaminante,
                alertas[i].valor_actual,
                alertas[i].limite_oms,
                alertas[i].desviacion_porcentaje);
            printf("      %s\n", alertas[i].recomendacion);
        }
    }
}

/**
 * Muestra alertas para todas las zonas
 */
int mostrarAlertasTodas(const ZonaUrbana* zonas, int cantidad) {
    int i;
    int total_alertas = 0;
    
    if (zonas == NULL) return 0;
    
    printf("\n");
    printf(" ================================================================ \n");
    printf("||      SISTEMA DE ALERTAS Y RECOMENDACIONES DE MITIGACION      ||\n");
    printf("||                 QUITO, ECUADOR                               ||\n");
    printf(" ================================================================ \n");
    
    for (i = 0; i < cantidad; i++) {
        int zona_alertas = evaluarAlertasZona(&zonas[i], NULL);
        total_alertas += zona_alertas;
        mostrarAlertasZona(&zonas[i]);
    }
    
    printf("\n");
    printf(" ===============================================================\n");
    printf("|| RESUMEN: %d Total de alertas activas en el sistema          ||\n", total_alertas);
    printf(" ===============================================================\n");
    
    return total_alertas;
}
