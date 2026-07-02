#include <stdio.h>
#include <math.h>
#include "estadisticas.h"
#include "limites.h"
#include "zonas.h"

/**
 * Calcula promedio de un arreglo
 */
float calcularPromedio(const float* valores, int cantidad) {
    int i;
    float suma = 0;
    if (valores == NULL || cantidad <= 0) return 0;
    
    for (i = 0; i < cantidad; i++) {
        suma += valores[i];
    }
    return suma / cantidad;
}

/**
 * Calcula minimo de un arreglo
 */
float calcularMinimo(const float* valores, int cantidad) {
    int i;
    float minimo;
    if (valores == NULL || cantidad <= 0) return 0;
    
    minimo = valores[0];
    for (i = 1; i < cantidad; i++) {
        if (valores[i] < minimo) {
            minimo = valores[i];
        }
    }
    return minimo;
}

/**
 * Calcula maximo de un arreglo
 */
float calcularMaximo(const float* valores, int cantidad) {
    int i;
    float maximo;
    if (valores == NULL || cantidad <= 0) return 0;
    
    maximo = valores[0];
    for (i = 1; i < cantidad; i++) {
        if (valores[i] > maximo) {
            maximo = valores[i];
        }
    }
    return maximo;
}

/**
 * Calcula desviacion estandar de un arreglo
 */
float calcularDesviacionEstandar(const float* valores, int cantidad) {
    int i;
    float promedio, suma_cuadrados, varianza;
    if (valores == NULL || cantidad <= 1) return 0;
    
    promedio = calcularPromedio(valores, cantidad);
    suma_cuadrados = 0;
    
    for (i = 0; i < cantidad; i++) {
        float diferencia = valores[i] - promedio;
        suma_cuadrados += diferencia * diferencia;
    }
    
    varianza = suma_cuadrados / (cantidad - 1);
    return (float)sqrt(varianza);
}

/**
 * Calcula desviacion porcentual respecto a un limite
 */
float calcularDesviacionPorcentual(float valor, float limite) {
    if (limite == 0) return 0;
    return ((valor - limite) / limite) * 100.0f;
}

/**
 * Calcula estadisticas de un contaminante en una zona
 * Versión sin memoria dinámica
 */
void calcularEstadisticasContaminante(const ZonaUrbana* zona, int tipo_contaminante,
                                      EstadisticasContaminante* stats) {
    int i;
    float valor, promedio, suma_cuadrados, varianza;
    
    if (zona == NULL || stats == NULL) return;
    
    /* Inicializar */
    stats->promedio = 0;
    stats->minimo = 0;
    stats->maximo = 0;
    stats->desviacion_estandar = 0;
    
    /* Calcular promedio y min/max pasando una vez */
    stats->minimo = 999999;
    stats->maximo = -999999;
    promedio = 0;
    
    for (i = 0; i < DIAS_HISTORICO; i++) {
        /* Obtener valor segun tipo de contaminante */
        switch (tipo_contaminante) {
            case 0: valor = zona->historico[i].co; break;
            case 1: valor = zona->historico[i].so2; break;
            case 2: valor = zona->historico[i].no2; break;
            case 3: valor = zona->historico[i].pm25; break;
            default: return;
        }
        
        /* Acumular promedio */
        promedio += valor;
        
        /* Calcular min/max */
        if (valor < stats->minimo) stats->minimo = valor;
        if (valor > stats->maximo) stats->maximo = valor;
    }
    
    /* Finalizar promedio */
    stats->promedio = promedio / DIAS_HISTORICO;
    
    /* Calcular desviacion estandar en segundo pase */
    suma_cuadrados = 0;
    for (i = 0; i < DIAS_HISTORICO; i++) {
        /* Obtener valor segun tipo de contaminante */
        switch (tipo_contaminante) {
            case 0: valor = zona->historico[i].co; break;
            case 1: valor = zona->historico[i].so2; break;
            case 2: valor = zona->historico[i].no2; break;
            case 3: valor = zona->historico[i].pm25; break;
            default: return;
        }
        
        valor = valor - stats->promedio;
        suma_cuadrados += valor * valor;
    }
    
    varianza = suma_cuadrados / (DIAS_HISTORICO - 1);
    stats->desviacion_estandar = (float)sqrt(varianza);
}

/**
 * Muestra estadisticas de una zona
 */
void mostrarEstadisticasZona(const ZonaUrbana* zona) {
    EstadisticasContaminante stats_co, stats_so2, stats_no2, stats_pm25;
    
    if (zona == NULL) return;
    
    /* Calcular estadisticas para cada contaminante */
    calcularEstadisticasContaminante(zona, 0, &stats_co);
    calcularEstadisticasContaminante(zona, 1, &stats_so2);
    calcularEstadisticasContaminante(zona, 2, &stats_no2);
    calcularEstadisticasContaminante(zona, 3, &stats_pm25);
    
    printf("\nESTADISTICAS DE 30 DIAS - %s\n", zona->nombre);
    printf("┌────────┬──────────┬──────────┬──────────┬──────────────┐\n");
    printf("│ Contam.│ Promedio │  Minimo  │  Maximo  │  Desv.Est.   │\n");
    printf("├────────┼──────────┼──────────┼──────────┼──────────────┤\n");
    printf("│ CO     │ %8.2f │ %8.2f │ %8.2f │ %12.4f │\n",
        stats_co.promedio, stats_co.minimo, stats_co.maximo, stats_co.desviacion_estandar);
    printf("│ SO2    │ %8.2f │ %8.2f │ %8.2f │ %12.4f │\n",
        stats_so2.promedio, stats_so2.minimo, stats_so2.maximo, stats_so2.desviacion_estandar);
    printf("│ NO2    │ %8.2f │ %8.2f │ %8.2f │ %12.4f │\n",
        stats_no2.promedio, stats_no2.minimo, stats_no2.maximo, stats_no2.desviacion_estandar);
    printf("│ PM2.5  │ %8.2f │ %8.2f │ %8.2f │ %12.4f │\n",
        stats_pm25.promedio, stats_pm25.minimo, stats_pm25.maximo, stats_pm25.desviacion_estandar);
    printf("└────────┴──────────┴──────────┴──────────┴──────────────┘\n");
}
