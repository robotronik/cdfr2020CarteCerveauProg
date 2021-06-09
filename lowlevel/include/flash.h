/**
 * @file      flash.c
 * @brief     This implements function to read and flash data in the rom sector
 * @date      Wed Jun  9 21:06:39 2021
 * @author    benano NPXav
 * @copyright
 *
 * Robotronik phelma
 * This file is part of cdfr2020CerveauProg
 * useful reference:
 * libopencm3 example: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32-h107/flash_rw_example/flash_rw_example.c
 */

#pragma once

#include <libopencm3/stm32/flash.h>
#include <stdio.h>

/**
 * @defgroup FLASH_MEMORY flash_memory
 * @{
 * @name Rom memory structure
 * @brief These define the addresses use to operate on the rom part of the memory
 * This organization is dependant on the very structure of the memory on the µcontroller
 * You must check the documentation and allocate enough memory for your program and ram
 * and reserve some of it for your storage in the linked script
 */

#define FLASH_OPERATION_ADDRESS ((uint32_t)0x08020000)
#define FLASH_SECTOR_NUM_MAX 3
#define FLASH_SECTOR_SIZE 128000
#define RESULT_OK 0
#define FLASH_PROGRAM_SIZE 0 // this means that words are of width=1 byte
/**
 * @}
 */

/**
 * @brief This function programs data into the rom of the STM32
 * @details STM32 F4 has big sectors that can be programmed
 * @param[in] sector adress of the sector to program (it will be overwritten /!\)
 * @param[out] input_data data to program
 * @param[in] num_elements number of byte of input_data
 * @return error status (success=0)
 *
 */
uint32_t flash_program_data(uint8_t sector, uint8_t *input_data, uint16_t num_elements);

/**
 * @brief This function reads data in the rom of the STM32
 * @details
 * @param[in] start_address memory adress to start reading from
 * @param[in] num_elements number of bytes to read
 * @param[out] poutput_data pointer to the byte array where the read data will be stored
 */
void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *poutput_data);
