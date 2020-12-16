// reference used code by Ken Sarkies ksarkies@trinity.asn.au

#include "tof_timer.h"
#include "timer.h"




void timer_setup_interrupt(){
    _timer_setup(TOF_TIM_RCC,TOF_TIM,TOF_TIM_PRESCALE,TOF_TIM_PERIOD);
    _timer_setup_output_c(TOF_TIM,TOF_TIM_OC_ID,TOF_TIM_OC_ID,0);

    timer_disable_oc_output(TOF_TIM, TIM_OC1 | TIM_OC2 | TIM_OC3 | TIM_OC4);
    

    nvic_enable_irq(TOF_TIM_NVIC);
	timer_enable_irq(TOF_TIM, TIM_DIER_CC1IE);
    timer_enable_counter(TOF_TIM);
}

void tim4_isr(){
    if (timer_get_flag(TOF_TIM, TIM_SR_CC1IF))
	{
        //do something

		timer_clear_flag(TOF_TIM, TIM_SR_CC1IF);

	}

}

