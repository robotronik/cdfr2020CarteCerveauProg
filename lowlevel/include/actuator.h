/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the setup of the actuators: the arm and the flag
 * 
 * @date  07/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author PhenixRobotik NPXav Benano Trukbidule
*/

#pragma once

#include "gpio.h"
#include "timer.h"

/**
 * Prescale 64000000 Hz system clock by 64 = 1000000 Hz.
 */
#define PWM_PRESCALE	(64)

/**
 * We need a 50 Hz period (1000 / 20ms = 50), thus divide 100000 by 50 = 20000 (us).
 */
#define PWM_PERIOD		(20000)

/**
 * @defgroup ACTUATOR_TIM actuator_tim
 * @{
 * @brief Internal timer used to pilot the motors of the actuators with a PWM. Both use TIM3
 * 
 * Two channels are used for the ARM and FLAG
 */
#define ACTUATOR_TIM_RCC				RCC_TIM3
#define ACTUATOR_TIM					TIM3
/** @} */

/**
 * @defgroup ARM arm
 * @{
 * @brief Definitions for the arm
 *
 * EN stands for enable (output of the PWM signal) \n
 * We use OC_ID to select a specific channel of the output comparator as a PWM_output\n
 * DIR_1/2 stands for direction (boolean value) \n
 * INIT_DIR is the initial direction of the motor
 * INVERT_DIR allows to define the forward direction in motor_set (must be 1 or -1)
 * Pinmap used here: EN on PC7 (with TIM3_CH2), DIR_1 on PB12, DIR_2 on PB13
 */
#define ARM_GPIO_RCC_EN			RCC_GPIOC
#define ARM_PORT_EN				GPIOC
#define ARM_PIN_EN				GPIO7

#define ARM_AF					GPIO_AF2
#define ARM_OC_ID				TIM_OC2
#define ARM_OC_MODE				TIM_OCM_PWM1

#define ARM_GPIO_RCC_DIR_1		RCC_GPIOB
#define ARM_PORT_DIR_1			GPIOB
#define ARM_PIN_DIR_1			GPIO12

#define ARM_GPIO_RCC_DIR_2		RCC_GPIOB
#define ARM_PORT_DIR_2			GPIOB
#define ARM_PIN_DIR_2			GPIO13

#define ARM_INIT_DIR			0
#define ARM_INVERT_DIR          (-1)
/** @} */

/**
 * @defgroup ARM arm
 * @{
 * @brief Definitions for the flag
 *
 * EN stands for enable (output of the PWM signal) \n
 * We use OC_ID to select a specific channel of the output comparator as a PWM_output\n
 * DIR_1/2 stands for direction (boolean value) \n
 * INIT_DIR is the initial direction of the motor
 * INVERT_DIR allows to define the forward direction in motor_set (must be 1 or -1)
 * Pinmap used here: EN on PC6 (with TIM3_CH1), DIR_1 on PB14, DIR_2 on PB15
 */
#define FLAG_GPIO_RCC_EN		RCC_GPIOC
#define FLAG_PORT_EN			GPIOC
#define FLAG_PIN_EN				GPIO6

#define FLAG_AF					GPIO_AF2
#define FLAG_OC_ID				TIM_OC1
#define FLAG_OC_MODE			TIM_OCM_PWM1

#define FLAG_GPIO_RCC_DIR_1		RCC_GPIOB
#define FLAG_PORT_DIR_1			GPIOB
#define FLAG_PIN_DIR_1			GPIO14

#define FLAG_GPIO_RCC_DIR_2		RCC_GPIOB
#define FLAG_PORT_DIR_2			GPIOB
#define FLAG_PIN_DIR_2			GPIO15

#define FLAG_INIT_DIR			0
#define FLAG_INVERT_DIR         (-1)
/** @} */

/**
 * @brief enum of the actuators, used to identify them in some functions (like function actuators_set)
 * 
 */
enum actuator_sel {ARM, FLAG};

/**
 * @brief This function initializes the timers (including the timer output comparator) and GPIOs to pilot by PWM the propulsion motors + the GPIOs for the direction
 * 
 */
void actuator_setup();

/**
 * @brief This function pilots the sel with a value between -100(backward full speed) and +100 (forward full speed). The forward direction depends on the sign of ACT_X_INVER_DIR.
 * 
 * @param sel    The actuator that will be piloted (eg ARM) 
 * @param value    value is between -100 and +100, controls the speed and direction of the motor sel (eg +54)
 */
void actuator_set(enum actuator_sel sel, int8_t value);
