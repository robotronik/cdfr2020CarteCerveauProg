/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the storage of data in flash memory
 * 
 * @date  12/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano
*/

#pragma once

#include <libopencm3/stm32/flash.h>
#include <stdio.h>

#define FLASH_OPERATION_ADDRESS ((uint32_t)0x08020000)
#define FLASH_SECTOR_NUM_MAX 3
#define FLASH_SECTOR_SIZE 128000
#define RESULT_OK 0

#define FLASH_PROGRAM_SIZE 0 // this means that words are of width=1 byte

/**
 * @brief This function programs data into the rom of the STM32
 * @details STM32 F4 has big sectors that can be programmed
 * @param[in] sector adress of the sector to program (it will be overwritten /!\)
 * @param[out] input_data data to program
 * @param[in] num_elements number of byte of input_data
 * @return error status (success=0)
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
