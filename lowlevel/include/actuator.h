/**
 * @file
 * 
 * This file is part of cdfr2020CerveauProg
 * 
 * @brief This implements the setup of the actuators
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
 * @brief enum of the actuators, used to identify them in some functions (like function actuators_set)
 * 
 */
enum actuator_sel {ARM_A, ARM_B};

/**
 * @brief This function initializes the timers (including the timer output comparator) and GPIOs to pilot by PWM the propulsion motors + the GPIOs for the direction
 * 
 */
void actuator_setup();

/**
 * @brief This function pilots the sel with a value between -100(backward full speed) and +100 (forward full speed). The forward direction depends on the sign of ACT_X_INVER_DIR.
 * 
 * @param sel    The actuator that will be piloted (eg ARM_A) 
 * @param value    value is between -100 and +100, controls the speed and direction of the motor sel (eg +54)
 */
void actuator_set(enum actuator_sel sel, int8_t value);
