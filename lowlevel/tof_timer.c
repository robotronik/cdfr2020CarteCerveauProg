// reference used code by Ken Sarkies ksarkies@trinity.asn.au
//https://github.com/ksarkies/ARM-Ports/blob/master/test-libopencm3-stm32f1/timer-interrupt-oc-et-stm32f103.c
#include "tof_timer.h"

void timer_setup_interrupt(){
    _timer_setup(TOF_TIM_RCC,TOF_TIM,TOF_TIM_PRESCALE,TOF_TIM_PERIOD);
    _timer_setup_output_c(TOF_TIM,TOF_TIM_OC_ID,TOF_TIM_OC_MODE,0);
    //timer_disable_oc_clear(TOF_TIM, TOF_TIM_OC_ID);
    //timer_disable_preload(TOF_TIM);
    //timer_set_oc_slow_mode(TOF_TIM, TOF_TIM_OC_ID);
    nvic_enable_irq(TOF_TIM_NVIC);
	timer_enable_irq(TOF_TIM, TOF_TIM_DIER_CCIE);
    _timer_start(TOF_TIM);
}

void tim4_isr(){
    if (timer_get_flag(TOF_TIM, TOF_TIM_SR_CCIF))
	{
        //do something
        fprintf(stderr,"interrupt from timer\n");
        gpio_toggle(GPIOA,GPIO5);

		timer_clear_flag(TOF_TIM, TOF_TIM_SR_CCIF);
	}
}

