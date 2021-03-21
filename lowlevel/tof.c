#include "tof.h"

VL53L0X_Error tof_setup(VL53L0X_DEV* t_dev,uint8_t tof_number){
    VL53L0X_Error status;
    
    i2c_setup(I2C1);

    //setup pin for SHIFTR
    _gpio_setup_pin(SHIFTR_DSAB_RCC, SHIFTR_DSAB_PORT, SHIFTR_DSAB_PIN, GPIO_MODE_OUTPUT,
     GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PP);
    _gpio_setup_pin(SHIFTR_CP_RCC, SHIFTR_CP_PORT, SHIFTR_CP_PIN, GPIO_MODE_OUTPUT,
     GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PP);

    /*Reset Tof */

    for(int i = 0; i< tof_number;++i){
        //faire un truc
        __pulse(GPIOA,GPIO6,low,20);
    }
    //placeholder for one tof
    _gpio_setup_pin(RCC_GPIOA,GPIOA,GPIO6,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,
    GPIO_OTYPE_PP);
    gpio_set(GPIOA,GPIO6);
    // reset tof
    __pulse(GPIOA,GPIO6,low,20);


    status = _tof_1_setup(t_dev[0],0x66);
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_1_setup(VL53L0X_DEV dev, uint8_t tof_addr){
    VL53L0X_Error status = 0;
    VL53L0X_Calibration_Parameter calibration;


    status = _tof_setup_addr(dev,tof_addr);
    // fprintf(stderr,"Setup addr ! error status: %d\n",status);
    if(status) return status;

    status = _tof_setup_calib(dev,&calibration);
    // fprintf(stderr,"Setup calib ! error status: %d\n",status);
    if(status) return status;

    // not needed using single measure and default range
    /*
    status = _tof_config(dev, calibration);
    if(status) return status;
    */

    status = VL53L0X_StartMeasurement(dev);
    // fprintf(stderr,"Start measure ! error status: %d\n",status);
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

void _tof_init_struct(VL53L0X_DEV dev){
    dev->i2c_slave_address = 0x52 / 2;
	dev->i2c_dev = I2C1;
    dev->range = 0xfffe;
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

VL53L0X_Error _tof_setup_addr(VL53L0X_DEV dev, uint8_t addr){
    VL53L0X_Error status;
    _tof_init_struct(dev);

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

VL53L0X_Error _tof_calibration(VL53L0X_DEV dev, 
VL53L0X_Calibration_Parameter* calib_param, FixPoint1616_t offset_cal_distance,
FixPoint1616_t xTalk_cal_distance){
    VL53L0X_Error status;

    /*Calibration without target*/
    status = VL53L0X_PerformRefCalibration(dev, &(calib_param->VhvSettings),
     &(calib_param->PhaseCal));
    if(status) return status;

    status = VL53L0X_PerformRefSpadManagement(dev, &(calib_param->refSpadCount),
     &(calib_param->isApertureSpads));
    if(status) return status;

    /*Calibration with target*/
    /*We don't have the robot to make the calibration with target*/
    /*Set a White Target and define the distance to the sensor*/
    status = VL53L0X_PerformOffsetCalibration(dev, offset_cal_distance,
    &(calib_param->OffsetMicroMeter));
    if(status) return status;

    /*Set a Grey Target and define */
    status = VL53L0X_PerformXTalkCalibration(dev, xTalk_cal_distance,
    &(calib_param->XTalkCompensationRateMegaCps));
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_setup_calib(VL53L0X_DEV dev, 
VL53L0X_Calibration_Parameter* calib_param){
    VL53L0X_Error status;

    /*Calibration without target*/
    status = VL53L0X_PerformRefCalibration(dev, &(calib_param->VhvSettings), 
    &(calib_param->PhaseCal));
    if(status) return status;

    status = VL53L0X_PerformRefSpadManagement(dev, &(calib_param->refSpadCount), 
    &(calib_param->isApertureSpads));
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error _tof_config(VL53L0X_DEV dev){
    //Fig 5
    VL53L0X_Error status;

    /*Device mode*/
    //Set ranging mode
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    if(status) return status;

    // /* LONG Ranging Profile*/
    //Enable Sigma Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    fprintf(stderr,"Set enable sigma status: %d\n",status);
    if(status) return status;

    //Enable Signal Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    fprintf(stderr,"Set enable signal status: %d\n",status);
    if(status) return status;

    // Profile Long Range
    //Set signal limit
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, VL53L0X_LR_SIGNAL_LIMIT);
    fprintf(stderr,"Set limit check signal status: %d\n",status);
    if(status) return status;

    //Set sigma limit
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, VL53L0X_LR_SIGMA_LIMIT);
    fprintf(stderr,"Set limit check sigma status: %d\n",status);
    if(status) return status;

    //Set timing budget
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, VL53L0X_LR_TIMING_BUDGET);
    fprintf(stderr,"Set timing budget status: %d\n",status);
    if(status) return status;

    //Set pre range pulse period
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, VL53L0X_LR_VCSEL_PERIOD_PRE_RANGE);
    fprintf(stderr,"Set pre range pulse period status: %d\n",status);
    if(status) return status;

    //Set final range pulse period
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, VL53L0X_LR_VCSEL_PERIOD_FINAL_RANGE);
    fprintf(stderr,"Set vc sel pulse period status: %d\n",status);
    if(status) return status;

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error tof_perform_measure(VL53L0X_DEV dev){
    VL53L0X_Error status;
    VL53L0X_RangingMeasurementData_t measure_data;

    // Blocking call for 1-2 ms if the called are spaced by 40ms
    status = VL53L0X_PerformSingleRangingMeasurement (dev, &measure_data);
    //monitoring function 
    // tof_print_ranging_status(dev,measure_data);
    if(status) return status;

    dev->range = measure_data.RangeMilliMeter;
    
    //monitoring function with verbose ranging data
    // tof_print_data_measure(dev,measure_data);

    return VL53L0X_ERROR_NONE;
}

// void _shift_reg_init(){
//     gpio_set(SHIFTR_DSAB_PORT, SHIFTR_DSAB_PIN);
//     __pulse(SHIFTR_CP_PORT, SHIFTR_CP_PIN, low, 20);
//     gpio_clear(SHIFTR_DSAB_PORT, SHIFTR_DSAB_PIN);
// }

// void _shift_reg(int i){
//     for(int j =0; j<i;++j){
//         __pulse(SHIFTR_CP_PORT, SHIFTR_CP_PIN, low, 20);
//     }
// }

// void tof_reset(){
//     _shift_reg_init();
//     for(int i =0; i<TOF_NUM-1;++i){
//         _shift_reg(1);
//     }
// }

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
