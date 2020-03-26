#pragma once

#include <stdbool.h>
#include <stdint.h>

// This API should be splitted to specific headers

/*****************************************************************************/
// Led,…
void gpio_setup();

void led_set_status(uint32_t status); // PWM ?
void led_toggle_status();
bool about_da_power();

void set_all_1();

/*****************************************************************************/
void can_setup();
// TODO !
void* can_get_message();
void can_send(void* msg);

/*****************************************************************************/
void adcs_setup();
// Value in ampers ?
uint32_t adc_a_value();
uint32_t adc_b_value();

/*****************************************************************************/
void encoders_setup();
void encoders_reset_tics();
// Should reset internal counters ?
int32_t encoder_1_get_tics();
int32_t encoder_2_get_tics();
