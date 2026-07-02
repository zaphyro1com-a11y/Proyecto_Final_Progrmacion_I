#include <stdio.h>
#include <string.h>
#include "zonas.h"

/**
 * Inicializa una zona urbana con valores por defecto
 */
void inicializarZona(ZonaUrbana* zona, int id, const char* nombre) {
    int i;
    if (zona == NULL) return;
    
    zona->id = id;
    strncpy(zona->nombre, nombre, MAX_NOMBRE - 1);
    zona->nombre[MAX_NOMBRE - 1] = '\0';
    
    /* Inicializar todos los contaminantes en 0 */
    zona->actual.co = 0.0f;
    zona->actual.so2 = 0.0f;
    zona->actual.no2 = 0.0f;
    zona->actual.pm25 = 0.0f;
    
    zona->prediccion.co = 0.0f;
    zona->prediccion.so2 = 0.0f;
    zona->prediccion.no2 = 0.0f;
    zona->prediccion.pm25 = 0.0f;
    
    zona->condiciones_clima.temperatura = 0.0f;
    zona->condiciones_clima.velocidad_viento = 0.0f;
    zona->condiciones_clima.humedad = 0.0f;
    
    /* Inicializar historico */
    for (i = 0; i < DIAS_HISTORICO; i++) {
        zona->historico[i].co = 0.0f;
        zona->historico[i].so2 = 0.0f;
        zona->historico[i].no2 = 0.0f;
        zona->historico[i].pm25 = 0.0f;
    }
}

/**
 * Muestra informacion detallada de una zona
 */
void mostrarZona(const ZonaUrbana* zona) {
    if (zona == NULL) return;
    
    printf("\n========================================\n");
    printf("ZONA: %s (ID: %d)\n", zona->nombre, zona->id);
    printf("========================================\n");
    
    printf("\nContaminantes Actuales:\n");
    printf("  CO:    %.2f mg/m3\n", zona->actual.co);
    printf("  SO2:   %.2f ug/m3\n", zona->actual.so2);
    printf("  NO2:   %.2f ug/m3\n", zona->actual.no2);
    printf("  PM2.5: %.2f ug/m3\n", zona->actual.pm25);
    
    printf("\nCondiciones Climaticas:\n");
    printf("  Temperatura:      %.1f C\n", zona->condiciones_clima.temperatura);
    printf("  Velocidad viento: %.1f km/h\n", zona->condiciones_clima.velocidad_viento);
    printf("  Humedad:          %.1f %%\n", zona->condiciones_clima.humedad);
}

/**
 * Muestra todas las zonas de forma resumida
 */
void mostrarTodasLasZonas(ZonaUrbana* zonas, int cantidad) {
    int i;
    if (zonas == NULL || cantidad <= 0) return;
    
    printf("\n\n######## TODAS LAS ZONAS ########\n");
    for (i = 0; i < cantidad; i++) {
        mostrarZona(&zonas[i]);
    }
    printf("\n");
}
