/**
 * @file
 * 
 * This file is part of cdfr2020CarteCerveauProg
 * 
 * @brief This implements the setup of the sensors linked to the actuators: the
 * arm and the flag
 *
 * @date  09/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/

#include "exti.h"

void exti_setup(){
    rcc_periph_clock_enable(RCC_SYSCFG);
}

void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,
        enum exti_trigger_type trig){
    //To be sure
    exti_disable_request(exti);


    //enable the entry in the vector table of interruption (this table says
    //hey there is an interrupt now you must go there to see which code to 
    //execute)
    nvic_enable_irq(interrupt_number);

    //plug the exti with the right gpio port
    exti_select_source(exti,gpio_port);

    //choose the type of event that will trigger the exti
    exti_set_trigger(exti,trig);
    
    //TODO commentaire a ecrire
    exti_enable_request(exti);
}

void _arm_limit_switch_init(){
    _gpio_setup_pin(ARM_LIMITSWITCH_RCC,ARM_LIMITSWITCH_PORT,ARM_LIMITSWITCH_PIN,GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    _limit_switch_init(ARM_LIMITSWITCH_EXTI,ARM_LIMITSWITCH_PORT,ARM_NVIC_INTERRUPT_NUMBER, EXTI_TRIGGER_FALLING);
    //nvic_set_priority(ARM_NVIC_INTERRUPT_NUMBER, ARM_PRIORITY);
}

void _flag_limit_switch_init(){
    _gpio_setup_pin(FLAG_LIMITSWITCH_RCC,FLAG_LIMITSWITCH_PORT,FLAG_LIMITSWITCH_PIN,GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    _limit_switch_init(FLAG_LIMITSWITCH_EXTI,FLAG_LIMITSWITCH_PORT,FLAG_NVIC_INTERRUPT_NUMBER, EXTI_TRIGGER_FALLING);
    //nvic_set_priority(FLAG_NVIC_INTERRUPT_NUMBER, FLAG_PRIORITY);
}

void exti9_5_isr(){
    if(exti_get_flag_status(ARM_LIMITSWITCH_EXTI)){
        //TO DO: things to do when arm limit switch is pushed
        fprintf(stderr,"interrupt received from arm\n");       
        
        actuator_set(ARM,0);

        exti_reset_request(ARM_LIMITSWITCH_EXTI);
    }
    if(exti_get_flag_status(FLAG_LIMITSWITCH_EXTI)){
        //TO DO: things to do when flag limit switch is pushed
        fprintf(stderr,"interrupt received from flag\n");       
        actuator_set(FLAG,0);
        exti_reset_request(FLAG_LIMITSWITCH_EXTI);
    }
}
