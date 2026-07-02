#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "zonas.h"

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #define MKDIR(dir) mkdir(dir, 0755)
#endif

/**
 * Crea directorio datos si no existe
 */
int crearDirectorioDatos(void) {
    #ifdef _WIN32
        /* En Windows, intentar crear y ignorar si ya existe */
        _mkdir("datos");
    #else
        /* En Linux/Unix */
        mkdir("datos", 0755);
    #endif
    
    return 0;
}

/**
 * Verifica si el archivo de historico existe
 */
int existeArchivoHistorico(void) {
    FILE* archivo = fopen(ARCHIVO_HISTORICO, "r");
    if (archivo != NULL) {
        fclose(archivo);
        return 1;
    }
    return 0;
}

/**
 * Guarda historicos de contaminacion en archivo
 * Formato: ID | Nombre | CO_actual | SO2_actual | NO2_actual | PM25_actual | 
 *          CO_hist[0] SO2_hist[0] ... (30 valores por contaminante)
 */
int guardarHistoricoEnArchivo(const ZonaUrbana* zonas, int cantidad) {
    FILE* archivo;
    int i, dia;
    
    if (zonas == NULL || cantidad <= 0) return -1;
    
    /* Crear directorio si no existe */
    crearDirectorioDatos();
    
    /* Abrir archivo para escribir */
    archivo = fopen(ARCHIVO_HISTORICO, "w");
    if (archivo == NULL) {
        fprintf(stderr, "[ERROR] No se puede abrir archivo: %s\n", ARCHIVO_HISTORICO);
        return -1;
    }
    
    /* Escribir encabezado */
    fprintf(archivo, "# HISTORICO DE CONTAMINACION - QUITO\n");
    fprintf(archivo, "# Formato: ID | Nombre | CO_act | SO2_act | NO2_act | PM25_act | ");
    fprintf(archivo, "TEMP | VIENTO | HUMEDAD | ");
    fprintf(archivo, "[CO_h0-CO_h29 SO2_h0-SO2_h29 NO2_h0-NO2_h29 PM25_h0-PM25_h29]\n\n");
    
    /* Escribir datos de cada zona */
    for (i = 0; i < cantidad; i++) {
        /* ID, Nombre, valores actuales */
        fprintf(archivo, "%d|%s|%.2f|%.2f|%.2f|%.2f|%.1f|%.1f|%.1f|",
            zonas[i].id,
            zonas[i].nombre,
            zonas[i].actual.co,
            zonas[i].actual.so2,
            zonas[i].actual.no2,
            zonas[i].actual.pm25,
            zonas[i].condiciones_clima.temperatura,
            zonas[i].condiciones_clima.velocidad_viento,
            zonas[i].condiciones_clima.humedad
        );
        
        /* Historico de 30 dias para CO */
        for (dia = 0; dia < DIAS_HISTORICO; dia++) {
            fprintf(archivo, "%.2f", zonas[i].historico[dia].co);
            if (dia < DIAS_HISTORICO - 1) fprintf(archivo, ",");
        }
        fprintf(archivo, "|");
        
        /* Historico de 30 dias para SO2 */
        for (dia = 0; dia < DIAS_HISTORICO; dia++) {
            fprintf(archivo, "%.2f", zonas[i].historico[dia].so2);
            if (dia < DIAS_HISTORICO - 1) fprintf(archivo, ",");
        }
        fprintf(archivo, "|");
        
        /* Historico de 30 dias para NO2 */
        for (dia = 0; dia < DIAS_HISTORICO; dia++) {
            fprintf(archivo, "%.2f", zonas[i].historico[dia].no2);
            if (dia < DIAS_HISTORICO - 1) fprintf(archivo, ",");
        }
        fprintf(archivo, "|");
        
        /* Historico de 30 dias para PM25 */
        for (dia = 0; dia < DIAS_HISTORICO; dia++) {
            fprintf(archivo, "%.2f", zonas[i].historico[dia].pm25);
            if (dia < DIAS_HISTORICO - 1) fprintf(archivo, ",");
        }
        fprintf(archivo, "\n");
    }
    
    fclose(archivo);
    printf("[*] Historico guardado en: %s\n", ARCHIVO_HISTORICO);
    return 0;
}

