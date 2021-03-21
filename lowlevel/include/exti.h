/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the setup of the sensors linked to the actuators: 
 * the arm and the flag
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
#include <stdio.h>

#include "libopencm3/stm32/exti.h"
#include "libopencm3/cm3/nvic.h"

#include "gpio.h"
#include "actuator.h"

/**
 * @defgroup ARM_LIMITSWITCH arm_limit_switch
 * @{
 * @brief INterruption when the actuator reaches limit switch
 * 
 * EXTI: External Interrupt, peripheral that is linked to a pin and generates interrupts
 * NVIC: Nested vectored interrupt controller. It is a table that makes the link
 * between the interruption event and the code (interrupt routine) to execute
 * PRIORITY: from 0 to 255 in steps of 16 (for the time being not really important)
 */
#define ARM_LIMITSWITCH_RCC     RCC_GPIOC
#define ARM_LIMITSWITCH_PORT    GPIOC
#define ARM_LIMITSWITCH_PIN     GPIO9
#define ARM_NVIC_INTERRUPT_NUMBER   NVIC_EXTI9_5_IRQ
#define ARM_LIMITSWITCH_EXTI    EXTI9
#define ARM_PRIORITY            (3*16)
/** @} */

/**
 * @defgroup FLAG_LIMITSWITCH flag_limit_switch
 * @{
 * @brief Interruption when the actuator is done(touches the limitswitch)
 * EXTI: External Interrupt, peripheral that is linked to a pin and generates interrupts
 * NVIC: Nested vectored interrupt controller. It is a table that makes the link
 * between the interruption event and the code (interrupt routine) to execute
 * PRIORITY: from 0 to 255 in steps of 16 (for the time being not really important)
 */
#define FLAG_LIMITSWITCH_RCC        RCC_GPIOC
#define FLAG_LIMITSWITCH_PORT       GPIOC
#define FLAG_LIMITSWITCH_PIN        GPIO8
#define FLAG_NVIC_INTERRUPT_NUMBER  NVIC_EXTI9_5_IRQ
#define FLAG_LIMITSWITCH_EXTI       EXTI8
#define FLAG_PRIORITY               (4*16)
/** @} */

/**
 * @brief This function initializes the exti interrupt and nvic
 * interrupts will be received from gpio_port with the pin matching the number
 * of the exti
 *
 * @param exti the external interrupt peripheral linked to the gpio pin (number
 * must match !)
 * @param gpio_port the port on which the limit switch will be plugged
 * @param interrupt_number the interrupt number in the NVIC table
 * @param trig the type of event that will trigger the interrupt
 * (rising,falling,both)
 */
void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,enum exti_trigger_type trig);

/**
 * @brief Initialize the GPIO and interrupts for the limit switch of the FLAG
 */
void _flag_limit_switch_init();

/**
 * @brief Initialize the GPIO and interrupts for the limit switch of the ARM
 */
void _arm_limit_switch_init();

/**
 * @brief initialize the peripheral that managed the exti line (syscfg)
 * 
 * @warning We assume you already setup your actuator and uart
 */
void exti_setup();

/**
 * @brief interrupt routine for interrution of exti 9 to exti 5
 * 
 * @warning You may need to edit this function to change the interrupt routine 
 * for the given functionality
 */
void exti9_5_isr();
