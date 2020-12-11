#include "tof.h"

void tof_setup(){
    //DONE setup i2c peripheral from benano 
    i2c_setup(I2C1);

    //setup pin for SR
    _gpio_setup_pin(SR_DSAB_RCC, SR_DSAB_PORT, SR_DSAB_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PP);
    _gpio_setup_pin(SR_CP_RCC, SR_CP_PORT, SR_CP_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PP);

    /* For more than one
    //Pulse reset on tof
    __pulse(SR_CP_PORT, SR_CP_PIN);

    // Wait for the chip to start
    delay_ms(TOF_DELAY);
    */

    s

    
}



void _shift_reg_init(){
    gpio_set(SR_DSAB_PORT, SR_DSAB_PIN);
    __pulse(SR_CP_PORT, SR_CP_PIN);
    gpio_clear(SR_DSAB_PORT, SR_DSAB_PIN);
}

void _shift_reg(int i){
    for(int j =0; j<i;++j){
        __pulse(SR_CP_PORT, SR_CP_PIN);
    }
}

void tof_reset(){
    _shift_reg_init();
    for(int i =0; i<TOF_NUM-1;++i){
        _shift_reg(1);
    }
}
