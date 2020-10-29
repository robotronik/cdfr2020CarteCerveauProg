#include "i2c.h"

void i2c_setup(uint32_t i2c_peripheral,){
    switch(i2c_peripheral){
        case I2C1:

            /* Enable timer clock */
            rcc_periph_clock_enable(RCC_I2C1);

            /* Disable peripheral before changing parameters */
            i2c_peripheral_disable(I2C1);
            //i2c_enable_analog_filter(I2C1); /*used on f3 (pas besoin je crois)*/ 
            i2c_set_speed(I2C1, i2c_speed_fm_400k, (rcc_apb1_frequency / 1000000) );

            //i2c_set_data_hold_time(I2C1, 1); /*used on f2 (called by set_speed)*/
            //i2c_set_7bit_addr_mode(I2C1); /*used on f3 (pas besoin je crois)*/
            //i2c_enable_stretching(I2C1); /*used on f3 (je sais pas si je suis master)*/
            i2c_peripheral_enable(I2C1);
            
            break
        case I2C2:

            /* Enable timer clock */
            rcc_periph_clock_enable(RCC_I2C2);

            /* Disable peripheral before changing parameters */
            i2c_peripheral_disable(I2C2);
            i2c_set_speed(I2C2, i2c_speed_fm_400k, (rcc_apb1_frequency / 1000000) );

            i2c_peripheral_enable(I2C2);

            break
}
