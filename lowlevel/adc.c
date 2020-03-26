#include "theyseemerolling.h"
#include "clock.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>


void adcs_setup() {
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_ADC);

  gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
  gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);


  // ADC Setup
  adc_power_off(ADC1);
  adc_set_continuous_conversion_mode(ADC1);
  adc_set_right_aligned(ADC1);

  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_61DOT5CYC);
  adc_set_resolution(ADC1, ADC_CFGR1_RES_12_BIT);

  uint8_t channel_seq[16] = { 1, 2 };
  adc_set_regular_sequence(ADC1, 2, channel_seq);

  adc_power_on(ADC1);

  delay_ms(1);
  adc_calibrate(ADC1);
  delay_ms(1);
}

uint32_t adc_a, adc_b;
void adc_read_all() {
  adc_a = adc_read_regular(ADC1);
  adc_b = adc_read_regular(ADC1);
}

uint32_t adc_a_value() {
  adc_read_all();
  return adc_a;
}

uint32_t adc_b_value() {
  adc_read_all();
  return adc_b;
}

double convert_adc_to_milliamps(uint32_t adc_value) {
  // ACS711KLCTR-25AB-T : 0.055V/A
  const double acs_ma_by_mv = 1/0.055;
  const double mv_by_unit = 3300.0 / 4096; // 8.056mV
  double mv = 1.0 * adc_value * mv_by_unit;
  double ma = mv * acs_ma_by_mv;
  return ma;
}
