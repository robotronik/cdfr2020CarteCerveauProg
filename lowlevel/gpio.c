#include "gpio.h"


void gpio_setup() {
  //enable port gpio clock
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);
  // status led
  gpio_mode_setup(GPIO_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED_PIN);
  
  
  // test blue button
  gpio_mode_setup(GPIO_BLUEBUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_BLUEBUTTON_PIN);
  
  // power input
  /* gpio_mode_setup(GPIO_POW_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_POW_PIN); */
}



//PWM TEST on the nucleo led
void initTimer(){
  rcc_periph_clock_enable(PWM_LED_RCC_TIM);
  timer_set_mode(PWM_LED_RCC_TIM,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
  timer_enable_preload(PWM_LED_TIM); //shadow register?? update on UEV ???
	timer_continuous_mode(PWM_LED_TIM); //as opposed to one shot mode: one cycle and stops
	timer_set_repetition_counter(PWM_LED_TIM, 0); //repeat event after set number of completed count cycle
  timer_enable_break_main_output(PWM_LED_TIM);//required for advanced timers
  timer_set_prescaler(PWM_LED_TIM, PWM_LED_TIM_PRESCALER);
	timer_set_period(PWM_LED_TIM, PWM_LED_TIM_PERIOD); // in counter clock tick
}

void pwmSetup(){
  rcc_periph_clock_enable(GPIO_LED_PORT_RCC);
	gpio_mode_setup(GPIO_LED_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_LED_PIN);
	gpio_set_output_options(GPIO_LED_PORT, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_LED_PIN); //why ?
	gpio_set_af(GPIO_LED_PORT,GPIO_LED_AF,GPIO_LED_PIN);
	timer_set_oc_mode(PWM_LED_TIM, GPIO_LED_PIN_OC, GPIO_LED_PIN_OCM_PWM);
	timer_enable_oc_preload(PWM_LED_TIM, GPIO_LED_PIN_OC);
  timer_set_oc_value(PWM_LED_TIM, GPIO_LED_PIN_OC, 0);//set 0 by default
  timer_enable_oc_output(PWM_LED_TIM, GPIO_LED_PIN_OC);

  //start PWM
  timer_generate_event(PWM_LED_TIM, TIM_EGR_UG);
  timer_enable_counter(PWM_LED_TIM);
}



void led_toggle_status() {
  gpio_toggle(GPIO_LED_PORT, GPIO_LED_PIN);
}

void led_set_status(uint32_t status) {
  if (status == 0)
    gpio_clear(GPIO_LED_PORT, GPIO_LED_PIN);
  else
    gpio_set  (GPIO_LED_PORT, GPIO_LED_PIN);
}

bool about_da_power() {
  // return (gpio_get(GPIO_POW_PORT, GPIO_POW_PIN) == 0);
  return false;
}

int read_bluebutton(){
   if(gpio_get(GPIO_BLUEBUTTON_PORT,GPIO_BLUEBUTTON_PIN) == 1<<13){
     return(1);
   }
   return(0);
}
