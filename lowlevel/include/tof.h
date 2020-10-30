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
#include "i2c.h"
//TODO include API
#include "vl53l0x_api.h"

#define TOF_NUM 1

//Fixed in hardware
#define TOF_DEFAULT_ADDR 0x52 
#define TOF_DELAY 10

//Shortcut redefinitions
typedef VL53L0X_RangingMeasurementData_t ToF_Data_t;
typedef VL53L0X_Dev_t ToF_Dev_t;


typedef struct ToF_Handler_S{
  ToF_Dev_t dev;
  ToF_Data_t data;
}ToF_Handler_t;

typedef struct ToF_Params_S{
  //Read
  uint8_t VhvSettings;
  uint8_t PhaseCal;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;

  //Write
  FixPoint1616_t signalLimit;
  FixPoint1616_t sigmaLimit;
  uint32_t timingBudget;
  uint8_t preRangeVcselPeriod;
  uint8_t finalRangeVcselPeriod;
}ToF_Params_t;





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

/* Liste globale de tout les tofs */
ToF_Handler_t tof[TOF_NUM];


/**
 * @brief setup the tof and all pin
 * 
 */
void tof_setup();

/**
 * @brief initialize 1 tof according to fig2 User Manual
 * @param htof  pointer on the tof "object"
 * @param i2c_addr  i2c address of the given tof
 * @return bool value of success
 */
//int _tof_init(ToF_Handler_t *htof, uint8_t i2c_addr);

/**
 * @brief reset all tof via the shift register
 * 
 */
void tof_reset();

/**
 * @brief reset the shiftregister to start counting from 0
 * 
 */
void _shift_reg_init();

/**
 * @brief pulse at the output +i
 * @params i    amount of shifting
 * 
 */
void _shift_reg(int i);
