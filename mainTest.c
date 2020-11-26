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
    //Setup SCL
    // _gpio_setup_pin_af(RCC_GPIOB, GPIOB, GPIO6, GPIO_AF4);
    // //Setup SDA
    // _gpio_setup_pin_af(RCC_GPIOB, GPIOB, GPIO7, GPIO_AF4);
    
 /* Enable clock on port*/
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Setup pin as altfun and no pull up or pull down */
    gpio_mode_setup(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO6);
    
    /* Setup pin as push-pull with a default speed of 50 MHz */
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ, GPIO6); 

    /* Setup alternate function */    
    gpio_set_af(GPIOB, GPIO_AF4, GPIO6);

        /* Setup pin as altfun and no pull up or pull down */
    gpio_mode_setup(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO7);
    
    /* Setup pin as push-pull with a default speed of 50 MHz */
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ, GPIO7); 

    /* Setup alternate function */    
    gpio_set_af(GPIOB, GPIO_AF4, GPIO7);





    delay_ms(200);
    i2c_setup(I2C1);
    delay_ms(100);
    uint32_t data = 0xabcdef ;
     uint16_t addr = 4;
    while(1){
        i2c_write7_v1(I2C1, addr, &data, sizeof(data)); 	
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