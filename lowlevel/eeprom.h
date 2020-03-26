#pragma once

#include <stdbool.h>
#include <stdint.h>


/*****************************************************************************/
// SPI-connected EEPROM chip

// This is inspired from https://github.com/mattairtech/EEPROM_CAT25

typedef union {
  struct {
    uint8_t  :1;  // reserved
    uint8_t  :1;   // reserved
    uint8_t  :1;      // reserved
    uint8_t  :1;   // reserved
    uint8_t  BP:2;   // Block protection 0
    uint8_t  WEL:1;   // Write Enable Latch
    uint8_t  nRDY:1;  // Ready (ready when low)
  } bit;
  uint8_t word;
} EEPROM_CAT25_Status_Register;


void eeprom_setup();

void eeprom_write_enable();
void eeprom_write_disable();
EEPROM_CAT25_Status_Register eeprom_status_register();
bool eeprom_is_ready();

uint8_t eeprom_read_byte  (uint8_t address);
uint8_t eeprom_write_byte (uint8_t address, uint8_t byte);

uint8_t eeprom_read_buff (uint8_t address, uint8_t length, uint8_t* buffer);
uint8_t eeprom_write_buff(uint8_t address, uint8_t length, uint8_t* buffer);
