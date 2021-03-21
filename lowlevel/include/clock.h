/**
 * @file
 * 
 * This file is part of cdfr2020CerveauProg
 * 
 * @brief This implements the setup of the system clock, acces function (debug) and temporal fonction (delay)  
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


#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

/**
 * @brief This function setup the system clock
 * 
 */
void clock_setup();

/**
 * @brief This function gets the number of systicks since starting
 */
uint32_t _clock_get_systicks();

//uint32_t get_uptime_ms();
//uint32_t get_frequency();
//uint32_t get_systicks();

/**
 * @brief This function implements a delay in ms 
 * @param ms    value of delay in ms
 */
void delay_ms(uint32_t ms);
