/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements all needed peripheral to use the tof 
 * 
 * @date  08/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author PhenixRobotik NPXav Benano Trukbidule
*/

#pragma once

#include "gpio.h"

#define TOF_NUM 8

/**
 * @defgroup TOF_SR tof_sr
 * @{
 * @brief Resetting the tof is done via this shift register 
 * 
 * Registre à décalage :
 * à cause de la configuration interne des ToFs, il n'est pas possible de les reset tous en même temps car alors les adresses I2C se 
 * réattribueraient un peu n'importe comment et ne 
 * permettraient plus au microcontrôleur de savoir quel ToF a 
 * quelle adresse. 
 * C'est pourquoi on a choisi d'utiliser un registre à décalage : 
 * Lorsqqu'il faut reset les ToFs, on passe DSAB à 1 puis on envoie une impulsion sur CP pour mettre ce 1 dans le premier bit du registre.
 * Ceci aura pour effet de reset le premier ToF qui ne connaît alors
 * plus son adresse.
 * On passe DSAB à 0 pour éviter de reset plusieurs ToFs d'un coup.
 * Ensuite on répète :
 * "On envoie une impulsion sur CP pour faire passer le 1 du bit n au bit n+1 dans le registre.
 * Ce faisant, le n ème ToF n'est plus en état de reset et on peut lui réattribuer sa propre adresse sans crainte de confusion."
 * On reset successivement tous les ToFs en leur réattribuant leur propre adresse à chaque fois
 */
#define SR_DSAB_RCC     RCC_GPIOC 
#define SR_DSAB_PORT    GPIOC
#define SR_DSAB_PIN     GPIO1

#define SR_CP_RCC       RCC_GPIOC
#define SR_CP_PORT      GPIOC
#define SR_CP_PIN       GPIO0
/** @} */

/**
 * @brief setup the tof and all pin
 * 
 */
void tof_setup();


/**
 * @brief reset all tof via the shift register
 * 
 */
void tof_reset();
