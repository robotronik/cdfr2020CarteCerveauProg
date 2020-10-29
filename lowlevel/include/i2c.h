/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
     * @brief This implements the setup of an I2C peripheral
 * 
 * @date  10/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano PhoenixRobotics (Antonin H.)
*/

#pragma once

#include <libopencm3/stm32/i2c.h>
#include "gpio.h"

#define I2C_CLK_MHZ    8
#define I2C_MAX_NBYTES 255

typedef enum I2C_Status_E{
    I2C_OK,
    I2C_ETIME, // timeout for unknown reason
    I2C_ENACK, // NACK
}I2C_Status;

/**
 * @brief Set the application-specific I2C configuration
 */
void i2c_setup();
