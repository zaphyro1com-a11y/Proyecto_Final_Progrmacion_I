#include <stdio.h>
#include <stdlib.h>
#include "zonas.h"
#include "data.h"
#include "limites.h"
#include "persistencia.h"
#include "reportes.h"
#include "monitoreo.h"
#include "estadisticas.h"
#include "prediccion.h"
#include "alertas.h"
#include "menu.h"

int main(void) {
    ZonaUrbana zonas[MAX_ZONAS];
    int zonas_cargadas;
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION    ║\n");
    printf("║                     DEL AIRE EN QUITO                         ║\n");
    printf("║                      Ecuador - 2026                          ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\nLimites OMS 2021 (utilizados en el sistema):\n");
    printf("  - CO:    %.2f mg/m3 (8h)\n", LIMITE_CO);
    printf("  - SO2:   %.2f ug/m3 (24h)\n", LIMITE_SO2);
    printf("  - NO2:   %.2f ug/m3 (anual)\n", LIMITE_NO2);
    printf("  - PM2.5: %.2f ug/m3 (anual)\n\n", LIMITE_PM25);
    
    /* Detectar si ya existen datos guardados */
    if (existeArchivoHistorico()) {
        printf("[*] Datos encontrados. Cargando desde persistencia...\n");
        zonas_cargadas = cargarHistoricoDesdeArchivo(zonas, MAX_ZONAS);
        if (zonas_cargadas <= 0) {
            fprintf(stderr, "[ERROR] Fallo al cargar datos. Inicializando nuevos...\n");
            printf("[*] Inicializando 5 parroquias de Quito...\n");
            inicializarZonasQuito(zonas, MAX_ZONAS);
            guardarHistoricoEnArchivo(zonas, MAX_ZONAS);
        }
    } else {
        printf("[*] Primer uso detectado. Inicializando 5 parroquias de Quito...\n");
        inicializarZonasQuito(zonas, MAX_ZONAS);
        guardarHistoricoEnArchivo(zonas, MAX_ZONAS);
    }
    
    printf("\n[*] Mostrando datos:\n");
    mostrarResumenDatos(zonas, MAX_ZONAS);
    
    printf("\n[*] Iniciando interfaz interactiva...\n");
    iniciarInterfazInteractiva(zonas, MAX_ZONAS);
    
    printf("\n[*] Sistema finalizado correctamente.\n");
    printf("[*] Archivos ubicados en directorio 'datos/'\n\n");
    
    return EXIT_SUCCESS;
}
