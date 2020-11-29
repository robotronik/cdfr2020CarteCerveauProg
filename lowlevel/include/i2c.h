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
#include <stdio.h>
#include "gpio.h"


typedef enum I2C_Status_E{
    I2C_OK,
    I2C_TIMEOUT, // timeout for unknown reason
}I2C_status;

#define I2C_MAX_TIMEOUT 20 //ms


//I2C1
#define I2C_GPIO_OTYPE GPIO_OTYPE_OD
#define I2C_GPIO_PULL_UP GPIO_PUPD_PULLUP

//SCL
#define I2C1_SCL_GPIO_PORT GPIOB
#define I2C1_SCL_GPIO_RCC RCC_GPIOB
#define I2C1_SCL_GPIO_PIN GPIO6
#define I2C1_SCL_AF GPIO_AF4
//SDA
#define I2C1_SDA_GPIO_PORT GPIOB
#define I2C1_SDA_GPIO_RCC RCC_GPIOB
#define I2C1_SDA_GPIO_PIN GPIO7
#define I2C1_SDA_AF GPIO_AF4





/**
 * @brief Set the application-specific I2C configuration
 */
void i2c_setup(uint32_t i2c_peripheral);


I2C_status i2c_write7(uint32_t i2c, int addr, uint8_t *data, size_t n);
I2C_status i2c_read7(uint32_t i2c, int addr, uint8_t *res, size_t n);
