# ESPECIFICACIONES TÉCNICAS DE DESARROLLO: SISTEMA DE MONITOREO AMBIENTAL C
# ROL PARA EL AGENTE DE IA: Arquitecto de Software y Programador Experto en C

---

## 1. OBJETIVO DEL SISTEMA
Diseñar e implementar un programa modular en Lenguaje C ejecutable para el "Sistema Integral de Gestión y Predicción de Contaminación del Aire en Zonas Urbanas". El sistema debe evaluar y mitigar los impactos de los contaminantes atmosféricos en la salud pública y el medio ambiente en un mínimo de 5 zonas urbanas.

---

## 2. VARIABLES Y ESPECIFICACIONES DEL ENTORNO

### Variables de Entrada (Input Variables)
* **Datos de la Zona:** ID numérico, Nombre de la zona (cadena de caracteres).
* **Contaminantes Actuales:** CO2, SO2, NO2, PM2.5 (valores flotantes).
* **Histórico:** Matriz o arreglo de registros con datos de los últimos 30 días para cada uno de los 4 contaminantes.
* **Factores Climáticos Actuantes:** Temperatura (°C), Velocidad del viento (km/h), Porcentaje de Humedad (%).

### Variables de Salida (Output Variables)
* **Métricas:** Promedio histórico de contaminación por zona, porcentaje de desviación frente a límites normativos.
* **Predicciones:** Niveles estimados de CO2, SO2, NO2 y PM2.5 para las próximas 24 horas.
* **Alertas:** Cadenas de texto con advertencias preventivas críticas o moderadas.
* **Recomendaciones:** Texto con planes de mitigación social y ambiental.
* **Persistencia:** Archivo físico de texto plano (`.txt` o `.dat`) con el reporte consolidado.

---

## 3. LIMITACIONES Y RESTRICCIONES TÉCNICAS ESTRICTAS
El Agente de IA debe apegarse estrictamente a las siguientes directrices de diseño de código; cualquier violación invalida la solución:

* **Lenguaje:** C Estándar (ANSI C / C99).
* **Librerías Prohibidas:** No se permite el uso de librerías externas o frameworks de terceros para el manejo de archivos, parsing, o cálculos matemáticos avanzados. Solo se deben usar librerías estándar (`<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>`).
* **Paradigma Obligatorio:** Programación modular (funciones parametrizadas) y paso de datos eficiente utilizando **punteros** para evitar la duplicación de estructuras en memoria.
* **Estructuras de Datos:** Uso explícito de `struct` anidados, arreglos multidimensionales (arreglos paralelos o históricos) y manipulación de cadenas nativas.
* **Persistencia:** El sistema debe guardar y leer los datos directamente desde archivos locales para garantizar la persistencia de los históricos entre ejecuciones.

---

## 4. REQUERIMIENTOS FUNCIONALES (LOGICA DE NEGOCIO)

### RF1: Monitoreo Actual y Límites OMS
El sistema debe comparar las lecturas del día con los siguientes umbrales regulatorios predefinidos:
* Limite CO2: 50.0
* Limite SO2: 20.0
* Limite NO2: 40.0
* Limite PM2.5: 25.0

### RF2: Algoritmo de Predicción Ponderada (24 Horas)
Para proyectar los niveles de contaminación del día siguiente, la IA debe implementar un cálculo matemático basado en un promedio ponderado de los últimos días del histórico, asignando un peso linealmente mayor a las fechas más recientes. 
Adicionalmente, se deben aplicar modificadores condicionales según el clima:
* **Alta velocidad del viento (> 20 km/h):** Debe actuar como factor de dispersión, reduciendo la predicción de contaminantes en un porcentaje estimado (ej. 15% o 20%).
* **Alta humedad (> 80%):** Debe actuar como factor de concentración para partículas como PM2.5, incrementando su valor proyectado.

### RF3: Sistema de Alertas y Mitigación Colectiva
Si la predicción o el estado actual supera los límites, el sistema debe disparar alertas específicas en consola y generar recomendaciones de mitigación tales como:
* Restricciones y reducción del tráfico vehicular pesado.
* Cierre temporal de actividades industriales en la periferia de la zona.
* Suspensión total de actividades físicas o recreativas al aire libre.

### RF4: Generación de Reportes
Exportación automatizada estructurada de los datos calculados (actuales, promedios de 30 días, predicciones y alertas activas) hacia un archivo físico para auditorías externas.

---

## 5. ARQUITECTURA DEL CÓDIGO FUENTE EN C

El Agente de IA debe estructurar el desarrollo del software bajo el siguiente esqueleto base:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ZONAS 5
#define DIAS_HISTORICO 30

typedef struct {
    float temperatura;
    float velocidad_viento;
    float humedad;
} Clima;

typedef struct {
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminantes;

typedef struct {
    int id;
    char nombre[50];
    Contaminantes actual;
    Contaminantes historico[DIAS_HISTORICO];
    Contaminantes prediccion;
    Clima condiciones_clima;
} ZonaUrbana;

// PROTOTIPOS OBLIGATORIOS PARA EL AGENTE DE IA
int cargarHistoricoBase(ZonaUrbana* zonas, const char* ruta);
void procesarMonitoreoActual(const ZonaUrbana* zonas);
void ejecutarPrediccionPonderada(ZonaUrbana* zona);
void evaluarAlertasYMitigacion(const ZonaUrbana* zona);
int exportarReporteConsolidado(const ZonaUrbana* zonas, const char* ruta);
void desplegarMenuInteractivos();