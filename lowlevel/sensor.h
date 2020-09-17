/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the setup of the sensors linked to the actuators: the arm and the flag
 * 
 * @date  07/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author PhenixRobotik NPXav Benano Trukbidule
*/

#pragma once

#include <stdint.h>

#include "libopencm3/stm32/exti.h"
#include "libopencm3/cm3/nvic.h"

#include "gpio.h"

#define ARM_LIMITSWITCH_RCC     RCC_GPIOC
#define ARM_LIMITSWITCH_PORT    GPIOC
#define ARM_LIMITSWITCH_PIN     GPIO9

#define ARM_NVIC_INTERRUPT_NUMBER   NVIC_EXTI9_5_IRQ
#define ARM_LIMITSWITCH_EXTI    EXTI9

#define FLAG_LIMITSWITCH_RCC     RCC_GPIOC
#define FLAG_LIMITSWITCH_PORT    GPIOC
#define FLAG_LIMITSWITCH_PIN     GPIO8

#define FLAG_NVIC_INTERRUPT_NUMBER   NVIC_EXTI9_5_IRQ
#define FLAG_LIMITSWITCH_EXTI    EXTI8


void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,enum exti_trigger_type trig);
void flag_limit_switch_init();
void arm_limit_switch_init();
