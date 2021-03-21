#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "actuator.h"
#include "uart.h"
#include "exti.h"
#include "i2c.h"
#include "vl53l0x_platform.h"
#include "tof.h"
#include "tof_timer.h"

void blink_led();
void test_actuator();
void test_tof_Single();
void test_tof_poke();
void test_i2c();
void test_xshut();
void test_tof_platform_write();
void test_tof_platform_read();
void test_interrupt_timer();

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


    // blink_led();
    // test_tof_platform_write();
    // test_i2c();
    // test_tof_platform_read();
    test_interrupt_timer();
    // while(1){
    //     test_tof_poke();
    //     delay_ms(500);
    // }
    // test_tof_Single();
    // test_xshut();

}

void blink_led(){
    //led is on PA5
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_OTYPE_PP);

    while(1){
        gpio_toggle(GPIOA,GPIO5);
        delay_ms(50);
    }
}

void test_actuator(){
    actuator_set(ARM, 80);
    actuator_set(FLAG, 10);
}

void test_i2c(){
    i2c_setup(I2C1);
    // uint32_t data = 0xabcdef ;
    uint16_t addr = 4;
    // I2C_status my_I2C_return;

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
        i2c_read7(I2C1,addr,(uint8_t*)(&dataRead),2);
        //fprintf(stderr,"dataRead=%04X \r\n",dataRead);
    }
}

void test_tof_platform_write(){
    i2c_setup(I2C1);
    VL53L0X_DEV pDev = calloc(1,sizeof(*pDev));
    pDev->i2c_dev = I2C1;
    pDev->i2c_slave_address = 0x52>>1; //4;
    delay_ms(200);

    uint8_t index = 0xc0;
    uint8_t dataByte = 0xab;
    VL53L0X_Error status = VL53L0X_WrByte(pDev,index,dataByte);
    fprintf(stderr,"Write Byte! error status: %d\n",status);
    delay_ms(30);

    uint16_t dataWord = 0xa1b2;
    status = VL53L0X_WrWord(pDev,index,dataWord);
    fprintf(stderr,"Write Word! error status: %d\n",status);
    delay_ms(30);

    uint32_t dataDWord = 0xa1b2e3f4;
    status = VL53L0X_WrDWord(pDev,index,dataDWord);
    fprintf(stderr,"Write Double Word! error status: %d\n",status);
    delay_ms(30);

    uint8_t bigDataWink[6] ={0xab,0xcd,0xef,0x12,0x13,0x14};
    status = VL53L0X_WriteMulti(pDev,index,bigDataWink,6);
    fprintf(stderr,"Write Multi! error status: %d\n",status);
}

void test_tof_platform_read(){
    i2c_setup(I2C1);
    VL53L0X_DEV pDev = calloc(1,sizeof(*pDev));
    pDev->i2c_dev = I2C1;
    pDev->i2c_slave_address = 4;
    delay_ms(200);

    uint8_t index = 0xcd;
    VL53L0X_Error status;

    uint8_t dataReadByte = 0;
    status = VL53L0X_RdByte(pDev,index,&dataReadByte);
    fprintf(stderr,"Read Byte! error status: %d\n",status);
    delay_ms(200);

    uint16_t dataReadWord = 0;
    status = VL53L0X_RdWord(pDev,index,&dataReadWord);
    fprintf(stderr,"Read Word! error status: %d\n",status);
    delay_ms(200);

    uint32_t dataReadDWord = 0;
    status = VL53L0X_RdDWord(pDev,index,&dataReadDWord);
    fprintf(stderr,"Read Double Word! error status: %d\n",status);
    delay_ms(200);

    uint8_t bigDataRead[6];
    status = VL53L0X_ReadMulti(pDev,index,(uint8_t*)(&bigDataRead),6);
    fprintf(stderr,"Read Multi! error status: %d\n",status);
    delay_ms(200);

    fprintf(stderr,"Byte = %X \t Word = %X \t DWord = %lX \t ",dataReadByte,dataReadWord,dataReadDWord);
    fprintf(stderr,"Multi = ");
    for (int i=0;i<6;i++){
        fprintf(stderr,"%X",bigDataRead[i]);
    }
    fprintf(stderr,"\n");

    fprintf(stderr,"Read a bunch of data, error status after everything: %d\n",status);
}

