#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "reportes.h"
#include "limites.h"
#include "persistencia.h"

/**
 * Calcula promedio de un historico
 */
static float calcularPromedio(const float* historico, int dias) {
    int i;
    float suma = 0;
    if (historico == NULL || dias <= 0) return 0;
    
    for (i = 0; i < dias; i++) {
        suma += historico[i];
    }
    return suma / dias;
}

/**
 * Calcula desviacion porcentual respecto a un limite
 */
static float calcularDesviacionPorcentual(float valor, float limite) {
    if (limite == 0) return 0;
    return ((valor - limite) / limite) * 100.0f;
}

/**
 * Determina nivel de alerta basado en desviacion
 */
static const char* determinarNivelAlerta(float desviacion) {
    if (desviacion >= EXCESO_CRITICA) return "CRITICA";
    if (desviacion >= EXCESO_MODERADA) return "MODERADA";
    return "NORMAL";
}

/**
 * Exporta datos de una zona a un archivo
 */
void exportarZonaAArchivo(const ZonaUrbana* zona, FILE* archivo) {
    float promedio_co, promedio_so2, promedio_no2, promedio_pm25;
    float desv_co, desv_so2, desv_no2, desv_pm25;
    
    if (zona == NULL || archivo == NULL) return;
    
    /* Calcular promedios historicos */
    promedio_co = calcularPromedio((const float*)&zona->historico[0].co, DIAS_HISTORICO);
    promedio_so2 = calcularPromedio((const float*)&zona->historico[0].so2, DIAS_HISTORICO);
    promedio_no2 = calcularPromedio((const float*)&zona->historico[0].no2, DIAS_HISTORICO);
    promedio_pm25 = calcularPromedio((const float*)&zona->historico[0].pm25, DIAS_HISTORICO);
    
    /* Calcular desviaciones */
    desv_co = calcularDesviacionPorcentual(zona->actual.co, LIMITE_CO);
    desv_so2 = calcularDesviacionPorcentual(zona->actual.so2, LIMITE_SO2);
    desv_no2 = calcularDesviacionPorcentual(zona->actual.no2, LIMITE_NO2);
    desv_pm25 = calcularDesviacionPorcentual(zona->actual.pm25, LIMITE_PM25);
    
    /* Encabezado de zona */
    fprintf(archivo, "\n╔════════════════════════════════════════════════════════════╗\n");
    fprintf(archivo, "║ ZONA: %-55s ║\n", zona->nombre);
    fprintf(archivo, "║ ID: %d%-56s ║\n", zona->id, "");
    fprintf(archivo, "╚════════════════════════════════════════════════════════════╝\n\n");
    
    /* Condiciones Climaticas */
    fprintf(archivo, "CONDICIONES CLIMATICAS ACTUALES:\n");
    fprintf(archivo, "  Temperatura:      %.1f C\n", zona->condiciones_clima.temperatura);
    fprintf(archivo, "  Velocidad viento: %.1f km/h\n", zona->condiciones_clima.velocidad_viento);
    fprintf(archivo, "  Humedad relativa: %.1f %%\n\n", zona->condiciones_clima.humedad);
    
    /* Contaminantes Actuales */
    fprintf(archivo, "CONTAMINANTES ACTUALES:\n");
    fprintf(archivo, "  CO:    %7.2f mg/m3 | Limite: %6.2f | Desv: %+7.1f%% | Estado: %s\n",
        zona->actual.co, LIMITE_CO, desv_co, determinarNivelAlerta(desv_co));
    fprintf(archivo, "  SO2:   %7.2f ug/m3 | Limite: %6.2f | Desv: %+7.1f%% | Estado: %s\n",
        zona->actual.so2, LIMITE_SO2, desv_so2, determinarNivelAlerta(desv_so2));
    fprintf(archivo, "  NO2:   %7.2f ug/m3 | Limite: %6.2f | Desv: %+7.1f%% | Estado: %s\n",
        zona->actual.no2, LIMITE_NO2, desv_no2, determinarNivelAlerta(desv_no2));
    fprintf(archivo, "  PM2.5: %7.2f ug/m3 | Limite: %6.2f | Desv: %+7.1f%% | Estado: %s\n\n",
        zona->actual.pm25, LIMITE_PM25, desv_pm25, determinarNivelAlerta(desv_pm25));
    
    /* Promedios de 30 dias */
    fprintf(archivo, "PROMEDIOS HISTORICOS (30 DIAS):\n");
    fprintf(archivo, "  CO:    %.2f mg/m3\n", promedio_co);
    fprintf(archivo, "  SO2:   %.2f ug/m3\n", promedio_so2);
    fprintf(archivo, "  NO2:   %.2f ug/m3\n", promedio_no2);
    fprintf(archivo, "  PM2.5: %.2f ug/m3\n\n", promedio_pm25);
    
    /* Predicciones */
    if (zona->prediccion.co > 0 || zona->prediccion.so2 > 0 || 
        zona->prediccion.no2 > 0 || zona->prediccion.pm25 > 0) {
        fprintf(archivo, "PREDICCION (24 HORAS SIGUIENTES):\n");
        fprintf(archivo, "  CO:    %.2f mg/m3\n", zona->prediccion.co);
        fprintf(archivo, "  SO2:   %.2f ug/m3\n", zona->prediccion.so2);
        fprintf(archivo, "  NO2:   %.2f ug/m3\n", zona->prediccion.no2);
        fprintf(archivo, "  PM2.5: %.2f ug/m3\n\n", zona->prediccion.pm25);
    }
    
    fprintf(archivo, "════════════════════════════════════════════════════════════\n");
}

