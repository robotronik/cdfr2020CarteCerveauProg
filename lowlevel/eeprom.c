#include "eeprom.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>


// Some C mojo to have a scope "protector" to select SPI
// The containing scope would be a "spi session"

static inline int select() {
  gpio_clear(GPIOA, GPIO15); return 0;
}
static inline int deselect(__attribute__((unused)) int* _) {
  gpio_set  (GPIOA, GPIO15); return 0;
}
#define spi_select \
  __attribute__((cleanup(deselect))) __attribute__((unused)) int a = select()


void eeprom_setup() {
  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);

  // Setup GPIOA15 pin for spi select
  gpio_mode_setup(
    GPIOA,
    GPIO_MODE_OUTPUT,
    GPIO_PUPD_NONE,
    GPIO15
  );

  // Setup GPIO pins for AF5 for SPI1 signals
  gpio_mode_setup(
    GPIOB,
    GPIO_MODE_AF,
    GPIO_PUPD_NONE,
    GPIO3 | GPIO4 | GPIO5
  );
  gpio_set_af(
    GPIOB,
    GPIO_AF0,
    GPIO3 | GPIO4 | GPIO5
  );

  // Start with spi communication disabled
  { spi_select; }


  //spi initialization;
  spi_set_master_mode         (SPI1);
  spi_set_baudrate_prescaler  (SPI1, SPI_CR1_BR_FPCLK_DIV_64);
  spi_set_clock_polarity_0    (SPI1);
  spi_set_clock_phase_0       (SPI1);
  spi_set_full_duplex_mode    (SPI1);
  spi_set_unidirectional_mode (SPI1); // bidirectional but in 3-wire
  spi_set_data_size           (SPI1, SPI_CR2_DS_8BIT);
  spi_enable_software_slave_management(SPI1);
  spi_send_msb_first          (SPI1);
  spi_set_nss_high            (SPI1);
  // spi_enable_ss_output(SPI1);
  spi_fifo_reception_threshold_8bit(SPI1);
  SPI_I2SCFGR(SPI1) &= ~SPI_I2SCFGR_I2SMOD;
  spi_enable(SPI1);
}


static const int capacity   = 0x80;
// static const int page_size  = 16;

// Commands supported by all chips
#define EEPROM_CAT25_COMMAND_READ   0x03
#define EEPROM_CAT25_COMMAND_WRITE  0x02
#define EEPROM_CAT25_COMMAND_RDSR   0x05
#define EEPROM_CAT25_COMMAND_WRSR   0x01
#define EEPROM_CAT25_COMMAND_WREN   0x06
#define EEPROM_CAT25_COMMAND_WRDI   0x04


void eeprom_write_enable() {
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_WREN);
}

void eeprom_write_disable() {
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_WRDI);
}

EEPROM_CAT25_Status_Register eeprom_status_register() {
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_RDSR);
  return (EEPROM_CAT25_Status_Register)spi_read8(SPI1);
}

bool eeprom_is_ready() {
  return (eeprom_status_register(). bit.nRDY == 0);
}

bool eeprom_wait_for_ready() {
  while (!eeprom_is_ready()) {};
  return eeprom_is_ready();
}

uint8_t eeprom_read_byte  (uint8_t address) {
  if (address >= capacity)
    return 0;
  eeprom_wait_for_ready();
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_READ);
  spi_send8(SPI1, address);
  return spi_read8(SPI1);
}

uint8_t eeprom_write_byte (uint8_t address, uint8_t byte) {
  if (address >= capacity)
    return 0;
  eeprom_wait_for_ready();
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_WRITE);
  spi_send8(SPI1, address);
  spi_send8(SPI1, byte);
  return 1;
}

uint8_t eeprom_read_buff (uint8_t address, uint8_t length, uint8_t* buffer) {
  if (address >= capacity)
    return 0;
  eeprom_wait_for_ready();
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_WRITE);
  spi_send8(SPI1, address);
  for (uint32_t i = 0; i < length; i++)
    buffer[i] = spi_read8(SPI1);
  return length;
}

uint8_t eeprom_write_buff(uint8_t address, uint8_t length, uint8_t* buffer) {
  if (address >= capacity)
    return 0;
  eeprom_wait_for_ready();
  spi_select;
  spi_send8(SPI1, EEPROM_CAT25_COMMAND_WRITE);
  spi_send8(SPI1, address);
  for (uint32_t i = 0; i < length; i++)
    spi_send8(SPI1, buffer[i]);
  return 1;
}
