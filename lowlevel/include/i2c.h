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

/**
 * @brief enum of the possible I2C status, used for status monitoring
 * 
 */
typedef enum I2C_Status_E{
    I2C_OK,
    I2C_TIMEOUT, // timeout for unknown reason
}I2C_status;

#define I2C_MAX_TIMEOUT 10 //ms


/**
 * @defgroup I2C
 * @{
 * @brief Definitions for the I2C serial protocol
 *
 * OD: Open drain (req. for the protocol arbitration)
 * PULLUP: the two I2C lines have to be pulled up 
 * SCL: Clock Pin
 * SDA: Data pin
 */

#define I2C_GPIO_OTYPE GPIO_OTYPE_OD
#define I2C_GPIO_PULL_UP GPIO_PUPD_PULLUP

#define I2C1_SCL_GPIO_PORT GPIOB
#define I2C1_SCL_GPIO_RCC RCC_GPIOB
#define I2C1_SCL_GPIO_PIN GPIO6
#define I2C1_SCL_AF GPIO_AF4

#define I2C1_SDA_GPIO_PORT GPIOB
#define I2C1_SDA_GPIO_RCC RCC_GPIOB
#define I2C1_SDA_GPIO_PIN GPIO7
#define I2C1_SDA_AF GPIO_AF4
/** @} */

/**
 * @brief Set the application-specific I2C configuration
 * 
 * @param i2c_peripheral I2C peripheral used (expected I2C1 or I2C2)
 */
void i2c_setup(uint32_t i2c_peripheral);

/**
 * @brief This function re-implement Libopencm3 write on I2C bus with 7 bit address
 *
 * @param i2c       I2C peripheral used
 * @param addr      address of slave
 * @param data      data to be sent
 * @param n         size of data in byte
 * 
 * @return I2C bus status
 * @see libopencm3 i2c_write7
 */
I2C_status i2c_write7(uint32_t i2c, int addr, uint8_t *data, size_t n);

/**
 * @brief This function re-implement Libopencm3 read on I2C bus with 7 bit address
 *
 * @param i2c       I2C peripheral used
 * @param addr      address of slave
 * @param res       data that have been read
 * @param n         size of data in byte
 * 
 * @return I2C bus status
 * @see libopencm3 i2c_read7
 */
I2C_status i2c_read7(uint32_t i2c, int addr, uint8_t *res, size_t n);