/**
 * Muestra estadisticas de una zona en consola
 */
/**
 * Genera reporte consolidado de todas las zonas
 */
int generarReporteConsolidado(const ZonaUrbana* zonas, int cantidad, 
                               int incluir_predicciones, int incluir_alertas) {
    FILE* archivo;
    int i;
    
    if (zonas == NULL || cantidad <= 0) return -1;
    
    (void)incluir_predicciones;  /* Evitar warning de parametro no usado */
    (void)incluir_alertas;
    
    /* Crear directorio */
    crearDirectorioDatos();
    
    /* Abrir archivo para escribir */
    archivo = fopen(ARCHIVO_REPORTE, "w");
    if (archivo == NULL) {
        fprintf(stderr, "[ERROR] No se puede crear archivo: %s\n", ARCHIVO_REPORTE);
        return -1;
    }
    
    /* Encabezado */
    fprintf(archivo, "╔═══════════════════════════════════════════════════════════╗\n");
    fprintf(archivo, "║   SISTEMA DE MONITOREO AMBIENTAL - QUITO, ECUADOR         ║\n");
    fprintf(archivo, "║     Reporte Consolidado de Contaminacion del Aire         ║\n");
    fprintf(archivo, "╚═══════════════════════════════════════════════════════════╝\n\n");
    
    fprintf(archivo, "Fechas del reporte:\n");
    fprintf(archivo, "  Generado: [dinamico en tiempo de ejecucion]\n");
    fprintf(archivo, "  Limites OMS 2021 (valores mas restrictivos):\n");
    fprintf(archivo, "    - CO:    %.2f mg/m3 (8h)\n", LIMITE_CO);
    fprintf(archivo, "    - SO2:   %.2f ug/m3 (24h)\n", LIMITE_SO2);
    fprintf(archivo, "    - NO2:   %.2f ug/m3 (anual)\n", LIMITE_NO2);
    fprintf(archivo, "    - PM2.5: %.2f ug/m3 (anual)\n\n", LIMITE_PM25);
    
    /* Resumen de zonas */
    fprintf(archivo, "RESUMEN DE ZONAS MONITOREADAS:\n");
    fprintf(archivo, "Total de zonas: %d\n\n", cantidad);
    
    /* Detalles de cada zona */
    for (i = 0; i < cantidad; i++) {
        exportarZonaAArchivo(&zonas[i], archivo);
    }
    
    /* Pie del reporte */
    fprintf(archivo, "\n╔═══════════════════════════════════════════════════════════╗\n");
    fprintf(archivo, "║                    FIN DEL REPORTE                        ║\n");
    fprintf(archivo, "║              Para consultas, contactar a:                 ║\n");
    fprintf(archivo, "║        Sistema de Gestion Ambiental - Quito 2026          ║\n");
    fprintf(archivo, "╚═══════════════════════════════════════════════════════════╝\n");
    
    fclose(archivo);
    printf("[*] Reporte generado exitosamente: %s\n", ARCHIVO_REPORTE);
    return 0;
}
