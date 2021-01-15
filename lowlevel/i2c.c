#include "i2c.h"

void i2c_setup(uint32_t i2c_peripheral){
    switch (i2c_peripheral){
    case I2C1:
        //gpio setup for SCL line
        _gpio_setup_pin_af(I2C1_SCL_GPIO_RCC, I2C1_SCL_GPIO_PORT,
                           I2C1_SCL_GPIO_PIN, I2C1_SCL_AF, I2C_GPIO_PULL_UP, I2C_GPIO_OTYPE);
        //gpio setup for SDA line
        _gpio_setup_pin_af(I2C1_SDA_GPIO_RCC, I2C1_SDA_GPIO_PORT,
                           I2C1_SDA_GPIO_PIN, I2C1_SDA_AF, I2C_GPIO_PULL_UP, I2C_GPIO_OTYPE);

        /* Enable timer clock for I2C1*/
        rcc_periph_clock_enable(RCC_I2C1);
        /* Disable peripheral before changing parameters */
        i2c_peripheral_disable(I2C1);
        i2c_reset(I2C1);
        i2c_set_speed(I2C1, i2c_speed_fm_400k, (rcc_apb1_frequency / 1000000));
        i2c_peripheral_enable(I2C1);

        break;
    case I2C2:
        // NOT DONE
        /* Enable timer clock */
        rcc_periph_clock_enable(RCC_I2C2);

        /* Disable peripheral before changing parameters */
        i2c_peripheral_disable(I2C2);
        i2c_set_speed(I2C2, i2c_speed_fm_400k, (rcc_apb1_frequency / 1000000));
        i2c_peripheral_enable(I2C2);

        break;
    }
}

//copie depuis libopencm3
I2C_status i2c_write7(uint32_t i2c, int addr, uint8_t *data, size_t n){
    uint32_t start_time = _clock_get_systicks(); //in ms

    while ((I2C_SR2(i2c) & I2C_SR2_BUSY)){
        if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
            i2c_send_stop(i2c);
            return (I2C_TIMEOUT);
        }
    }
    //fprintf(stderr, "send start\n");
    i2c_send_start(i2c);
    start_time = _clock_get_systicks();
    /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
    while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & I2C_SR2_MSL) && (I2C_SR2(i2c) & I2C_SR2_BUSY))){
        if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
            i2c_send_stop(i2c);
            return (I2C_TIMEOUT);
        }
    }

    //fprintf(stderr, "start condition passed\n");

    i2c_send_7bit_address(i2c, addr, I2C_WRITE);
    start_time = _clock_get_systicks();
    /* Waiting for address is transferred. */
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR)){
        if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
            i2c_send_stop(i2c);
            return (I2C_TIMEOUT);
        }
    }
    //fprintf(stderr, "adress transferred\n");

    /* Clearing ADDR condition sequence. */
    (void)I2C_SR2(i2c);

    for (size_t i = 0; i < n; i++){
        i2c_send_data(i2c, data[i]);
        start_time = _clock_get_systicks();
        while (!(I2C_SR1(i2c) & (I2C_SR1_BTF))){
            if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
                i2c_send_stop(i2c);
                return (I2C_TIMEOUT);
            }
        }
        //fprintf(stderr, "data[ %d ]transferred\n", i);
    }
    i2c_send_stop(i2c);
    return (I2C_OK);
}

I2C_status i2c_read7(uint32_t i2c, int addr, uint8_t *res, size_t n){
    uint32_t start_time = _clock_get_systicks(); //in ms

    i2c_send_start(i2c);
    i2c_enable_ack(i2c);

    /* Wait for the end of the start condition, master mode selected, and BUSY bit set */
    while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & I2C_SR2_MSL) && (I2C_SR2(i2c) & I2C_SR2_BUSY))){
        if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
            i2c_send_stop(i2c);
            return (I2C_TIMEOUT);
        }
    }
    i2c_send_7bit_address(i2c, addr, I2C_READ);
    start_time = _clock_get_systicks();

    /* Waiting for address is transferred. */
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR))if((_clock_get_systicks()-start_time)>I2C_MAX_TIMEOUT){
        i2c_send_stop(i2c);
        return(I2C_TIMEOUT);
    }
    /* Clearing ADDR condition sequence. */
    (void)I2C_SR2(i2c);

    for (size_t i = 0; i < n; ++i){
        if (i == n - 1){
            i2c_disable_ack(i2c);
        }
        start_time = _clock_get_systicks();
        while (!(I2C_SR1(i2c) & I2C_SR1_RxNE)){
            if ((_clock_get_systicks() - start_time) > I2C_MAX_TIMEOUT){
                i2c_send_stop(i2c);
                return (I2C_TIMEOUT);
            }
        }
        res[i] = i2c_get_data(i2c);
    }
    i2c_send_stop(i2c);

    return(I2C_OK);
}
