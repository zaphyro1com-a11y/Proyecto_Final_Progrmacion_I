#include <stdio.h>
#include <string.h>
#include "monitoreo.h"
#include "estadisticas.h"
#include "limites.h"

/**
 * Determina nivel de alerta basado en desviacion porcentual
 */
static const char* determinarNivelAlerta(float desviacion) {
    if (desviacion >= EXCESO_CRITICA) return "CRITICA";
    if (desviacion >= EXCESO_MODERADA) return "MODERADA";
    return "NORMAL";
}

/**
 * Procesa estado de un contaminante
 */
static void procesarEstadoContaminante(float valor_actual, float limite,
                                       EstadoContaminante* estado) {
    if (estado == NULL) return;
    
    estado->valor_actual = valor_actual;
    estado->limite = limite;
    estado->desviacion_porcentual = calcularDesviacionPorcentual(valor_actual, limite);
    estado->supera_limite = (valor_actual > limite) ? 1 : 0;
    estado->nivel_alerta = determinarNivelAlerta(estado->desviacion_porcentual);
}

/**
 * Procesa monitoreo actual de una zona
 */
void procesarMonitoreoActual(const ZonaUrbana* zona, DiagnosticoZona* diagnostico) {
    if (zona == NULL || diagnostico == NULL) return;
    
    diagnostico->nombre_zona = zona->nombre;
    
    /* Procesar cada contaminante */
    procesarEstadoContaminante(zona->actual.co, LIMITE_CO, &diagnostico->co);
    procesarEstadoContaminante(zona->actual.so2, LIMITE_SO2, &diagnostico->so2);
    procesarEstadoContaminante(zona->actual.no2, LIMITE_NO2, &diagnostico->no2);
    procesarEstadoContaminante(zona->actual.pm25, LIMITE_PM25, &diagnostico->pm25);
    
    /* Contar alertas activas */
    diagnostico->alertas_activas = 0;
    if (diagnostico->co.supera_limite) diagnostico->alertas_activas++;
    if (diagnostico->so2.supera_limite) diagnostico->alertas_activas++;
    if (diagnostico->no2.supera_limite) diagnostico->alertas_activas++;
    if (diagnostico->pm25.supera_limite) diagnostico->alertas_activas++;
}

/**
 * Muestra diagnostico de una zona
 */
void mostrarDiagnosticoZona(const DiagnosticoZona* diagnostico) {
    if (diagnostico == NULL) return;
    
    printf("\n┌─────────────────────────────────────────────────────────────┐\n");
    printf("│ DIAGNOSTICO: %s\n", diagnostico->nombre_zona);
    printf("├─────────────────────────────────────────────────────────────┤\n");
    
    printf("│ CO    : %7.2f | Limite: %6.2f | Desv: %+7.1f%% | %s\n",
        diagnostico->co.valor_actual, diagnostico->co.limite,
        diagnostico->co.desviacion_porcentual, diagnostico->co.nivel_alerta);
    
    printf("│ SO2   : %7.2f | Limite: %6.2f | Desv: %+7.1f%% | %s\n",
        diagnostico->so2.valor_actual, diagnostico->so2.limite,
        diagnostico->so2.desviacion_porcentual, diagnostico->so2.nivel_alerta);
    
    printf("│ NO2   : %7.2f | Limite: %6.2f | Desv: %+7.1f%% | %s\n",
        diagnostico->no2.valor_actual, diagnostico->no2.limite,
        diagnostico->no2.desviacion_porcentual, diagnostico->no2.nivel_alerta);
    
    printf("│ PM2.5 : %7.2f | Limite: %6.2f | Desv: %+7.1f%% | %s\n",
        diagnostico->pm25.valor_actual, diagnostico->pm25.limite,
        diagnostico->pm25.desviacion_porcentual, diagnostico->pm25.nivel_alerta);
    
    printf("├─────────────────────────────────────────────────────────────┤\n");
    
    if (diagnostico->alertas_activas == 0) {
        printf("│ Estado General: NORMAL - Todos los contaminantes dentro de limites ✓\n");
    } else {
        printf("│ Estado General: %d ALERTA(S) ACTIVA(S) - Revisar contaminantes\n",
            diagnostico->alertas_activas);
    }
    
    printf("└─────────────────────────────────────────────────────────────┘\n");
}

/**
 * Muestra monitoreo de todas las zonas
 */
void mostrarMonitoreoTodasLasZonas(const ZonaUrbana* zonas, int cantidad) {
    int i, total_alertas = 0;
    DiagnosticoZona diagnostico;
    
    if (zonas == NULL || cantidad <= 0) return;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          MONITOREO INTEGRAL DE CONTAMINACION             ║\n");
    printf("║                   QUITO, ECUADOR                         ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    /* Procesar y mostrar cada zona */
    for (i = 0; i < cantidad; i++) {
        procesarMonitoreoActual(&zonas[i], &diagnostico);
        mostrarDiagnosticoZona(&diagnostico);
        total_alertas += diagnostico.alertas_activas;
    }
    
    /* Resumen general */
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║ RESUMEN: %d Zonas monitoreadas | %d Total de alertas activas\n",
        cantidad, total_alertas);
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}
