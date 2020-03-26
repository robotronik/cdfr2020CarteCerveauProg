#include "motors.h"

void motors_setup() {
  rcc_periph_clock_enable(PWM_RCC_TIM);//configuration of PWM timer
  timer_set_mode(PWM_TIM,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
	timer_enable_preload(PWM_TIM);
	timer_continuous_mode(PWM_TIM);
	timer_set_repetition_counter(PWM_TIM, 0);
  timer_enable_break_main_output(PWM_TIM);
  timer_set_prescaler(PWM_TIM, PWM_TIM_PRESCALER);
	timer_set_period(PWM_TIM, PWM_TIM_PERIOD);

  //PWM output configuration MotorA
  rcc_periph_clock_enable(MotorA_Enable_PORT_RCC);
	gpio_mode_setup(MotorA_Enable_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,MotorA_Enable_PIN);
	gpio_set_output_options(MotorA_Enable_PORT, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, MotorA_Enable_PIN);
	gpio_set_af(MotorA_Enable_PORT,MotorA_Enable_PIN_AF,MotorA_Enable_PIN);
	timer_set_oc_mode(PWM_TIM, MotorA_Enable_PIN_TIM_OC, MotorA_Enable_PIN_TIM_OCM_PWM);
	timer_enable_oc_preload(PWM_TIM, MotorA_Enable_PIN_TIM_OC);
  timer_set_oc_value(PWM_TIM, MotorA_Enable_PIN_TIM_OC, 0);//set 0 by default
  timer_enable_oc_output(PWM_TIM, MotorA_Enable_PIN_TIM_OC);

  //MotorA direction control
  rcc_periph_clock_enable(MotorA_Input1_PORT_RCC);
  gpio_mode_setup(MotorA_Input1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MotorA_Input1_PIN);
  rcc_periph_clock_enable(MotorA_Input2_PORT_RCC);
  gpio_mode_setup(MotorA_Input2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MotorA_Input2_PIN);

  //PWM output configuration MotorB
  rcc_periph_clock_enable(MotorB_Enable_PORT_RCC);
	gpio_mode_setup(MotorB_Enable_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,MotorB_Enable_PIN);
	gpio_set_output_options(MotorB_Enable_PORT, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, MotorB_Enable_PIN);
	gpio_set_af(MotorB_Enable_PORT,MotorB_Enable_PIN_AF,MotorB_Enable_PIN);
	timer_set_oc_mode(PWM_TIM, MotorB_Enable_PIN_TIM_OC, MotorB_Enable_PIN_TIM_OCM_PWM);
	timer_enable_oc_preload(PWM_TIM, MotorB_Enable_PIN_TIM_OC);
  timer_set_oc_value(PWM_TIM, MotorB_Enable_PIN_TIM_OC, 0);//set 0 by default
  timer_enable_oc_output(PWM_TIM, MotorB_Enable_PIN_TIM_OC);

  //MotorB direction control
  rcc_periph_clock_enable(MotorB_Input1_PORT_RCC);
  gpio_mode_setup(MotorB_Input1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MotorB_Input1_PIN);
  rcc_periph_clock_enable(MotorB_Input2_PORT_RCC);
  gpio_mode_setup(MotorB_Input2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MotorB_Input2_PIN);

  //start PWM
  timer_generate_event(PWM_TIM, TIM_EGR_UG);
  timer_enable_counter(PWM_TIM);
}

//signed voltage between -1 and 1
void motor_a_set(double signed_voltage) {
  uint32_t value = 0;

  signed_voltage*=MotorA_Inversion;//apply the polarity inversion, or not

  if (signed_voltage < 0.0) {//set the motor direction
    signed_voltage=-signed_voltage;
    gpio_set(MotorA_Input1_PORT, MotorA_Input1_PIN);
    gpio_clear(MotorA_Input2_PORT, MotorA_Input2_PIN);
  }
  else {
    gpio_clear(MotorA_Input1_PORT, MotorA_Input1_PIN);
    gpio_set(MotorA_Input2_PORT, MotorA_Input2_PIN);
  }

  if (signed_voltage>max_voltage)//limit the maximum voltage
  {
    signed_voltage=max_voltage;
  }
  value=signed_voltage*PWM_TIM_PERIOD;

  timer_set_oc_value(PWM_TIM, MotorA_Enable_PIN_TIM_OC, value);
}

void motor_b_set(double signed_voltage) {
  uint32_t value = 0;

  signed_voltage*=MotorB_Inversion;

  if (signed_voltage < 0.0) {//set the motor direction
    signed_voltage=-signed_voltage;
    gpio_set(MotorB_Input1_PORT, MotorB_Input1_PIN);
    gpio_clear(MotorB_Input2_PORT, MotorB_Input2_PIN);
  }
  else {
    gpio_clear(MotorB_Input1_PORT, MotorB_Input1_PIN);
    gpio_set(MotorB_Input2_PORT, MotorB_Input2_PIN);
  }

  if (signed_voltage>max_voltage)//limit the maximum voltage
  {
    signed_voltage=max_voltage;
  }
  value=signed_voltage*PWM_TIM_PERIOD;

  timer_set_oc_value(PWM_TIM, MotorB_Enable_PIN_TIM_OC, value);
}
