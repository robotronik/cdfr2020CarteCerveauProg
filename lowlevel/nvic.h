#pragma once

#include "libopencm3/cm3/nvic.h" 
#include "libopencm3/stm32/exti.h" 
#include "libopencm3/stm32/gpio.h"
#include "lowlevel/gpio.h"
#include "libopencm3/stm32/rcc.h"


void nvic_setup();
void interrupt_gpio_setup();