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
 * @author NPXav Benano
*/

#pragma once

#include "gpio.h"
#include "i2c.h"

#include "vl53l0x_api.h"

#define TOF_NUM 1

//Fixed in hardware
#define TOF_DEFAULT_ADDR 0x52 
#define TOF_DELAY 50

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

/*Long Range Profile*/
/** @defgroup Long Range Ranging Profile 
 * @{
 */
#define VL53L0X_LR_SIGNAL_LIMIT             (FixPoint1616_t)(0.25*65536)
#define VL53L0X_LR_SIGMA_LIMIT              (FixPoint1616_t)(18*65536)
#define VL53L0X_LR_TIMING_BUDGET            33000
#define VL53L0X_LR_VCSEL_PERIOD_PRE_RANGE   14
#define VL53L0X_LR_VCSEL_PERIOD_FINAL_RANGE 10
/** @} */

/**
 * @brief Storage of all Calibration Parameter for the TOF (VL53L1X)
 * @struct VL53L0X_Calibration_Parameter_S tof.h
 * TODO: write all parameter
 */
typedef struct VL53L0X_Calibration_Parameter_S{
  uint8_t VhvSettings;
  uint8_t PhaseCal;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;
  int32_t OffsetMicroMeter;
  FixPoint1616_t XTalkCompensationRateMegaCps;
}VL53L0X_Calibration_Parameter;


/* Liste globale de tout les tofs */
// VL53L0X_DEV* tof[TOF_NUM];
// VL53L0X_DEV myTof;


/**
 * @brief setup the tof and all pin
 * 
 */
VL53L0X_Error tof_setup(VL53L0X_DEV dev);

/**
 * @brief setup the structure with the standard address and I2C peripheral
 * @param dev our tof object
 */
void _tof_init_dev(VL53L0X_DEV dev);

/**
 * @brief Check if the tof is answering
 * @param dev our tof object
 * @return return the error type from the API
 */
VL53L0X_Error _tof_poke(VL53L0X_DEV dev);

/**
 * @brief set the tof I2C slave address
 * @param dev our tof object
 * @param addr the slave address
 * @return return the error type from the API
 */
VL53L0X_Error _tof_set_address(VL53L0X_DEV dev, uint8_t addr);

/**
 * @brief setup the tof with its address (calling poke and set address)
 * @param dev our tof object
 * @param addr the slave address
 * @return return the error type from the API
 */
VL53L0X_Error _tof_setup_dev(VL53L0X_DEV dev, uint8_t addr);

/**
 * @brief Configure the tof with its calibration data and ranging profile
 * @param dev our tof object
 * @param calib_param structure that store all calibration parameter
 * @return return the error type from the API
 */
VL53L0X_Error _tof_configure_dev(VL53L0X_DEV dev, VL53L0X_Calibration_Parameter calib_param);

/**
 * @brief Function to calibrate a tof (each tof has its own calibration)
 * @param dev our tof object
 * @param calib_param structure to store all calibration parameter
 * @param offset_cal_distance distance to the white target in millimeter
 * @param xTalk_cal_distance distance to the grey target in millimeter
 * @return return the error type from the API
 */
VL53L0X_Error _tof_calibration(VL53L0X_DEV dev, VL53L0X_Calibration_Parameter* calib_param, FixPoint1616_t offset_cal_distance, FixPoint1616_t xTalk_cal_distance);

/**
 * @brief Function to acces the measure of the tof
 * @param dev our tof object
 * @param range pointer to the variable storing the range in /!\ MILLIMETER /!\
 * @return return the error type from the API
 */
VL53L0X_Error tof_get_measure(VL53L0X_DEV dev, uint16_t* range);

/**
 * @brief Function to print tof device information
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 */
VL53L0X_Error tof_print_device_info(VL53L0X_DEV dev);

/**
 * @brief Function to print tof calibration information
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 */
VL53L0X_Error tof_print_calib_info(VL53L0X_DEV dev);

/**
 * @brief Function to print PAL state
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 * @see VL53L0X State group (l273 vl53l0x_def.h)
 */
VL53L0X_Error tof_print_PAL_state(VL53L0X_DEV dev);

/**
 * @brief Function to print device mode
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 * @see VL53L0X DeviceModes group (l181 vl53l0x_def.h)
 */
VL53L0X_Error tof_print_device_mode(VL53L0X_DEV dev);

/**
 * @brief Function to print range status
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 * @param measure_data measured data buffer
 * @see VL53L0X Range status (p16 User Manual)
 */
VL53L0X_Error tof_print_ranging_status(VL53L0X_DEV dev, VL53L0X_RangingMeasurementData_t measure_data);

/**
 * @brief Function to print measured data in detail
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 * @param measure_data measured data buffer
 * @see VL53L0X Ranging Measurement Data
 */
VL53L0X_Error tof_print_data_measure(VL53L0X_DEV dev, VL53L0X_RangingMeasurementData_t measure_data);

/**
 * @brief Function to print interrupt status
 * @warning We assume you already setup your tof and uart
 * @param dev our tof object
 * @see VL53L0X_REG_SYSTEM_INTERRUPT (l152 vl53l0x_device.h)
 */
VL53L0X_Error tof_print_int_status(VL53L0X_DEV dev);

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
