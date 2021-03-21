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
#include "canmsgs.h"

void test_actuator();
void test_com();
void test_tof();
void test_tof_poke();
void test_i2c();
void test_xshut();
void blink_led();
void test_tof_platform_write();
void test_tof_platform_read();
void interrupt_timer_test();
void test_can_transmit();

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
    //test_tof_platform_write();
    // test_i2c();
    //test_tof_platform_read();
    //interrupt_timer_test();
/*     while(1){
        test_tof_poke();
        delay_ms(500);
    } */
    /*test_tof();*/
    //test_xshut();

    test_can_transmit();
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
    myError = VL53L0X_ReadMulti(pDev,index,(uint8_t*)(&bigDataRead),6);
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

void interrupt_timer_test(){
    int counter;
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_OTYPE_PP);
    timer_setup_interrupt();
    while (1){

    }

}

void test_tof(){
    fprintf(stderr,"Welcome in test tof\n");
    //tof setup
    i2c_setup(I2C1);
    VL53L0X_DEV dev = calloc(1,sizeof(*dev));
    VL53L0X_Error status;
    VL53L0X_Calibration_Parameter myCalib;
    uint8_t addr = 0x66;

    //gestion du reset des tofs
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_set(GPIOA,GPIO6);
    gpio_clear(GPIOA,GPIO5);

    while(1){
         fprintf(stderr,"New Cycle\n");
        gpio_clear(GPIOA,GPIO5);
        gpio_clear(GPIOA,GPIO6);
        delay_ms(20);
        gpio_set(GPIOA,GPIO6);

        status = _tof_setup_dev(dev,0x66); //0x33 en pratique
        status = VL53L0X_GetReferenceSpads(dev,&(myCalib.refSpadCount),&(myCalib.isApertureSpads));
        fprintf(stderr,"get SPAD count: %d\n",myCalib.refSpadCount);
        fprintf(stderr,"get is aperture SPAD: %d\n",myCalib.isApertureSpads);
        fprintf(stderr,"Get Ref SPAD DONE ! error status: %d\n",status);
        
        //J4ai fait une erreur a corriger 
        status = _tof_calibration(dev,&myCalib,0,0);
        // ICI
        
        fprintf(stderr,"Perform SPAD count: %d\n",myCalib.refSpadCount);
        fprintf(stderr,"Perform is aperture SPAD: %d\n",myCalib.isApertureSpads);
        fprintf(stderr,"Perform Ref SPAD DONE ! error status: %d\n",status);
        status = _tof_configure_dev(dev, myCalib);
        fprintf(stderr,"Configuration DONE ! error status: %d\n",status);

        delay_ms(1000);
    }

    fprintf(stderr,"Goodbye from test tof\n");
}

void test_tof_poke(){
    fprintf(stderr,"Welcome in test tof_poke\n");

    i2c_setup(I2C1);

    VL53L0X_DEV dev = calloc(1,sizeof(*dev));
    VL53L0X_Error status;
    fprintf(stderr,"Before init_dev\n");
    _tof_init_dev(dev);
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
    VL53L0X_Error status;
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_set(GPIOA, GPIO6);

    while(1){
        fprintf(stderr,"Start poke on base address\n");
        _tof_init_dev(dev);
        fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        status = _tof_poke(dev); //0x00c0
        //delay_ms(50);
        fprintf(stderr,"status: %d\n",status);
        fprintf(stderr,"After base address poke\n");

        fprintf(stderr,"Start poke on new address\n");
        status = _tof_set_address(dev, addr);
        //delay_ms(50);
        fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        fprintf(stderr,"status: %d\n",status);
        fprintf(stderr,"After new address poke\n");

        gpio_clear(GPIOA,GPIO6);
        delay_ms(20);
        gpio_set(GPIOA,GPIO6);


        fprintf(stderr,"Start poke after reset\n");
        dev->i2c_slave_address = 0x52 / 2;
        status = _tof_poke(dev); //0x00c0
        fprintf(stderr,"slave address: %X\n",dev->i2c_slave_address);
        fprintf(stderr,"status: %d\n",status);
        fprintf(stderr,"After reset poke\n");

        delay_ms(1000);
    }
}

void test_can_transmit(){
    uint8_t* data = calloc(2,sizeof(*data));
    data = 0xbeef;

    can_setup();
    int status = 0;

    while(!status){
        status = can_transmit(CAN1, 0x4, false, false,sizeof(data), data);
        fprintf(stderr,"transmission status: %d\n",status);
        delay_ms(100);
    }
}