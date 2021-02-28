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
    //test_i2c();
    //test_tof_platform_read();
    //interrupt_timer_test();
/*     while(1){
        test_tof_poke();
        delay_ms(500);
    } */
    test_tof();
    // test_xshut();

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

// Obsolete implementation en cours
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
    
    //reset tof
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_set(GPIOA,GPIO6);
 
    // debug led(trigger)
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO5,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_clear(GPIOA,GPIO5);

    //while(0){
        fprintf(stderr,"New Cycle\n");
        uint8_t direction_falling = 0;
        __pulse(GPIOA,GPIO6,direction_falling,20);

        status = _tof_setup_dev(dev,addr);
        fprintf(stderr,"Setup dev DONE ! error status: %d\n",status);

        // get information
        __pulse(GPIOA,GPIOA,1,100);
        VL53L0X_DeviceInfo_t DeviceInfo;
        status = VL53L0X_GetDeviceInfo(dev, &DeviceInfo);
        fprintf(stderr,"Get Information DONE ! error status: %d\n",status);

        fprintf(stderr,"VL53L0X_GetDeviceInfo:\n");
        fprintf(stderr,"Device Name : %s\n", DeviceInfo.Name);
        fprintf(stderr,"Device Type : %s\n", DeviceInfo.Type);
        fprintf(stderr,"Device ID : %s\n", DeviceInfo.ProductId);
        fprintf(stderr,"ProductRevisionMajor : %d\n", DeviceInfo.ProductRevisionMajor);
        fprintf(stderr,"ProductRevisionMinor : %d\n", DeviceInfo.ProductRevisionMinor);

        uint32_t refSPADCount;
        uint8_t aperture;
        uint8_t vhvsetting;
        uint8_t phasecal;

        status = _tof_calibration(dev,&myCalib,0,0);
        fprintf(stderr,"Calibration DONE ! error status: %d\n",status);
        fprintf(stderr,"Calibration DONE ! number of SPAD: %d\n", myCalib.refSpadCount);
        fprintf(stderr,"Calibration DONE ! is aperture: %d\n",myCalib.isApertureSpads);
        fprintf(stderr,"Calibration DONE ! VHV setting: %d\n",myCalib.VhvSettings);
        fprintf(stderr,"Calibration DONE ! Phase calibration: %d\n",myCalib.PhaseCal);


        status = VL53L0X_GetReferenceSpads(dev, &refSPADCount, &aperture);
        fprintf(stderr,"Get Spad DONE ! error status: %d\n",status);
        fprintf(stderr,"Get Spad DONE ! number of SPAD: %d\n",refSPADCount);
        fprintf(stderr,"Get Spad DONE ! is aperture: %d\n",aperture);

        status = VL53L0X_GetRefCalibration(dev,&vhvsetting,&phasecal);
        fprintf(stderr,"Get Calib DONE ! error status: %d\n",status);
        fprintf(stderr,"Get Calib DONE ! VHV setting: %d\n",vhvsetting);
        fprintf(stderr,"Get Calib DONE ! Phase calibration: %d\n",phasecal);
        
        
        // status = _tof_configure_dev(dev, myCalib);
        // status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);
        status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
        fprintf(stderr,"Configuration DONE ! error status: %d\n",status);
        
        status = VL53L0X_StartMeasurement(dev);
        fprintf(stderr,"Start Measure DONE ! error status: %d\n",status);

        delay_ms(1000);

        uint16_t range = 0;
        VL53L0X_RangingMeasurementData_t measure_data;
        uint8_t ready = 0;

