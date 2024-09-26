/*
 * config_juego.h
 *
 *  Created on: 04/09/2015
 *      Author: jmcg
 */

#ifndef EXAMEN_JUN_INICIALV2_CONFIG_JUEGO_H_
#define EXAMEN_JUN_INICIALV2_CONFIG_JUEGO_H_
#include <limits.h>

//Permite cambiar el modo de juego mientras se implementa la solucion
//#define JUEGO_SIMON 0
#define JUEGO_BUTTONFIGHT 0
#define JUEGO_MICECATS 1

#define DEBOUNCE_TIME 10

///* parametros del Mice&Cats */
//Nota --> scaledRand es una funcion que devuelve un flotante aleatorio entre 0 y 1.

#define MC_PROB_LARGA 0.5
#define MC_DURACION_LARGA ((0.5+scaledRand())*configTICK_RATE_HZ)
#define MC_DURACION_CORTA (0.1* configTICK_RATE_HZ)
#define MC_PAUSA ((2+3*scaledRand())*configTICK_RATE_HZ)
#define MC_MIN_POINTS (-50)
//TODO P1: Descomenta y cambia por la de arriba si no lo puedes arreglar
//#define MC_MIN_POINTS INT_MIN

/* parametros de ButtonFight */
#define BF_MAX_WAIT_TIME 200
#define BF_MAX_WAIT_PUSHES 4


#endif /* MODO_JUEGO_H_ */
