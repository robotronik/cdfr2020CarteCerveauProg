#pragma once

// POOLING Timer
#define ODOM_TIM TIM15
// magic trick of timer 15 normaly timX_isr
#define timX_isr(void) tim1_brk_tim15_isr(void)
#define ODOM_RCC_TIM RCC_TIM15
// normaly NVIC_TIMx_IRQ
#define ODOM_NVIC_TIM_IRQ NVIC_TIM1_BRK_TIM15_IRQ
#define ODOM_RST_TIM RST_TIM15
// change these 2 lines to setup interruption frequency
// #define ODOM_TIM_PRESCALER ((rcc_apb1_frequency * 2) / 5000)
#define ODOM_TIM_PRESCALER 50
#define ODOM_TIM_PERIOD 600

typedef struct{//positions in mm and angles in rad
  double x;
  double y;
  double theta;

  int left_count;//last encoders count
  int left_total_count;//do not use this in asser
  double left_total_distance;//in mm update only when odometry_get_position called
  int right_count;
  int right_total_count;
  double right_total_distance;//in mm update only when odometry_get_position called
} odometry;

void odometry_setup();
odometry odometry_get_position();

void print_odometry(odometry *odom);
