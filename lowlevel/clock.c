#include "clock.h"

void clock_setup() {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
    // SystemCoreClock = ((INPUT_CLOCK (HSE_OR_HSI_IN_HZ) / PLL_M) * PLL_N) / PLL_P
  
  	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  	
  	/* Interrupts each millisec */
  	systick_set_reload(rcc_ahb_frequency/ 1000 - 1);
  	
  	/* clear counter so it starts right away */
  	systick_clear();
  	systick_interrupt_enable();
  	systick_counter_enable();
}

volatile uint32_t _systicks = 0;

void sys_tick_handler() {
  _systicks++;
}

uint32_t _clock_get_systicks() {
  return _systicks;
}

void delay_ms(uint32_t ms) {
  uint32_t count_max = _clock_get_systicks() + ms;
	while(_clock_get_systicks() < count_max);
}
