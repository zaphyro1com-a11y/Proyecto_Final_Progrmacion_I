#ifndef MENU_H
#define MENU_H

#include "zonas.h"

/**
 * Muestra menu principal del sistema
 * @return Opcion seleccionada (1-5)
 */
int mostrarMenuPrincipal(void);

/**
 * Ejecuta opcion seleccionada del menu
 * @param opcion Numero de opcion (1-5)
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 * @return 0 para continuar, 1 para salir
 */
int ejecutarOpcionMenu(int opcion, ZonaUrbana* zonas, int cantidad);

/**
 * Loop interactivo del sistema
 * @param zonas Array de ZonaUrbana
 * @param cantidad Numero de zonas
 */
void iniciarInterfazInteractiva(ZonaUrbana* zonas, int cantidad);

#endif
