#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>


#define PWM_LED_TIM           TIM2
#define PWM_LED_RCC_TIM       RCC_TIM2
#define PWM_LED_TIM_PERIOD    1000
#define PWM_LED_TIM_PRESCALER 1

#define GPIO_LED_PORT_RCC RCC_GPIOA
#define GPIO_LED_PORT GPIOA
#define GPIO_LED_PIN  GPIO0
#define GPIO_LED_PIN_OC TIM_OC1 // what is TIM_OC1N ?
#define GPIO_LED_PIN_OCM_PWM TIM_OCM_PWM1
#define GPIO_LED_AF GPIO_AF1
#define GPIO_BLUEBUTTON_PORT GPIOC
#define GPIO_BLUEBUTTON_PIN GPIO13




void gpio_setup();
void initTimer();
void pwmSetup();

void led_set_status(uint32_t status); // PWM ?
void led_toggle_status();
int read_bluebutton();

/* void set_all_1(); */
/* void set_all_0(); */


