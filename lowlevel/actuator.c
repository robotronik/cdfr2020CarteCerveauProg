#include "actuator.h"


/**
 * @brief This function initializes the GPIO and timers (including the timer output comparator) to pilot by PWM the actuators (flag and arm)
 * 
 */
void actuator_setup(){
    /* Setup one timer for the flag and arm */
    timer_setup(ACTUATOR_TIM_RCC, ACTUATOR_TIM, PWM_PRESCALE, PWM_PERIOD);

    /* setup arm EN*/
    gpio_setup_pin_af(ARM_GPIO_RCC_EN, ARM_PORT_EN, ARM_PIN_EN, ARM_AF);
    timer_setup_output_c(ACTUATOR_TIM, ARM_OC_ID, ARM_OC_MODE, 0);

    /* setup arm DIR*/
    rcc_periph_clock_enable(ARM_GPIO_RCC_DIR_1);
    gpio_mode_setup(ARM_PORT_DIR_1, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ARM_PIN_DIR_1);
    gpio_set_output_options(ARM_PORT_DIR_1, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, ARM_PIN_DIR_1);

    rcc_periph_clock_enable(ARM_GPIO_RCC_DIR_21);
    gpio_mode_setup(ARM_PORT_DIR_2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ARM_PIN_DIR_2);
    gpio_set_output_options(ARM_PORT_DIR_2, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, ARM_PIN_DIR_2);

    if(ARM_INIT_DIR){
        gpio_clear(ARM_PORT_DIR_1, ARM_PIN_DIR_1);
        gpio_set(ARM_PORT_DIR_2, ARM_PIN_DIR_2);
    } else {
        gpio_clear(ARM_PORT_DIR_2, ARM_PIN_DIR_2);
        gpio_set(ARM_PORT_DIR_1, ARM_PIN_DIR_1);
    }

    /* setup flag EN */
    gpio_setup_pin_af(FLAG_GPIO_RCC_EN, FLAG_PORT_EN, FLAG_PIN_EN, FLAG_AF);
    timer_setup_output_c(ACTUATOR_TIM, FLAG_OC_ID, FLAG_OC_MODE, 0);

    /* setup flag DIR*/
    rcc_periph_clock_enable(FLAG_GPIO_RCC_DIR_1);
    gpio_mode_setup(FLAG_PORT_DIR_1, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FLAG_PIN_DIR_1);
    gpio_set_output_options(FLAG_PORT_DIR_1, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, FLAG_PIN_DIR_1);

    rcc_periph_clock_enable(FLAG_GPIO_RCC_DIR_21);
    gpio_mode_setup(FLAG_PORT_DIR_2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FLAG_PIN_DIR_2);
    gpio_set_output_options(FLAG_PORT_DIR_2, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, FLAG_PIN_DIR_2);

    if(FLAG_INIT_DIR){
        gpio_clear(FLAG_PORT_DIR_1, FLAG_PIN_DIR_1);
        gpio_set(FLAG_PORT_DIR_2, FLAG_PIN_DIR_2);
    } else {
        gpio_clear(FLAG_PORT_DIR_2, FLAG_PIN_DIR_2);
        gpio_set(FLAG_PORT_DIR_1, FLAG_PIN_DIR_1);
    }

    /* start timer */
    timer_start(ACTUATOR_TIM);
}

/**
 * @brief This function pilots the sel (ARM, FLAG...) with a value between -100 (backward full speed) and +100 (forward full speed). The forward direction depends on the sign of ARM/FLAG_INVER_DIR.
 * 
 * @param sel    The motor that will be piloted (eg ARM) 
 * @param value    value is between -100 and +100, controls the speed and direction of the motor sel (eg +54)
 */
void actuator_set(enum actuator_sel sel, int8_t value){
    /*value should be between -100 and +100*/
    if(value < -100 || value > 100){
    	return;
    }

    int speed;
    switch(sel){
    	case ARM:
    		/* redefine forward direction */
    		value *= ARM_INVERT_DIR;
    	
    		/*direction is backward: set DIR to 1 and value become positive*/
    		if(value<0){
                gpio_set(ARM_PORT_DIR_1, ARM_PIN_DIR_1);
                gpio_clear(ARM_PORT_DIR_2, ARM_PIN_DIR_2);
                value *= -1;
    		}
    		/*direction is forward: set DIR to 0*/
    		else{
                gpio_clear(ARM_PORT_DIR_1, ARM_PIN_DIR_1);
                gpio_set(ARM_PORT_DIR_2, ARM_PIN_DIR_2);
    		}
    		/*calculate the up-time of EN between 0 and PWM_PERIOD*/
    		speed = value * PWM_PERIOD/100;
    		timer_set_oc_value(ACTUATOR_TIM, ARM_OC_ID, speed);
    		break;
    	case FLAG:
    		/* redefine forward direction */
    		value *= FLAG_INVERT_DIR;
    	
    		/*direction is backward: set DIR to 1 and value become positive*/
            if(value<0){
                gpio_set(FLAG_PORT_DIR_1, FLAG_PIN_DIR_1);
                gpio_clear(FLAG_PORT_DIR_2, FLAG_PIN_DIR_2);
                value *= -1;
    		}
    		/*direction is forward: set DIR to 0*/
    		else{
                gpio_clear(FLAG_PORT_DIR_1, FLAG_PIN_DIR_1);
                gpio_set(FLAG_PORT_DIR_2, FLAG_PIN_DIR_2);
    		}
    		/*calculate the up-time of EN between 0 and PWM_PERIOD*/
    		speed = value * PWM_PERIOD/100;
    		timer_set_oc_value(ACTUATOR_TIM, FLAG_OC_ID, speed);
    		break;
    }
}
