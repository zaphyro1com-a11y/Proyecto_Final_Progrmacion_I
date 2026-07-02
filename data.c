#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "zonas.h"

/**
 * Genera valores aleatorios realistas para historicos
 */
void generarHistoricoSimulado(ZonaUrbana* zona) {
    int dia, i;
    float suma_co, suma_so2, suma_no2, suma_pm25;
    
    if (zona == NULL) return;
    
    /* Semilla aleatoria basada en ID de zona */
    srand(zona->id * 12345);
    
    /* Generar historico de 30 dias con variaciones realistas */
    for (dia = 0; dia < DIAS_HISTORICO; dia++) {
        /* Valores base tipicos para Quito (altitud 2850m) */
        zona->historico[dia].co = 5.0f + (rand() % 8);      /* 5-13 mg/m3 */
        zona->historico[dia].so2 = 12.0f + (rand() % 20);   /* 12-32 ug/m3 */
        zona->historico[dia].no2 = 18.0f + (rand() % 25);   /* 18-43 ug/m3 */
        zona->historico[dia].pm25 = 20.0f + (rand() % 30);  /* 20-50 ug/m3 */
    }
    
    /* Valor actual basado en promedio del historico */
    suma_co = 0;
    suma_so2 = 0;
    suma_no2 = 0;
    suma_pm25 = 0;
    for (i = 0; i < DIAS_HISTORICO; i++) {
        suma_co += zona->historico[i].co;
        suma_so2 += zona->historico[i].so2;
        suma_no2 += zona->historico[i].no2;
        suma_pm25 += zona->historico[i].pm25;
    }
    
    zona->actual.co = suma_co / DIAS_HISTORICO + (rand() % 5 - 2);
    zona->actual.so2 = suma_so2 / DIAS_HISTORICO + (rand() % 8 - 4);
    zona->actual.no2 = suma_no2 / DIAS_HISTORICO + (rand() % 10 - 5);
    zona->actual.pm25 = suma_pm25 / DIAS_HISTORICO + (rand() % 12 - 6);
    
    /* Asegurar valores positivos */
    if (zona->actual.co < 0) zona->actual.co = 0;
    if (zona->actual.so2 < 0) zona->actual.so2 = 0;
    if (zona->actual.no2 < 0) zona->actual.no2 = 0;
    if (zona->actual.pm25 < 0) zona->actual.pm25 = 0;
}

/**
 * Asigna condiciones climaticas realistas por zona
 */
void asignarCondicionesClimaticas(ZonaUrbana* zona) {
    if (zona == NULL) return;
    
    /* Variaciones por zona de Quito */
    srand(zona->id * 54321);
    
    switch (zona->id) {
        case 0: /* Centro (mas urbano, mas calido) */
            zona->condiciones_clima.temperatura = 18.0f + (rand() % 4);
            zona->condiciones_clima.velocidad_viento = 8.0f + (rand() % 5);
            zona->condiciones_clima.humedad = 65.0f + (rand() % 15);
            break;
        case 1: /* La Mariscal (comercial) */
            zona->condiciones_clima.temperatura = 17.0f + (rand() % 5);
            zona->condiciones_clima.velocidad_viento = 10.0f + (rand() % 6);
            zona->condiciones_clima.humedad = 60.0f + (rand() % 20);
            break;
        case 2: /* Calderon (norte, industrial) */
            zona->condiciones_clima.temperatura = 16.0f + (rand() % 5);
            zona->condiciones_clima.velocidad_viento = 12.0f + (rand() % 8);
            zona->condiciones_clima.humedad = 70.0f + (rand() % 15);
            break;
        case 3: /* Cumbaya (este, mas fresco) */
            zona->condiciones_clima.temperatura = 14.0f + (rand() % 4);
            zona->condiciones_clima.velocidad_viento = 15.0f + (rand() % 10);
            zona->condiciones_clima.humedad = 55.0f + (rand() % 25);
            break;
        case 4: /* Turubamba (sur, residencial) */
            zona->condiciones_clima.temperatura = 17.0f + (rand() % 4);
            zona->condiciones_clima.velocidad_viento = 9.0f + (rand() % 7);
            zona->condiciones_clima.humedad = 68.0f + (rand() % 17);
            break;
        default:
            zona->condiciones_clima.temperatura = 17.0f;
            zona->condiciones_clima.velocidad_viento = 10.0f;
            zona->condiciones_clima.humedad = 65.0f;
    }
}

/**
 * Inicializa las 5 parroquias de Quito con datos simulados
 */
void inicializarZonasQuito(ZonaUrbana* zonas, int cantidad) {
    int i;
    const char* parroquias[] = {
        "Quito Centro",
        "La Mariscal",
        "Calderon",
        "Cumbaya",
        "Turubamba"
    };
    
    if (zonas == NULL || cantidad < MAX_ZONAS) return;
    
    for (i = 0; i < MAX_ZONAS; i++) {
        inicializarZona(&zonas[i], i, parroquias[i]);
        generarHistoricoSimulado(&zonas[i]);
        asignarCondicionesClimaticas(&zonas[i]);
    }
}

/**
 * Muestra un resumen de los datos inicializados
 */
void mostrarResumenDatos(const ZonaUrbana* zonas, int cantidad) {
    int i;
    if (zonas == NULL || cantidad <= 0) return;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║       SISTEMA DE MONITOREO AMBIENTAL - QUITO, ECUADOR         ║\n");
    printf("║                  Datos Inicializados (5 Zonas)                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n%-20s | CO(mg) | SO2(ug) | NO2(ug) | PM2.5(ug) | Temp(C) | Viento(km/h)\n", "Parroquia");
    printf("%-20s-+--------+--------+--------+----------+---------+-------------\n", "");
    
    for (i = 0; i < cantidad; i++) {
        printf("%-20s | %6.1f | %7.1f | %7.1f | %8.1f | %7.1f | %11.1f\n",
            zonas[i].nombre,
            zonas[i].actual.co,
            zonas[i].actual.so2,
            zonas[i].actual.no2,
            zonas[i].actual.pm25,
            zonas[i].condiciones_clima.temperatura,
            zonas[i].condiciones_clima.velocidad_viento
        );
    }
    printf("\n");
}