/**
 * Carga historicos de contaminacion desde archivo
 */
int cargarHistoricoDesdeArchivo(ZonaUrbana* zonas, int cantidad) {
    FILE* archivo;
    int i, dia, items_leidos;
    char linea[4096];
    char nombre_temp[MAX_NOMBRE];
    char historico_co_str[1024], historico_so2_str[1024], historico_no2_str[1024], historico_pm25_str[1024];
    float temp, viento, humedad;
    int id_temp;
    
    if (zonas == NULL || cantidad <= 0) return -1;
    
    /* Verificar si archivo existe */
    if (!existeArchivoHistorico()) {
        printf("[!] Archivo de historico no encontrado: %s\n", ARCHIVO_HISTORICO);
        return -1;
    }
    
    /* Abrir archivo para lectura */
    archivo = fopen(ARCHIVO_HISTORICO, "r");
    if (archivo == NULL) {
        fprintf(stderr, "[ERROR] No se puede abrir archivo: %s\n", ARCHIVO_HISTORICO);
        return -1;
    }
    
    /* Leer linea por linea */
    i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL && i < cantidad) {
        /* Ignorar lineas comentadas y vacias */
        if (linea[0] == '#' || linea[0] == '\n') continue;
        
        /* Parsear linea: ID|Nombre|CO|SO2|NO2|PM25|TEMP|VIENTO|HUMEDAD|hist_CO|hist_SO2|hist_NO2|hist_PM25 */
        items_leidos = sscanf(linea, 
            "%d|%49[^|]|%f|%f|%f|%f|%f|%f|%f|%1023[^|]|%1023[^|]|%1023[^|]|%1023s",
            &id_temp,
            nombre_temp,
            &zonas[i].actual.co,
            &zonas[i].actual.so2,
            &zonas[i].actual.no2,
            &zonas[i].actual.pm25,
            &temp,
            &viento,
            &humedad,
            historico_co_str,
            historico_so2_str,
            historico_no2_str,
            historico_pm25_str
        );
        
        if (items_leidos < 13) {
            printf("[!] Error al parsear zona %d. Items leidos: %d\n", i, items_leidos);
            continue;
        }
        
        /* Asignar valores basicos */
        zonas[i].id = id_temp;
        strncpy(zonas[i].nombre, nombre_temp, MAX_NOMBRE - 1);
        zonas[i].nombre[MAX_NOMBRE - 1] = '\0';
        
        zonas[i].condiciones_clima.temperatura = temp;
        zonas[i].condiciones_clima.velocidad_viento = viento;
        zonas[i].condiciones_clima.humedad = humedad;
        
        /* Parsear historicos (formato CSV) */
        /* CO */
        {
            char* token = strtok(historico_co_str, ",");
            for (dia = 0; dia < DIAS_HISTORICO && token != NULL; dia++) {
                zonas[i].historico[dia].co = (float)atof(token);
                token = strtok(NULL, ",");
            }
        }
        
        /* SO2 */
        {
            char* token = strtok(historico_so2_str, ",");
            for (dia = 0; dia < DIAS_HISTORICO && token != NULL; dia++) {
                zonas[i].historico[dia].so2 = (float)atof(token);
                token = strtok(NULL, ",");
            }
        }
        
        /* NO2 */
        {
            char* token = strtok(historico_no2_str, ",");
            for (dia = 0; dia < DIAS_HISTORICO && token != NULL; dia++) {
                zonas[i].historico[dia].no2 = (float)atof(token);
                token = strtok(NULL, ",");
            }
        }
        
        /* PM25 */
        {
            char* token = strtok(historico_pm25_str, ",");
            for (dia = 0; dia < DIAS_HISTORICO && token != NULL; dia++) {
                zonas[i].historico[dia].pm25 = (float)atof(token);
                token = strtok(NULL, ",");
            }
        }
        
        i++;
    }
    
    fclose(archivo);
    printf("[*] Cargadas %d zonas desde: %s\n", i, ARCHIVO_HISTORICO);
    return i;
}
