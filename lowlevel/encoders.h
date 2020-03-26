#pragma once

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

//number of ticks per turn, encoder 300CPR
// on compte les fronts
#define ENCODER_PERIOD (300*4-1)
// GPIO_PUPD_PULLUP GPIO_PUPD_PULLDOWN GPIO_PUPD_NONE
#define ENCODER_INPUT_CFG GPIO_PUPD_NONE

/*******************************
Left encoder configuration
*******************************/
#define ENCODER_L_RCC_TIM RCC_TIM1
#define ENCODER_L_TIM TIM1

#define ENCODER_L_INVERSION 0

#define ENCODER_L_CH1_PORT GPIOA
#define ENCODER_L_CH1_PORT_RCC RCC_GPIOA
#define ENCODER_L_CH1_AF GPIO_AF6
#define ENCODER_L_CH1_PIN GPIO8

#define ENCODER_L_CH2_PORT GPIOA
#define ENCODER_L_CH2_PORT_RCC RCC_GPIOA
#define ENCODER_L_CH2_AF GPIO_AF6
#define ENCODER_L_CH2_PIN GPIO9

/*******************************
Right encoder configuration
*******************************/
#define ENCODER_R_RCC_TIM RCC_TIM2
#define ENCODER_R_TIM TIM2

#define ENCODER_R_INVERSION 0

#define ENCODER_R_CH1_PORT GPIOA
#define ENCODER_R_CH1_PORT_RCC RCC_GPIOA
#define ENCODER_R_CH1_AF GPIO_AF1
#define ENCODER_R_CH1_PIN GPIO0

#define ENCODER_R_CH2_PORT GPIOA
#define ENCODER_R_CH2_PORT_RCC RCC_GPIOA
#define ENCODER_R_CH2_AF GPIO_AF1
#define ENCODER_R_CH2_PIN GPIO1


void encoder_left_setup(void);
void encoder_right_setup(void);
int encoder_left_get_counter(void);
int encoder_right_get_counter(void);

// delta steps in small time difference
int encoder_left_update(volatile int *prev_count);
int encoder_right_update(volatile int *prev_count);
