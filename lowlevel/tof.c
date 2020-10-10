#include "tof.h"

void tof_setup(){
    //TODO

    //setup pin for SR
    _gpio_setup_pin(SR_DSAB_RCC, SR_DSAB_PORT, SR_DSAB_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN);
    _gpio_setup_pin(SR_CP_RCC, SR_CP_PORT, SR_CP_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN);
}

void tof_reset(){
    gpio_set(SR_DSAB_PORT, SR_DSAB_PIN);
    __pulse(SR_CP_PORT, SR_CP_PIN);
    gpio_clear(SR_DSAB_PORT, SR_DSAB_PIN);
    for(int i =0; i<TOF_NUM-1;++i){
        __pulse(SR_CP_PORT, SR_CP_PIN);
    }
}
