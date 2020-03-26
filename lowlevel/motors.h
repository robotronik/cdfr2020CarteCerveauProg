#pragma once

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define max_voltage 0.4 //in absolute value between 0.0 and 1.0 to limit the motors power

//Timer for PWM
#define PWM_TIM           TIM3
#define PWM_RCC_TIM       RCC_TIM3
#define PWM_TIM_PERIOD    1000
#define PWM_TIM_PRESCALER 1

#define MotorA_Inversion (-1.0) //1 no reverse, -1 reverse
#define MotorB_Inversion (-1.0) //1 no reverse, -1 reverse


// Motor A :
#define MotorA_Enable_PORT_RCC RCC_GPIOA
#define MotorA_Enable_PORT GPIOA
#define MotorA_Enable_PIN  GPIO4

#define MotorA_Enable_PIN_TIM_OC TIM_OC2
#define MotorA_Enable_PIN_TIM_OCM_PWM TIM_OCM_PWM1
#define MotorA_Enable_PIN_AF GPIO_AF2

#define MotorA_Input1_PORT_RCC RCC_GPIOA
#define MotorA_Input1_PORT GPIOA
#define MotorA_Input1_PIN  GPIO3

#define MotorA_Input2_PORT_RCC RCC_GPIOA
#define MotorA_Input2_PORT GPIOA
#define MotorA_Input2_PIN  GPIO2

// Motor B :
#define MotorB_Enable_PORT_RCC RCC_GPIOA
#define MotorB_Enable_PORT GPIOA
#define MotorB_Enable_PIN  GPIO6

#define MotorB_Enable_PIN_TIM_OC TIM_OC1
#define MotorB_Enable_PIN_TIM_OCM_PWM TIM_OCM_PWM1
#define MotorB_Enable_PIN_AF GPIO_AF2

#define MotorB_Input1_PORT_RCC RCC_GPIOA
#define MotorB_Input1_PORT GPIOA
#define MotorB_Input1_PIN  GPIO7

#define MotorB_Input2_PORT_RCC RCC_GPIOA
#define MotorB_Input2_PORT GPIOA
#define MotorB_Input2_PIN  GPIO5

void motors_setup();
void motor_a_set(double signed_voltage);//normalized voltage between -1 and 1
void motor_b_set(double signed_voltage);
