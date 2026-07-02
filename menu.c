#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "monitoreo.h"
#include "prediccion.h"
#include "alertas.h"
#include "reportes.h"
#include "estadisticas.h"

/**
 * Muestra menu principal
 */
int mostrarMenuPrincipal(void) {
    int opcion;
    
    printf("\n");
    printf("ÔòöÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòù\n");
    printf("Ôòæ            MENU PRINCIPAL - SISTEMA DE MONITOREO            Ôòæ\n");
    printf("ÔòÜÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòÉÔòØ\n");
    printf("\n");
    printf("  1. Ver monitoreo actual de todas las zonas\n");
    printf("  2. Ver predicciones para proximas 24 horas\n");
    printf("  3. Ver alertas y recomendaciones de mitigacion\n");
    printf("  4. Generar reporte consolidado\n");
    printf("  5. Salir del sistema\n");
    printf("\n");
    printf("  Seleccione una opcion (1-5): ");
    fflush(stdout);
    
    if (scanf("%d", &opcion) != 1) {
        opcion = -1;
        while (getchar() != '\n'); /* Limpiar buffer */
    }
    
    return opcion;
}

/**
 * Ejecuta opcion del menu
 */
int ejecutarOpcionMenu(int opcion, ZonaUrbana* zonas, int cantidad) {
    int i;
    
    if (zonas == NULL) return 0;
    
    switch (opcion) {
        case 1:
            /* Monitoreo */
            mostrarMonitoreoTodasLasZonas(zonas, cantidad);
            break;
            
        case 2:
            /* Predicciones */
            for (i = 0; i < cantidad; i++) {
                ejecutarPrediccionPonderada(&zonas[i]);
            }
            mostrarPrediccionesTodasLasZonas(zonas, cantidad);
            break;
            
        case 3:
            /* Alertas */
            mostrarAlertasTodas(zonas, cantidad);
            break;
            
        case 4:
            /* Reporte */
            printf("\n[*] Generando reporte consolidado...\n");
            if (generarReporteConsolidado(zonas, cantidad, 0, 0) != 0) {
                printf("[ERROR] No se pudo generar el reporte\n");
            } else {
                printf("[OK] Reporte generado exitosamente\n");
            }
            break;
            
        case 5:
            printf("\n[*] Finalizando sistema. Datos guardados en 'datos/'\n");
            return 1; /* Salir */
            
        default:
            printf("\n[ERROR] Opcion invalida. Intente nuevamente.\n");
    }
    
    return 0; /* Continuar */
}

/**
 * Inicia interfaz interactiva
 */
void iniciarInterfazInteractiva(ZonaUrbana* zonas, int cantidad) {
    int opcion;
    int salir = 0;
    
    if (zonas == NULL) return;
    
    while (!salir) {
        opcion = mostrarMenuPrincipal();
        salir = ejecutarOpcionMenu(opcion, zonas, cantidad);
    }
}
