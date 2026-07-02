#ifndef ZONAS_H
#define ZONAS_H

#define MAX_ZONAS       5
#define DIAS_HISTORICO  30
#define MAX_NOMBRE      50
#define MAX_ALERTA      256

/* Estructura para datos climaticos */
typedef struct {
    float temperatura;          /* Temperatura en grados Celsius */
    float velocidad_viento;     /* Velocidad del viento en km/h */
    float humedad;              /* Humedad relativa en % */
} Clima;

/* Estructura para niveles de contaminantes */
typedef struct {
    float co;                   /* Monoxido de carbono en mg/m3 */
    float so2;                  /* Dioxido de azufre en ug/m3 */
    float no2;                  /* Dioxido de nitrogeno en ug/m3 */
    float pm25;                 /* Particulas PM2.5 en ug/m3 */
} Contaminantes;

/* Estructura para una zona urbana */
typedef struct {
    int id;                     /* ID unico de la zona */
    char nombre[MAX_NOMBRE];    /* Nombre de la parroquia */
    Contaminantes actual;       /* Medicion actual del dia */
    Contaminantes historico[DIAS_HISTORICO];  /* Historico de 30 dias */
    Contaminantes prediccion;   /* Prediccion para 24h siguientes */
    Clima condiciones_clima;    /* Condiciones climaticas actuales */
} ZonaUrbana;

/* Prototipos de funciones base */
void inicializarZona(ZonaUrbana* zona, int id, const char* nombre);
void mostrarZona(const ZonaUrbana* zona);
void mostrarTodasLasZonas(ZonaUrbana* zonas, int cantidad);

#endif
