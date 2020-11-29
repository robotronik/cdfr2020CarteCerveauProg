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
void blink_led();

int main() {
    
    //setup
    clock_setup();
    //exti_setup();
    //actuator_setup();
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
    //blink_led();

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
    i2c_setup(I2C1);
    uint32_t data = 0xabcdef ;
    uint16_t addr = 4;
    I2C_status my_I2C_return;

    delay_ms(200);
    my_I2C_return = i2c_write7(I2C1, addr, &data, sizeof(data));
    fprintf(stderr,"I2Cret = %d\n",my_I2C_return);
    delay_ms(50);
    data = 0xfedcba;
    my_I2C_return = i2c_write7(I2C1, addr, &data, sizeof(data));
    fprintf(stderr,"I2Cret = %d\n",my_I2C_return);
    while(1){
        delay_ms(1000);
    }
}

void blink_led(){
    //led is on PA5
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_OTYPE_PP);
    
    while(1){
        gpio_toggle(GPIOA,GPIO5);
        delay_ms(50);
    }
}