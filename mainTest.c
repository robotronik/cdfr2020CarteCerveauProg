#include <stdio.h>

#include "clock.h"
#include "actuator.h"
#include "uart.h"
#include "exti.h"
#include "i2c.h"

#include <libopencm3/stm32/i2c.h>

void test_actuator();
void test_com();
void test_i2c();

int main() {
    
    //setup
    clock_setup();
    exti_setup();
    actuator_setup();
    uart_setup();

    /*
    test_actuator();
    while(1){
        //test_actuator();
        fprintf(stderr,"un message de test\n");
        delay_ms(1000);
    }
    */

    test_i2c();
}

void test_actuator(){
    actuator_set(ARM, 80);
    actuator_set(FLAG, 10);
}

void test_com(){
    //uart_send_string("super nul");
    //printf("coucou \n");
}

void test_i2c(){
    //Setup SDA
    _gpio_setup_pin_af(RCC_GPIOB, GPIOB, GPIO7, GPIO_AF4);
    //Setup SCL
    _gpio_setup_pin_af(RCC_GPIOB, GPIOB, GPIO6, GPIO_AF4);
    
    i2c_setup(I2C1);

    uint8_t data = 0xab;

    while(1){
        i2c_write7(I2C1, 0x52, &data, 1); 	
        delay_ms(1000);
    }
}
