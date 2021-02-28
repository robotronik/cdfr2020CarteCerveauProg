/*******************************************************************************
Copyright � 2015, STMicroelectronics International N.V.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STMicroelectronics nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS ARE DISCLAIMED.
IN NO EVENT SHALL STMICROELECTRONICS INTERNATIONAL N.V. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************/

/**
 * @file VL53L0X_i2c.c
 *
 * Copyright (C) 2014 ST MicroElectronics
 *
 * provide variable word size byte/Word/dword VL6180x register access via i2c
 *
 */
#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"


#include "i2c.h"
#include "timer.h"


#define LOG_FUNCTION_START(fmt, ... )           _LOG_FUNCTION_START(TRACE_MODULE_PLATFORM, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ... )          _LOG_FUNCTION_END(TRACE_MODULE_PLATFORM, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ... ) _LOG_FUNCTION_END_FMT(TRACE_MODULE_PLATFORM, status, fmt, ##__VA_ARGS__)

/**
 * @def I2C_BUFFER_CONFIG
 *
 * @brief Configure Device register I2C access
 *
 * @li 0 : one GLOBAL buffer \n
 *   Use one global buffer of MAX_I2C_XFER_SIZE byte in data space \n
 *   This solution is not multi-Device compliant nor multi-thread cpu safe \n
 *   It can be the best option for small 8/16 bit MCU without stack and limited ram  (STM8s, 80C51 ...)
 *
 * @li 1 : ON_STACK/local \n
 *   Use local variable (on stack) buffer \n
 *   This solution is multi-thread with use of i2c resource lock or mutex see VL6180x_GetI2CAccess() \n
 *
 * @li 2 : User defined \n
 *    Per Device potentially dynamic allocated. Requires VL6180x_GetI2cBuffer() to be implemented.
 * @ingroup Configuration
 */
#define I2C_BUFFER_CONFIG 1
/** Maximum buffer size to be used in i2c */
#define VL53L0X_MAX_I2C_XFER_SIZE   64 /* Maximum buffer size to be used in i2c */

#if I2C_BUFFER_CONFIG == 0
    /* GLOBAL config buffer */
    uint8_t i2c_global_buffer[VL53L0X_MAX_I2C_XFER_SIZE];

    #define DECL_I2C_BUFFER
    #define VL53L0X_GetLocalBuffer(Dev, n_byte)  i2c_global_buffer

#elif I2C_BUFFER_CONFIG == 1
    /* ON STACK */
    #define DECL_I2C_BUFFER  uint8_t LocBuffer[VL53L0X_MAX_I2C_XFER_SIZE];
    #define VL53L0X_GetLocalBuffer(Dev, n_byte)  LocBuffer
#elif I2C_BUFFER_CONFIG == 2
    /* user define buffer type declare DECL_I2C_BUFFER  as access  via VL53L0X_GetLocalBuffer */
    #define DECL_I2C_BUFFER
#else
#error "invalid I2C_BUFFER_CONFIG "
#endif


#define VL53L0X_I2C_USER_VAR         /* none but could be for a flag var to get/pass to mutex interruptible  return flags and try again */
#define VL53L0X_GetI2CAccess(Dev)    /* todo mutex acquire */
#define VL53L0X_DoneI2CAcces(Dev)    /* todo mutex release */

//not implemented
VL53L0X_Error VL53L0X_LockSequenceAccess(VL53L0X_DEV Dev){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    return Status;
}
//not implemented
VL53L0X_Error VL53L0X_UnlockSequenceAccess(VL53L0X_DEV Dev){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count){

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;
	uint8_t buffer[count+1];

    if (count+1>=VL53L0X_MAX_I2C_XFER_SIZE){
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }
	//status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);
	buffer[0] = index;
    memcpy(&buffer[1], pdata, count);
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,buffer,count+1);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;

    if (count+1>=VL53L0X_MAX_I2C_XFER_SIZE){
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,&index,1);
    // pdata[0] = index; unused in our read7 implementation cf "VL53L0X_read_multi"
    i2c_status = i2c_read7(Dev->i2c_dev,Dev->i2c_slave_address,pdata,count);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}


VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;
	uint8_t buffer[2];

	//status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);
	buffer[0] = index;
    memcpy(&buffer[1], &data, 1);
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,buffer,2);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;
	uint8_t buffer[3];

	//status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);
	buffer[0] = index;
    memcpy((uint16_t*)(&buffer[1]), &data, 2);
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,buffer,3);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;
	uint8_t buffer[5];

	//status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);
	buffer[0] = index;
    memcpy((uint32_t*)(&buffer[1]), &data, 4);
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,buffer,5);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV Dev, uint8_t index, uint8_t AndData, uint8_t OrData){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t data;
    I2C_status i2c_status = I2C_OK;

    //status_int = VL53L0X_read_byte(deviceAddress, index, &data);
    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,&index,1);
    // pdata[0] = index; unused in our read7 implementation cf "VL53L0X_read_multi"
    i2c_status = i2c_read7(Dev->i2c_dev,Dev->i2c_slave_address,&data,1);

    if (i2c_status != I2C_OK)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    if (Status == VL53L0X_ERROR_NONE) {
        data = (data & AndData) | OrData;
        // status_int = VL53L0X_write_byte(deviceAddress, index, data);
        uint8_t buffer[2];
        //status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);
        buffer[1] = index;
        memcpy(&buffer[0], &data, 1);
        i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,buffer,2);

        if (i2c_status != I2C_OK){
            Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        }
    }

    return Status;
}

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;

    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,&index,1);
    // pdata[0] = index; unused in our read7 implementation cf "VL53L0X_read_multi"
    i2c_status = i2c_read7(Dev->i2c_dev,Dev->i2c_slave_address,data,1);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;

    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,&index,1);
    // pdata[0] = index; unused in our read7 implementation cf "VL53L0X_read_multi"
    i2c_status = i2c_read7(Dev->i2c_dev,Dev->i2c_slave_address,(uint8_t*)data,2);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

VL53L0X_Error  VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    I2C_status i2c_status = I2C_OK;

    i2c_status = i2c_write7(Dev->i2c_dev,Dev->i2c_slave_address,&index,1);
    // pdata[0] = index; unused in our read7 implementation cf "VL53L0X_read_multi"
    i2c_status = i2c_read7(Dev->i2c_dev,Dev->i2c_slave_address,(uint8_t*)data,4);

	if (i2c_status != I2C_OK ){
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}

#define VL53L0X_POLLINGDELAY_LOOPNB  250
VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    LOG_FUNCTION_START("");

    delay_ms(VL53L0X_DELAY_US/1000);

    LOG_FUNCTION_END(status);
    return status;
}
