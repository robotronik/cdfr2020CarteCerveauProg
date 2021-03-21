#include "tof.h"

void tof_reset_pulse(enum rcc_periph_clken rcc_gpio,uint32_t gpio_port,uint16_t gpio_pin){

}

VL53L0X_Error tof_setup(VL53L0X_DEV dev){
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

    // FOR ONE TOF
    VL53L0X_Error status;
    VL53L0X_Calibration_Parameter calibration;
    uint8_t addr = 0x66;

    //reset tof
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_OTYPE_PP);
    gpio_set(GPIOA,GPIO6);
 
    // reset tof
    uint8_t direction_falling = 0;
    __pulse(GPIOA,GPIO6,direction_falling,20);

    status = _tof_setup_dev(dev,addr);
    if(status) return status;

    // 0 0 because no target information
    status = _tof_calibration(dev,&calibration,0,0);
    if(status) return status;

    status = _tof_configure_dev(dev, calibration);
    if(status) return status;

    status = VL53L0X_StartMeasurement(dev);
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

void _tof_init_dev(VL53L0X_DEV dev){
    dev->i2c_slave_address = 0x52 / 2;
	dev->i2c_dev = I2C1;
    dev->last_range = 0xfffe;
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
/*     fprintf(stderr, "Poke: id = 0x%x \n",id);
    delay_ms(20); */
    if(id != 0xAAEE){
/*         fprintf(stderr, "Error: Bad ID \n");
        delay_ms(20); */
        return VL53L0X_ERROR_NOT_IMPLEMENTED;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_set_address(VL53L0X_DEV dev, uint8_t addr){
  //Set device address
  VL53L0X_Error status = VL53L0X_SetDeviceAddress(dev, addr);
  if(status) return status;
  dev->i2c_slave_address = addr / 2;

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

    // El salvatore
    delay_ms(40);
    VL53L0X_DataInit(dev);
    if(status) return status;
    VL53L0X_StaticInit(dev);
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_calibration(VL53L0X_DEV dev, VL53L0X_Calibration_Parameter* calib_param, FixPoint1616_t offset_cal_distance, FixPoint1616_t xTalk_cal_distance){
    VL53L0X_Error status;

    /*Calibration*/
    status = VL53L0X_PerformRefCalibration(dev, &(calib_param->VhvSettings), &(calib_param->PhaseCal));
    if(status) return status;

    status = VL53L0X_PerformRefSpadManagement(dev, &(calib_param->refSpadCount), &(calib_param->isApertureSpads));
    if(status) return status;

    /*Calibration avec un objectif*/
    /*We don't have the robot to make the calibration with target*/
    // /*Set a White Target and define the distance to the sensor*/
    // status = VL53L0X_PerformOffsetCalibration(dev, offset_cal_distance, &(calib_param->OffsetMicroMeter));
    // if(status) return status;

    // /*Set a Grey Target and define */
    // status = VL53L0X_PerformXTalkCalibration(dev, xTalk_cal_distance, &(calib_param->XTalkCompensationRateMegaCps));
    // if(status) return status;

    return VL53L0X_ERROR_NONE;
}


VL53L0X_Error _tof_configure_dev(VL53L0X_DEV dev, VL53L0X_Calibration_Parameter calib_param){
    //Fig 5
    VL53L0X_Error status;

    /*Device mode*/
    //Set ranging mode
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    if(status) return status;

    // /* Ranging Profile*/
    // //Enable Sigma Limit
    // status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    // fprintf(stderr,"Set enable sigma status: %d\n",status);
    // if(status) return status;

    // //Enable Signal Limit
    // status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    // fprintf(stderr,"Set enable signal status: %d\n",status);
    // if(status) return status;

    // // Profile Long Range
    // //Set signal limit
    // status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, VL53L0X_LR_SIGNAL_LIMIT);
    // fprintf(stderr,"Set limit check signal status: %d\n",status);
    // if(status) return status;

    // //Set sigma limit
    // status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, VL53L0X_LR_SIGMA_LIMIT);
    // fprintf(stderr,"Set limit check sigma status: %d\n",status);
    // if(status) return status;

    // //Set timing budget
    // status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, VL53L0X_LR_TIMING_BUDGET);
    // fprintf(stderr,"Set timing budget status: %d\n",status);
    // if(status) return status;

    // //Set pre range pulse period
    // status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, VL53L0X_LR_VCSEL_PERIOD_PRE_RANGE);
    // fprintf(stderr,"Set pre range pulse period status: %d\n",status);
    // if(status) return status;

    // //Set final range pulse period
    // status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, VL53L0X_LR_VCSEL_PERIOD_FINAL_RANGE);
    // fprintf(stderr,"Set vc sel pulse period status: %d\n",status);
    // if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_get_measure(VL53L0X_DEV dev, uint16_t* range){
    VL53L0X_Error status;
    uint8_t ready = 0;
    uint32_t loop_nb = 0;
    VL53L0X_RangingMeasurementData_t measure_data;

    // POLLING until ready or time out 
    while(loop_nb < VL53L0X_DEFAULT_MAX_LOOP){
        status = VL53L0X_GetMeasurementDataReady(dev, &ready);
        if(status) return status;
        if(ready == 1){
            break;
        }
        loop_nb += 1;
        delay_ms(1);
    }

    if (loop_nb >= VL53L0X_DEFAULT_MAX_LOOP) {
        status = VL53L0X_ERROR_TIME_OUT;
        return status;
    }

    status = VL53L0X_GetRangingMeasurementData(dev,&measure_data);
    // fprintf(stderr,"getRangingMeasurementData error status : %d\n",status);
    if(status) return status;

    // tof_print_data_measure(dev,measure_data);
    // tof_print_int_status(dev);
    
    status = VL53L0X_ClearInterruptMask(dev,VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    // fprintf(stderr,"clear interrupt mask error status : %d\n",status);
    if(status) return status;

    if(measure_data.RangeStatus == 0){
        // Code maudit remerciez notre seigneur ST
        if(dev->last_range == 0xfffe){
            *range = measure_data.RangeMilliMeter;
            dev->last_range = *range;
            fprintf(stderr,"Voila ma premiere mesure: %d\n",*range);
        }
        else{
            *range = (TOF_COR_FACTOR * dev->last_range + (256-TOF_COR_FACTOR) * measure_data.RangeMilliMeter)/256;
            dev->last_range = *range;
        }
    }
    else{
        // status = tof_print_ranging_status(dev,measure_data);
        // if(status) return status;
    }

    return VL53L0X_ERROR_NONE;
}

void _shift_reg_init(){
    gpio_set(SR_DSAB_PORT, SR_DSAB_PIN);
    __pulse(SR_CP_PORT, SR_CP_PIN, 0, 20);
    gpio_clear(SR_DSAB_PORT, SR_DSAB_PIN);
}

void _shift_reg(int i){
    for(int j =0; j<i;++j){
        __pulse(SR_CP_PORT, SR_CP_PIN, 0, 20);
    }
}

void tof_reset(){
    _shift_reg_init();
    for(int i =0; i<TOF_NUM-1;++i){
        _shift_reg(1);
    }
}

VL53L0X_Error tof_print_device_info(VL53L0X_DEV dev){
    VL53L0X_Error status;
    VL53L0X_DeviceInfo_t DeviceInfo;
    status = VL53L0X_GetDeviceInfo(dev, &DeviceInfo);
    fprintf(stderr,"Get Information DONE ! error status: %d\n",status);
    if(status) return status;

    fprintf(stderr,"VL53L0X_GetDeviceInfo:\n");
    fprintf(stderr,"Device Name : %s\n", DeviceInfo.Name);
    fprintf(stderr,"Device Type : %s\n", DeviceInfo.Type);
    fprintf(stderr,"Device ID : %s\n", DeviceInfo.ProductId);
    fprintf(stderr,"ProductRevisionMajor : %d\n", DeviceInfo.ProductRevisionMajor);
    fprintf(stderr,"ProductRevisionMinor : %d\n", DeviceInfo.ProductRevisionMinor);

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_print_calib_info(VL53L0X_DEV dev){
    VL53L0X_Error status;
    uint32_t refSPADCount;
    uint8_t aperture;
    uint8_t vhvsetting;
    uint8_t phasecal;

    status = VL53L0X_GetRefCalibration(dev,&vhvsetting,&phasecal);
    fprintf(stderr,"Get Ref Calibration DONE ! error status: %d\n",status);
    if(status) return status;
    fprintf(stderr,"Get Ref Calibration DONE ! VHV setting: %d\n",vhvsetting);
    fprintf(stderr,"Get Ref Calibration DONE ! Phase calibration: %d\n",phasecal);

    status = VL53L0X_GetReferenceSpads(dev, &refSPADCount, &aperture);
    fprintf(stderr,"Get Spad Calibration DONE ! error status: %d\n",status);
    if(status) return status;
    fprintf(stderr,"Get Spad Calibration DONE ! number of SPAD: %ld\n",refSPADCount);
    fprintf(stderr,"Get Spad Calibration DONE ! is aperture: %d\n",aperture);

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_print_PAL_state(VL53L0X_DEV dev){
    VL53L0X_Error status;
    VL53L0X_State state;

    status = VL53L0X_GetPalState(dev,&state);
    fprintf(stderr,"Get state. error status : %d\n",status);
    if(status) return status;

    switch (state)
    {
    case 2:
        fprintf(stderr,"Get state. state : STANDBY\n");
        break;
    case 3:
        fprintf(stderr,"Get state. state : IDLE\n");
        break;
    case 4:
        fprintf(stderr,"Get state. state : RUNNING\n");
        break;
    case 98:
        fprintf(stderr,"Get state. state : UNKNOWN\n");
        break;
    case 99:
        fprintf(stderr,"Get state. state : ERROR\n");
        break;
    
    default:
        fprintf(stderr,"Get state. state : NOT EXPECTED\n");
        break;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_print_device_mode(VL53L0X_DEV dev){
    VL53L0X_Error status;
    VL53L0X_DeviceModes deviceMode;

    status = VL53L0X_GetDeviceMode(dev, &deviceMode);
    fprintf(stderr,"Get device mode. error status : %d\n",status);
    if(status) return status;

    switch (deviceMode) {
    case 0:
        fprintf(stderr,"Get device mode. mode : SINGLE_RANGING\n");
        break;
    case 1:
        fprintf(stderr,"Get device mode. mode : CONTINUOUS_RANGING\n");
        break;
    case 3:
        fprintf(stderr,"Get device mode. mode : CONTINUOUS_TIMED_RANGING\n");
        break;
    
    default:
        fprintf(stderr,"Get device mode. mode : NOT EXPECTED\n");
        break;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_print_ranging_status(VL53L0X_RangingMeasurementData_t measure_data){
    VL53L0X_Error status;
    uint8_t range_status;

    range_status = measure_data.RangeStatus;

    switch (range_status) {
    case 0:
        fprintf(stderr,"Get range status. status : VALID\n");
        status = VL53L0X_ERROR_NONE;
        break;
    case 1:
        fprintf(stderr,"Get range status. status : SIGMA FAIL\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    case 2:
        fprintf(stderr,"Get range status. status : SIGNAL FAIL\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    case 3:
        fprintf(stderr,"Get range status. status : MIN RANGE FAIL\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    case 4:
        fprintf(stderr,"Get range status. status : PHASE FAIL\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    case 5:
        fprintf(stderr,"Get range status. status : HARDWARE FAIL\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    case 255:
        fprintf(stderr,"Get range status. status : NO UPDATE\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    
    default:
        fprintf(stderr,"Get range status. status : NOT EXPECTED\n");
        status = VL53L0X_ERROR_RANGE_ERROR;
        break;
    }

    return status;
}

VL53L0X_Error tof_print_data_measure(VL53L0X_RangingMeasurementData_t measure_data){    
    //print all parameter of measure data
    fprintf(stderr,"measure data time stamp: %ld\n",measure_data.TimeStamp);
    fprintf(stderr,"measure data measurement time Usec: %ld\n",measure_data.MeasurementTimeUsec);
    fprintf(stderr,"measure data range in milli: %d\n",measure_data.RangeMilliMeter);
    fprintf(stderr,"measure data range dmax in milli: %d\n",measure_data.RangeDMaxMilliMeter);
    fprintf(stderr,"measure data signal rate: %ld\n",measure_data.SignalRateRtnMegaCps);
    fprintf(stderr,"measure data ambient rate: %ld\n",measure_data.AmbientRateRtnMegaCps);
    fprintf(stderr,"measure data effective spad count: %d\n",measure_data.EffectiveSpadRtnCount);
    fprintf(stderr,"measure data zone ID: %d\n",measure_data.ZoneId);
    fprintf(stderr,"measure data fractionnal part: %d\n",measure_data.RangeFractionalPart);
    fprintf(stderr,"measure data status: %d\n",measure_data.RangeStatus);

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_print_int_status(VL53L0X_DEV dev){
    VL53L0X_Error status;
    uint32_t intStatus;

    status = VL53L0X_GetInterruptMaskStatus(dev,&intStatus);
    fprintf(stderr,"Get interupt status. error status : %d\n",status);
    if(status) return status;

    switch (intStatus)
    {
    case 0:
        fprintf(stderr,"Get Interrupt Status. status : DIASBLED\n");
        break;
    case 3:
        fprintf(stderr,"Get Interrupt Status. status : OUT OF WINDOW\n");
        break;
    case 4:
        fprintf(stderr,"Get Interrupt Status. status : NEW SAMPLE READY\n");
        break;
    
    default:
        fprintf(stderr,"Get Interrupt Status. status : NOT EXPECTED\n");
        break;
    }

    return VL53L0X_ERROR_NONE;
}
