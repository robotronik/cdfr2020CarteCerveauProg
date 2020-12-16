#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

/**
 * Prescale 84000000 Hz system clock by 64 = 1000000 Hz.
 */
#define TOF_TIM_PRESCALE	(84)

/**
 * We need a 50 Hz period (1000 / 20ms = 50), thus divide 100000 by 50 = 20000 (us).
 */
#define TOF_TIM_PERIOD		(20000)

/**
 * @defgroup TOF_TIM tof_tim
 * @{
 * @brief Internal timer that will generate interrupts to get tof sensor measurement TIM4
 * 
 */
#define TOF_TIM_RCC				RCC_TIM4
#define TOF_TIM					TIM4
/** @} */


#define TOF_TIM_OC_ID          TIM_OC1
#define TOF_TIM_OC_MODE     TIM_OCM_FROZEN 
#define TOF_TIM_NVIC        NVIC_TIM4_IRQ