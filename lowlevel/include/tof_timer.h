#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "timer.h"
#include "uart.h"
/**
 * Prescale 84000000 Hz system clock by 84 = 1000000 Hz.
 */
#define TOF_TIM_PRESCALE	(84)

/**
 * Period for the Timer
 */
#define TOF_TIM_PERIOD		(500000)

/**
 * @defgroup TOF_TIM tof_tim
 * @{
 * @brief Internal timer that will generate interrupts to get tof sensor measurement TIM4
 * 
 */
#define TOF_TIM_RCC				RCC_TIM4
#define TOF_TIM					TIM4
#define TOF_TIM_DIER_CCIE       TIM_DIER_CC1IE
#define TOF_TIM_SR_CCIF         TIM_SR_CC1IF
/** @} */

//note: the TIM_SR_CC4OF flag is set if two interrupts occured while the first flag
//was not cleared, this can be implemented for maximum safety later. 

#define TOF_TIM_OC_ID       TIM_OC1
#define TOF_TIM_OC_MODE     TIM_OCM_FROZEN 
#define TOF_TIM_NVIC        NVIC_TIM4_IRQ

void timer_setup_interrupt();
void tim4_isr();