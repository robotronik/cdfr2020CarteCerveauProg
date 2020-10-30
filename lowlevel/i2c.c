#include "i2c.h"

void i2c_setup(uint32_t i2c_peripheral){
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
            
            break;
        case I2C2:

            /* Enable timer clock */
            rcc_periph_clock_enable(RCC_I2C2);

            /* Disable peripheral before changing parameters */
            i2c_peripheral_disable(I2C2);
            i2c_set_speed(I2C2, i2c_speed_fm_400k, (rcc_apb1_frequency / 1000000) );

            i2c_peripheral_enable(I2C2);

            break;
    }
}


//copie depuis libopencm3
void i2c_write7(uint32_t i2c, int addr, uint8_t *data, size_t n)
{
     while ((I2C_SR2(i2c) & I2C_SR2_BUSY)) {
     }

     i2c_send_start(i2c);

     /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
     while ( !( (I2C_SR1(i2c) & I2C_SR1_SB)
             && (I2C_SR2(i2c) & I2C_SR2_MSL)
             && (I2C_SR2(i2c) & I2C_SR2_BUSY) ));

     i2c_send_7bit_address(i2c, addr, I2C_WRITE);

     /* Waiting for address is transferred. */
     while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));

     /* Clearing ADDR condition sequence. */
     (void)I2C_SR2(i2c);

     for (size_t i = 0; i < n; i++) {
             i2c_send_data(i2c, data[i]);
             while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)));
     }
}

void i2c_read7(uint32_t i2c, int addr, uint8_t *res, size_t n)
{
     i2c_send_start(i2c);
     i2c_enable_ack(i2c);

     /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
     while ( !( (I2C_SR1(i2c) & I2C_SR1_SB)
             && (I2C_SR2(i2c) & I2C_SR2_MSL)
             && (I2C_SR2(i2c) & I2C_SR2_BUSY) ));

     i2c_send_7bit_address(i2c, addr, I2C_READ);

     /* Waiting for address is transferred. */
     while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
     /* Clearing ADDR condition sequence. */
     (void)I2C_SR2(i2c);

     for (size_t i = 0; i < n; ++i) {
             if (i == n - 1) {
                     i2c_disable_ack(i2c);
             }
             while (!(I2C_SR1(i2c) & I2C_SR1_RxNE));
             res[i] = i2c_get_data(i2c);
     }
     i2c_send_stop(i2c);

     return;
}
