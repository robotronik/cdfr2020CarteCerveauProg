#include "nvic.h"

void interrupt_gpio_setup(){
//exti_disable_request(EXTI13);
rcc_periph_clock_enable(RCC_SYSCFG);

exti_select_source(EXTI13,GPIOC);
exti_set_trigger(EXTI13,EXTI_TRIGGER_FALLING);
exti_enable_request(EXTI13);
nvic_enable_irq(NVIC_EXTI15_10_IRQ);

// configuration with the blue button pin 13 on port GPIO C

}

void exti15_10_isr(){

    if(exti_get_flag_status(EXTI13)){
    led_set_status(1);
    exti_reset_request(EXTI13);
    }

}