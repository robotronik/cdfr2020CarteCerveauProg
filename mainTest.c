#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "actuator.h"
#include "uart.h"
#include "exti.h"
#include "i2c.h"
#include "vl53l0x_platform.h"

#include <libopencm3/stm32/i2c.h>

void test_actuator();
void test_com();
void test_i2c();
void blink_led();
void test_tof_platform_write();
void test_tof_platform_read();
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
    
    
    //blink_led();
    //test_tof_platform_write();
    // test_i2c();
    test_tof_platform_read();
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
    // my_I2C_return = i2c_write7(I2C1, addr, (uint8_t*)&data, sizeof(data));
    // fprintf(stderr,"I2Cret = %d\n",my_I2C_return);
    // delay_ms(50);
    // data = 0xfedcba;
    // my_I2C_return = i2c_write7(I2C1, addr,(uint8_t*)&data, sizeof(data));
    // fprintf(stderr,"I2Cret = %d\n",my_I2C_return);

    uint16_t dataRead = 0;

    while(1){
        delay_ms(20);
        i2c_read7(I2C1,addr,&dataRead,2);
        //fprintf(stderr,"dataRead=%04X \r\n",dataRead);
    }
}

void test_tof_platform_write(){
    i2c_setup(I2C1);
    VL53L0X_DEV pDev = calloc(1,sizeof(*pDev));
    pDev->i2c_dev = I2C1;
    pDev->i2c_slave_address = 4;
    delay_ms(200);

    uint8_t index = 0xcd;
    uint8_t dataByte = 0xab;
    VL53L0X_Error myError = VL53L0X_WrByte(pDev,index,dataByte);
    delay_ms(30);

    uint16_t dataWord = 0xa1b2;
    myError = VL53L0X_WrWord(pDev,index,dataWord);
    delay_ms(30);

    uint32_t dataDWord = 0xa1b2e3f4;
    myError = VL53L0X_WrDWord(pDev,index,dataDWord);
    delay_ms(30);
    
    uint8_t bigDataWink[6] ={0xab,0xcd,0xef,0x12,0x13,0x14};
    myError = VL53L0X_WriteMulti(pDev,index,bigDataWink,6);
}

void test_tof_platform_read(){
    i2c_setup(I2C1);
    VL53L0X_DEV pDev = calloc(1,sizeof(*pDev));
    pDev->i2c_dev = I2C1;
    pDev->i2c_slave_address = 4;
    delay_ms(200);

    uint8_t index = 0xcd;
    VL53L0X_Error myError;

    uint8_t dataReadByte = 0;
    myError = VL53L0X_RdByte(pDev,index,&dataReadByte);
    delay_ms(200);

    uint16_t dataReadWord = 0;
    myError = VL53L0X_RdWord(pDev,index,&dataReadWord);
    delay_ms(200);

    uint32_t dataReadDWord = 0;
    myError = VL53L0X_RdDWord(pDev,index,&dataReadDWord);
    delay_ms(200);

    uint8_t bigDataRead[6];
    myError = VL53L0X_ReadMulti(pDev,index,&bigDataRead,6);
    delay_ms(200);

    fprintf(stderr,"Byte = %X \t Word = %X \t DWord = %X \t ",dataReadByte,dataReadWord,dataReadDWord,bigDataRead);
    fprintf(stderr,"Multi = ");
    for (int i=0;i<6;i++){
        fprintf(stderr,"%X",bigDataRead[5-i]);
    }
    fprintf(stderr,"\n");
}

void blink_led(){
    //led is on PA5
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_OTYPE_PP);
    
    while(1){
        gpio_toggle(GPIOA,GPIO5);
        delay_ms(50);
    }
}