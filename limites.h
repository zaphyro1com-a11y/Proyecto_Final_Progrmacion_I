#ifndef LIMITES_H
#define LIMITES_H

/* Limites de Contaminacion segun OMS 2021 (valores mas restrictivos para seguridad) */

#define LIMITE_PM25     15.0    /* PM2.5 anual en ug/m3 */
#define LIMITE_NO2      25.0    /* NO2 anual en ug/m3 */
#define LIMITE_SO2      40.0    /* SO2 24h en ug/m3 */
#define LIMITE_CO       10.0    /* CO 8h en mg/m3 */

/* Niveles de severidad de alertas (basados en % de exceso del limite) */
#define EXCESO_CRITICA  30.0    /* Alerta critica si supera 30% del limite */
#define EXCESO_MODERADA 10.0    /* Alerta moderada si supera 10% del limite */

#endif