/*         while(1){
            status = VL53L0X_PerformSingleRangingMeasurement (dev, &measure_data);
            // fprintf(stderr,"One Measure DONE ! error status: %d\n",status);

            //print all parameter of measure data
            // fprintf(stderr,"measure data time stamp: %d\n",measure_data.TimeStamp);
            // fprintf(stderr,"measure data measurement time Usec: %d\n",measure_data.MeasurementTimeUsec);
            fprintf(stderr,"measure data range in milli: %d\n",measure_data.RangeMilliMeter);
            // fprintf(stderr,"measure data range dmax in milli: %d\n",measure_data.RangeDMaxMilliMeter);
            // fprintf(stderr,"measure data signal rate: %d\n",measure_data.SignalRateRtnMegaCps);
            // fprintf(stderr,"measure data ambient rate: %d\n",measure_data.AmbientRateRtnMegaCps);
            // fprintf(stderr,"measure data effective spad count: %d\n",measure_data.EffectiveSpadRtnCount);
            // fprintf(stderr,"measure data zone ID: %d\n",measure_data.ZoneId);
            // fprintf(stderr,"measure data fractionnal part: %d\n",measure_data.RangeFractionalPart);
            // fprintf(stderr,"measure data status: %d\n",measure_data.RangeStatus);

            delay_ms(100);
        } */

        VL53L0X_State state;

        VL53L0X_DeviceModes deviceMode;

        uint32_t interStatus;

        while(1){
            status = VL53L0X_GetPalState(dev,&state);
            fprintf(stderr,"Get state. error status : %d\n",status);
            fprintf(stderr,"Get state. state : %d\n",state);

            /* Get Current DeviceMode */
	        status = VL53L0X_GetDeviceMode(dev, &deviceMode);
            fprintf(stderr,"Get device mode. error status : %d\n",status);
            fprintf(stderr,"Get device mode. device mode : %d\n",deviceMode);

            //appel bloquant a voir si on peut faire sans ? cas d'erreur ou l'appel ne finirait jamais notamment ?
            while(!(ready && status == 0)){
                status = VL53L0X_GetMeasurementDataReady(dev, &ready);
                // fprintf(stderr,"Get measure ready. error status : %d\n",status);
                // fprintf(stderr,"Get measure ready. ready : %d\n",ready);
                if(status != 0){
                    fprintf(stderr,"Wait ready DONE ! error status: %d\n",status);
                    fprintf(stderr,"OH FUCKING SECOUR\n");
                    while(1);
                }
                delay_ms(1);
            }
            fprintf(stderr,"Wait ready DONE ! error status: %d\n",status);

            status = VL53L0X_GetRangingMeasurementData(dev,&measure_data);
            fprintf(stderr,"Get Data DONE ! error status: %d\n",status);

            //print all parameter of measure data
            fprintf(stderr,"measure data time stamp: %d\n",measure_data.TimeStamp);
            fprintf(stderr,"measure data measurement time Usec: %d\n",measure_data.MeasurementTimeUsec);
            fprintf(stderr,"measure data range in milli: %d\n",measure_data.RangeMilliMeter);
            fprintf(stderr,"measure data range dmax in milli: %d\n",measure_data.RangeDMaxMilliMeter);
            fprintf(stderr,"measure data signal rate: %d\n",measure_data.SignalRateRtnMegaCps);
            fprintf(stderr,"measure data ambient rate: %d\n",measure_data.AmbientRateRtnMegaCps);
            fprintf(stderr,"measure data effective spad count: %d\n",measure_data.EffectiveSpadRtnCount);
            fprintf(stderr,"measure data zone ID: %d\n",measure_data.ZoneId);
            fprintf(stderr,"measure data fractionnal part: %d\n",measure_data.RangeFractionalPart);
            fprintf(stderr,"measure data status: %d\n",measure_data.RangeStatus);

            ready = 0;
            status = VL53L0X_GetInterruptMaskStatus(dev,&interStatus);
            fprintf(stderr,"Get interupt status. error status : %d\n",status);
            fprintf(stderr,"Get interupt status. interupt status : %d\n",interStatus);
            status = VL53L0X_ClearInterruptMask(dev, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
            fprintf(stderr,"Get clear interupt status. error status : %d\n",status);

            delay_ms(1);
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