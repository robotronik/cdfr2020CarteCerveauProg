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

    //POUR 1 TOF
    VL53L0X_Error status;
    status = _tof_setup_dev(myTof,0x52);
    //TODO return error not yet implemented void function 
    //if(status) return -1;
    status = _tof_configure_dev(myTof);
    status = VL53L0X_StartMeasurement(myTof);
}

void _tof_init_dev(VL53L0X_DEV dev){
    dev = calloc(1,sizeof(dev));

    dev->i2c_slave_address = 0x52;
	dev->i2c_dev = I2C1;
}

VL53L0X_Error _tof_poke(VL53L0X_DEV dev){
    VL53L0X_Error status;
    //Read ID to know if the address is correct
    uint16_t id = 0;
    status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &id);

    //I2C error or bad ID
    //Compare to 0xEEAA because of black magic (standard value)
    if(status){
        return status;
    }
    if(id != 0xEEAA){
    return VL53L0X_ERROR_NOT_IMPLEMENTED;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_set_address(VL53L0X_DEV dev, uint8_t addr){
  //Set device address
  VL53L0X_Error status = VL53L0X_SetDeviceAddress(dev, addr);
  if(status) return status;
  dev->i2c_slave_address = addr;

  //Check if the device work with the new address
  return _tof_poke(dev);
}

VL53L0X_Error _tof_setup_dev(VL53L0X_DEV dev, uint8_t addr){
    VL53L0X_Error status;
    _tof_init_dev(dev);

    status = _tof_poke(dev);
    if(status) return status;

    status = _tof_set_address(dev, addr);
    if(status) return status;

    DataInit(dev);
    StaticInit(dev);

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_configure_dev(VL53L0X_DEV dev){
    //Fig 5
    /*TODO skip calibration*/
    
    VL53L0X_Error status;
    //Set single ranging mode
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    if(status) return status;
  
    //Enable Sigma Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if(status) return status;
  
    //Enable Signal Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if(status) return status;

    /*Profile Long Range*/
    //Set signal limit
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, VL53L0X_LR_SIGNAL_LIMIT);
    if(status) return status;
  
    //Set sigma limit
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, VL53L0X_LR_SIGMA_LIMIT);
    if(status) return status;

    //Set timing budget
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, VL53L0X_LR_TIMING_BUDGET);
    if(status) return status;
  
    //Set pre range pulse period
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, VL53L0X_LR_VCSEL_PERIOD_PRE_RANGE);
    if(status) return status;

    //Set final range pulse period
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, VL53L0X_LR_VCSEL_PERIOD_FINAL_RANGE);
    if(status) return status;
  
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_calibration(VL53L0X_DEV dev){
    VL53L0X_Error status;
    status = _tof_setup_dev(dev,0x52);
    if(status) return status;

    /*TODO Calibration*/
    return 0;
}

VL53L0X_Error tof_get_measure(VL53L0X_DEV dev, uint16_t* range){
    VL53L0X_Error status;
    uint8_t ready;
    status = VL53L0X_GetMeasurementDataReady(dev, &ready);
    if(status) return status;

    if(!ready) return VL53L0X_ERROR_NOT_IMPLEMENTED;

    VL53L0X_RangingMeasurementData_t measure_data;
    status = VL53L0X_GetRangingMeasurementData(dev,&measure_data);
    if(status) return status;

    status = VL53L0X_ClearInterruptMask(dev,-1);
    if(status) return status;

    *range = measure_data.RangeMilliMeter;

    return VL53L0X_ERROR_NONE;
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
