#pragma once

/***************************************
simple uart configuration for debugging
***************************************/

// DEFINES FOR DEBUG_UART
#define DEBUG_RCC_USART RCC_USART2
#define DEBUG_USART USART2
#define DEBUG_UART_SPEED (9600)
// Bug in libopencm3 ? Need to *2 because of APB2/APB1 weird things.  !!!!Removed it atm !!!

#define DEBUG_PORT_TX GPIOA
#define DEBUG_PORT_TX_RCC RCC_GPIOA // clock of GPIO port
#define DEBUG_PIN_TX GPIO2
#define DEBUG_AF_TX GPIO_AF7

#define DEBUG_PORT_RX GPIOA
#define DEBUG_PORT_RX_RCC RCC_GPIOA
#define DEBUG_PIN_RX GPIO3
#define DEBUG_AF_RX GPIO_AF7

#define COMM_PORT_TX GPIOA
#define COMM_PORT_TX_RCC RCC_GPIOA // clock of GPIO port
#define COMM_PIN_TX GPIO9
#define COMM_AF_TX GPIO_AF7

#define COMM_PORT_RX GPIOA
#define COMM_PORT_RX_RCC RCC_GPIOA
#define COMM_PIN_RX GPIO10
#define COMM_AF_RX GPIO_AF7


void uart_setup();
void uart_send_string(char* chain); // NULL-ended string
void uart_send_int(int integer);

// Warning : hard-limit on string length to 80 chars
void uart_send_string_formatted(const char *fmt, ...);

//rec=usart_recv_blocking(DEBUG_USART);//to receive a byte
