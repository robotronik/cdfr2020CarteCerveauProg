#include "lowlevel/theyseemerolling.h"
#include "lowlevel/clock.h"
#include "lowlevel/eeprom.h"
#include "lowlevel/motors.h"
#include "lowlevel/uart.h"


#include "asservissement/odometry.h"

void led_blink();
void motor_blink();
void led_blink();
void test_encoders();
void test_direction_general();
void test_odometry();
void test_uart_send();

int main() {
  // Basic initialization
  clock_setup();
  gpio_setup();
  uart_setup();

  odometry_setup();

  // motors_setup();
  test_uart_send();
  test_odometry();

  return 0;
}


void led_blink() {
  while(true) {
    led_set_status(0);
    delay_ms(1000);
    led_set_status(1);
    delay_ms(1000);
  }
}

void test_uart_send() {
  while(true) {
    uart_send_string("hello world (sure ?)\n\r");
    led_toggle_status();
    delay_ms(300);
  }
}

void motor_blink() {
  double speed = 0.0;
  while(true) {
    speed = 0;
    motor_b_set(speed);
    motor_a_set(speed);
    led_set_status(0);
    delay_ms(1000);

    speed = 1;
    motor_b_set(speed);
    motor_a_set(speed);
    led_set_status(1);
    delay_ms(1000);

    speed = 0;
    motor_b_set(speed);
    motor_a_set(speed);
    led_set_status(0);
    delay_ms(1000);

    speed = -1;
    motor_b_set(speed);
    motor_a_set(speed);
    led_set_status(1);
    delay_ms(1000);
  }
}

void led_blink_smart() {
  bool status = about_da_power();
  while(true) {
    led_set_status(1);
    delay_ms(100);
    led_set_status(0);
    delay_ms(100);

    if (status)
      led_set_status(1);
    else
      led_set_status(0);
    delay_ms(100);
    led_set_status(0);
    delay_ms(700);
  }
}

void test_encoders() {
  odometry odom;


  while(1)
  {
    odom = odometry_get_position();

    uart_send_int(odom.left_count);
    uart_send_string(" ; ");

    uart_send_int(odom.right_count);
    uart_send_string("\n\r");

    delay_ms(100);
  }
}

void ramp_motors(double begin, double end, int delay_by_step) {
  // for ()



}

void test_direction_general() {
  motor_b_set(0.2);
  motor_a_set(0.2);

  odometry odom_old, odom_new;

  odom_old = odometry_get_position();

  while(1)
  {
    odom_old = odom_new;
    odom_new = odometry_get_position();

    if (odom_new.left_count > odom_old.left_count)
      uart_send_string(" + ");
    else if (odom_new.left_count < odom_old.left_count)
      uart_send_string(" - ");
    else
      uart_send_string(" 0 ");

    uart_send_string(";");

    if (odom_new.right_count > odom_old.right_count)
      uart_send_string(" + ");
    else if (odom_new.right_count < odom_old.right_count)
      uart_send_string(" - ");
    else
      uart_send_string(" 0 ");

    uart_send_string("\n\r");

    delay_ms(300);
  }

}

void test_odometry() {
  odometry odom;

  while(1)
  {
    odom = odometry_get_position();

    uart_send_string("\r\n");
    uart_send_string("\r\nl=");
    uart_send_int((int)odom.left_count);
    uart_send_string("\t; r=");
    uart_send_int((int)odom.right_count);

    uart_send_string("\r\nx=");
    uart_send_int((int)odom.x);
    uart_send_string("\t; y=");
    uart_send_int((int)odom.y);
    uart_send_string("\t; theta=");
    uart_send_int((int)(odom.theta*100));

    delay_ms(300);
  }

}
