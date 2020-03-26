#include "odometry.h"
#include "calibration.h"

#include "lowlevel/encoders.h"
#include "lowlevel/uart.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include <math.h>//LDLIBS+= -lm

// WARNING global odometry variable for update during interruption
volatile odometry odometry_internal;
// do not use directly outside odometry.c

void odometry_setup()
{
  //gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

  //setup the pooling timer
	rcc_periph_clock_enable(ODOM_RCC_TIM);
	rcc_periph_reset_pulse(ODOM_RST_TIM);
	timer_set_mode(ODOM_TIM, TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(ODOM_TIM, ODOM_TIM_PRESCALER);
	timer_disable_preload(ODOM_TIM);
	timer_continuous_mode(ODOM_TIM);
	timer_set_period(ODOM_TIM, ODOM_TIM_PERIOD);
	timer_set_oc_value(ODOM_TIM, TIM_OC1,0);//compare to zero
	timer_enable_counter(ODOM_TIM);

  //start the encoders
  encoder_left_setup();
  encoder_right_setup();

  odometry_internal.x=0;//init the odometry structure
  odometry_internal.y=0;
  odometry_internal.theta=0;
  odometry_internal.left_count=encoder_right_get_counter();
  odometry_internal.right_count=encoder_left_get_counter();
  odometry_internal.left_total_count=0;
  odometry_internal.right_total_count=0;

  //start pooling
  nvic_enable_irq(ODOM_NVIC_TIM_IRQ);
	timer_enable_irq(ODOM_TIM, TIM_DIER_CC1IE);
}

void timX_isr(void)
{
  if (timer_get_flag(ODOM_TIM, TIM_SR_CC1IF))
  {
    // Clear compare interrupt flag.
    timer_clear_flag(ODOM_TIM, TIM_SR_CC1IF);

    // first read the encoders, should not be different than -1, 0 or 1
    int dl_l = encoder_left_update (&odometry_internal.left_count);
    int dl_r = encoder_right_update(&odometry_internal.right_count);
    odometry_internal.left_total_count+=dl_l;
    odometry_internal.right_total_count+=dl_r;

    double tmp_sigma = (Encoders_Dist_Per_Step_l * dl_l + Encoders_Dist_Per_Step_r * dl_r)/2;

    // update the position
    odometry_internal.x += tmp_sigma * cos(odometry_internal.theta);
    odometry_internal.y += tmp_sigma * sin(odometry_internal.theta);
    odometry_internal.theta += (-Encoders_Theta_Per_Diff_l * dl_l+ Encoders_Theta_Per_Diff_r * dl_r);

    //limit robot angle between -Pi and Pi
    if(odometry_internal.theta > Pi)
    {
      odometry_internal.theta += -2*Pi;
    }
    else if(odometry_internal.theta < -Pi)
    {
      odometry_internal.theta += +2*Pi;
    }
  }
}

odometry odometry_get_position()
{
	odometry_internal.left_total_distance=odometry_internal.left_total_count*Encoders_Dist_Per_Step_l;
	odometry_internal.right_total_distance=odometry_internal.right_total_count*Encoders_Dist_Per_Step_r;
  return odometry_internal;
}


void print_odometry(odometry *odom) {
  uart_send_string("\r\n");
  uart_send_string("\r\nl=");
  uart_send_int		((int)odom->left_count);
	uart_send_string(" ");
	uart_send_int		((int)odom->left_total_distance);
  uart_send_string("\t; r=");
  uart_send_int		((int)odom->right_count);
	uart_send_string(" ");
	uart_send_int		((int)odom->right_total_distance);

  uart_send_string("\r\nx=");
  uart_send_int		((int)odom->x);
  uart_send_string("\t; y=");
  uart_send_int		((int)odom->y);
  uart_send_string("\t; theta=");
  uart_send_int		((int)(odom->theta*180.0/Pi));
}
