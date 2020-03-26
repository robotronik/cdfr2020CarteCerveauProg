#include "lowlevel/theyseemerolling.h"
#include "lowlevel/clock.h"
#include "lowlevel/uart.h"
#include "lowlevel/motors.h"
#include "lowlevel/gpio.h"
#include "lowlevel/nvic.h"



int main() {
  //setup

  clock_setup();
  //gpio_setup();
  // interrupt_gpio_setup();
  uart_setup();
  //motors_setup();
  //odometry_setup();
  led_set_status(1);
  int customdelay = 100;  

  //pwm test
  initTimer();
  pwmSetup();
  timer_set_oc_value(PWM_LED_TIM, GPIO_LED_PIN_OC, 500);//set 0 by default


  while(1){

    //led test/button
    //led_set_status(0);
    //  led_toggle_status();
    //  delay_ms(customdelay);


    // if(!read_bluebutton()){  
    //   led_set_status(1);
    // }

    //uart test
    //uart_send_int('o');
    uart_send_string("test\r\n");







  }

  return (0);
}