void test_interrupt_timer(){
    i2c_setup(I2C1);
    //tof setup
    t_dev = calloc(1,sizeof(*t_dev));
    t_dev[0] = calloc(1,sizeof(VL53L0X_DEV));
    VL53L0X_DEV dev = t_dev[0];
    uint8_t addr = 0x66;
    VL53L0X_Error status = 0;

    // debug led(trigger)
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_clear(GPIOA,GPIO5);

    fprintf(stderr,"Setup TOF\n");
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    __pulse(GPIOA, GPIO6, low, 20);

    status = _tof_1_setup(dev,addr);
    fprintf(stderr,"Setup dev DONE and Measure STARTED ! error status: %d\n",status);

    timer_setup_interrupt();
    while (1);
}

void test_tof_Single(){
    fprintf(stderr,"Welcome in test tof Single Ranging Mode\n");
    i2c_setup(I2C1);
    //tof setup
    t_dev = calloc(1,sizeof(*t_dev));
    t_dev[0] = calloc(1,sizeof(VL53L0X_DEV));
    VL53L0X_DEV dev = t_dev[0];
    uint8_t addr = 0x66;
    VL53L0X_Error status = 0;

    // debug led(trigger)
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_clear(GPIOA,GPIO5);

    fprintf(stderr,"Setup TOF\n");
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    __pulse(GPIOA, GPIO6, low, 20);

    status = _tof_1_setup(dev,addr);
    fprintf(stderr,"Setup dev DONE and Measure STARTED ! error status: %d\n",status);

    uint32_t start,stop;
    uint8_t time;

    while(!status){
        start = _clock_get_systicks();
        gpio_toggle(GPIOA,GPIO5);
        status = tof_perform_measure(dev);
        stop = _clock_get_systicks();
        fprintf(stderr,"Measure Performed ! error status: %d\n",status);
        fprintf(stderr,"Measure Performed ! range: %d\n",dev->range);
        gpio_clear(GPIOA,GPIO5);

        time = stop - start;
        fprintf(stderr,"Measure Performed ! measure time [ms]: %d\n",time);

        delay_ms(50);
    }
}

void test_tof_poke(){
    fprintf(stderr,"Welcome in test tof_poke\n");

    i2c_setup(I2C1);

    VL53L0X_DEV dev = calloc(1,sizeof(*dev));
    VL53L0X_Error status;
    fprintf(stderr,"Before init_dev\n");
    _tof_init_struct(dev);
    fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
    fprintf(stderr,"i2c peripheral: %s\n",(dev->i2c_dev==I2C1)?"OK":"NOP");
    fprintf(stderr,"After init_dev\n");

    fprintf(stderr,"Before tof_poke\n");
    status = _tof_poke(dev); //0x00c0


    fprintf(stderr,"error status: %d\n",status);
    fprintf(stderr,"After tof_poke\n");
    fprintf(stderr,"Goodbye from test tof_poke\n");
}

void test_xshut(){
    fprintf(stderr,"Welcome in test xshut\n");
    i2c_setup(I2C1);
    uint8_t addr = 0x88;
    VL53L0X_DEV dev = calloc(1,sizeof(*dev));
    VL53L0X_Error status = 0;
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_set(GPIOA, GPIO6);

    while(1){
        fprintf(stderr,"Start poke on base address\n");
        // _tof_init_struct(dev);
        // fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        // status = _tof_poke(dev); //0x00c0
        // //delay_ms(50);
        // fprintf(stderr,"status: %d\n",status);
        // fprintf(stderr,"After base address poke\n");

        // fprintf(stderr,"Start poke on new address\n");
        // status = _tof_set_address(dev, addr);
        _tof_setup_addr(dev,addr);
        //delay_ms(50);
        fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        fprintf(stderr,"status: %d\n",status);
        fprintf(stderr,"After new address poke\n");

        __pulse(GPIOA, GPIO6, low, 20);

        fprintf(stderr,"Start poke after reset\n");
        dev->i2c_slave_address = 0x52 / 2;
        status = _tof_poke(dev); //0x00c0
        fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        fprintf(stderr,"status: %d\n",status);
        fprintf(stderr,"After reset poke\n");

        delay_ms(1000);
    }
}