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

uint32_t flash_program_data(uint8_t sector, uint8_t *input_data, uint16_t num_elements);
void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *output_data);
