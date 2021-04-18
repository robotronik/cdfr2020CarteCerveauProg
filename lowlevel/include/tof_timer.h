/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements a routine for periodic call of a function
 * 
 * @see reference used code by Ken Sarkies ksarkies@trinity.asn.au:
 * https://github.com/ksarkies/ARM-Ports/blob/master/test-libopencm3-stm32f1/timer-interrupt-oc-et-stm32f103.c
 * 
 * @date  12/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano
*/


#pragma once

#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "timer.h"
#include "uart.h"
#include "tof.h"

/**
 * Prescale 84000000 Hz system clock by 84000 = 1000 Hz.
 */
// TODO: cannot go over 2**16 with the prescale value despite the function accepting uint32 , investigate
#define TOF_TIM_PRESCALER	(42000)

/**
 * Period for the Timer [ms]
 */
#define TOF_TIM_PERIOD		(50)

/**
 * @defgroup TOF_TIM tof_tim
 * @{
 * @brief Internal timer that will generate interrupts to get tof sensor measurement TIM4
 * DIER: DMA/INterrupt enable register (we use an interrupt)
 * SR: Status Register
 * UI: Update interrupt
 */
#define TOF_TIM_RCC				RCC_TIM4
#define TOF_TIM					TIM4
#define TOF_TIM_NVIC            NVIC_TIM4_IRQ
#define TOF_TIM_DIER_UIE        TIM_DIER_UIE
#define TOF_TIM_SR_UIF          TIM_SR_UIF
/** @} */

/**
 * @brief setup a regular interruption routine
 * 
 */
void timer_setup_interrupt();

/**
 * @brief definition of the interrupt routine
 * 
 * @warning You may need to edit this function to change the interrupt routine 
 * for the given functionality
 */
void tim4_isr();