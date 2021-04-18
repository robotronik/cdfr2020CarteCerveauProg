#include "tof_timer.h"


extern volatile VL53L0X_DEV* t_dev;

void timer_setup_interrupt(){
    _timer_setup(TOF_TIM_RCC,TOF_TIM,TOF_TIM_PRESCALER,TOF_TIM_PERIOD*2);

    nvic_enable_irq(TOF_TIM_NVIC);
	timer_enable_irq(TOF_TIM, TOF_TIM_DIER_UIE);

    _timer_start(TOF_TIM);
}

void tim4_isr(){
    if (timer_get_flag(TOF_TIM, TOF_TIM_SR_UIF))
	{
        timer_clear_flag(TOF_TIM, TOF_TIM_SR_UIF);
        timer_get_flag(TOF_TIM, TOF_TIM_SR_UIF);
        //Insert your code here
        //It will be executed when the interruption occurs
        // fprintf(stderr,"interrupt from timer\n");
        // gpio_toggle(GPIOA,GPIO5);
        tof_perform_measure(t_dev[0]);
        // fprintf(stderr,"Measure Performed ! range: %d\n",t_dev[0]->range);
	}
}

