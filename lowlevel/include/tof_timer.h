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

/**
 * Prescale 84000000 Hz system clock by 84 = 1000000 Hz.
 */
#define TOF_TIM_PRESCALE	(84)

/**
 * Period for the Timer [us]
 */
#define TOF_TIM_PERIOD		(500000)

/**
 * @defgroup TOF_TIM tof_tim
 * @{
 * @brief Internal timer that will generate interrupts to get tof sensor measurement TIM4
 * DIER: DMA/INterrupt enable register (we use an interrupt)
 * SR: Status Register
 * CC1IE: capture and compare interrupt enable (the number is the channel of the timer)
 * CC1IF: capture and compare interrupt flag
 */
#define TOF_TIM_RCC				RCC_TIM4
#define TOF_TIM					TIM4
#define TOF_TIM_DIER_CCIE       TIM_DIER_CC1IE
#define TOF_TIM_SR_CCIF         TIM_SR_CC1IF
/** @} */

//note: the TIM_SR_CC4OF flag is set if two interrupts occured while the first flag
//was not cleared, this can be implemented for maximum safety later. 

/**
 * @defgroup TOF_TIM tof_tim
 * @{
 * @brief timer parameters
 * OC_MODE: "FROZEN" no output on GPIO in this use case
 */
#define TOF_TIM_OC_ID       TIM_OC1
#define TOF_TIM_OC_MODE     TIM_OCM_FROZEN 
#define TOF_TIM_NVIC        NVIC_TIM4_IRQ
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