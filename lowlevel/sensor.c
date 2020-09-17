/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the setup of the sensors linked to the actuators: the arm and the flag
 *
 * @date  09/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/

#include "sensor.h"

void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,enum exti_trigger_type trig){
    exti_select_source(exti,gpio_port);
    exti_set_trigger(exti,trig);
    nvic_enable_irq(interrupt_number);
}
void arm_limit_switch_init(){
    _gpio_setup_pin(ARM_LIMITSWITCH_RCC,ARM_LIMITSWITCH_PORT,ARM_LIMITSWITCH_PIN,GPIO_MODE_INPUT);
    _limit_switch_init(ARM_LIMITSWITCH_EXTI,ARM_LIMITSWITCH_PORT,ARM_NVIC_INTERRUPT_NUMBER, EXTI_TRIGGER_RISING);
}

void flag_limit_switch_init(){
    _gpio_setup_pin(FLAG_LIMITSWITCH_RCC,FLAG_LIMITSWITCH_PORT,FLAG_LIMITSWITCH_PIN,GPIO_MODE_INPUT);
    _limit_switch_init(ARM_LIMITSWITCH_EXTI,ARM_LIMITSWITCH_PORT,FLAG_NVIC_INTERRUPT_NUMBER, EXTI_TRIGGER_RISING);
}

void exti9_5_isr(){
    if(exti_get_flag_status(ARM_LIMITSWITCH_EXTI)){
        //TO DO: things to do when arm limit switch is pushed
        
        exti_reset_request(ARM_LIMITSWITCH_EXTI);
    }
    if(exti_get_flag_status(FLAG_LIMITSWITCH_EXTI)){
        //TO DO: things to do when flag limit switch is pushed
        exti_reset_request(FLAG_LIMITSWITCH_EXTI);
    }
}
