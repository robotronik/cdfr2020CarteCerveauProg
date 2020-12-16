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
    int status;
    status = _tof_setup_dev(myTof,0x52);
    //TODO return error not yet implemented void function 
    //if(status) return -1;
    
}

void _tof_init_dev(VL53L0X_DEV dev){
    dev = calloc(1,sizeof(dev));

    dev->i2c_slave_address = 0x52;
	dev->i2c_dev = I2C1;
}

int _tof_poke(VL53L0X_DEV dev){
  int status;
  //Read ID to know if the address is correct
  uint16_t id = 0;
  status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &id);
  
  //I2C error or bad ID
  //Compare to 0xEEAA because of black magic (standard value)
  if(status || id != 0xEEAA){
    return -1;
  }
  
  return 0;
}

int _tof_set_address(VL53L0X_DEV dev, uint8_t addr){
  //Set device address
  int status = VL53L0X_SetDeviceAddress(dev, addr);
  if(status) return -1;
  dev->i2c_slave_address = addr;

  //Check if the device work with the new address
  return _tof_poke(dev);
}

int _tof_setup_dev(VL53L0X_DEV dev, uint8_t addr){
    int status;
    _tof_init_dev(dev);

    status = _tof_poke(dev);
    if(status) return -1;

    status = _tof_set_address(dev, addr);
    if(status) return -1;

    DataInit(dev);
    StaticInit(dev);

    return 0;
}

int _tof_configure_dev(){
    //Fig 5

    /*TODO Take Parameter and configure the device*/
    return 0;
}

int _tof_calibration(VL53L0X_DEV dev){
    int status;
    status = _tof_setup_dev(dev,0x52);
    if(status) return -1;

    /*TODO Calibration*/
    return 0;
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
