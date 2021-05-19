/**
 * @file
 *
 * This file is part of cdfr2020CerveauProg
 *
 * @brief This implements the setup of communication between F3, F4 and other
 *        potential computers using CAN protocol.
 *		  Source: https://www.rhye.org/post/stm32-with-opencm3-3-canbus/
 * @date  10/2020
 *
 * Licence :
 *
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule JamesWright Floorcows KameradJS(Nornort)
 */

#pragma once

#include "gpio.h"

/* #include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> */

#include <stdio.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>


// can clock is on APB1, there is a /2 prescaler compared to cpu clock
// this means you have to do calculations with a 42MHz clock for the CAN peripheral
// TODO: check if the prescale comes fromp APB or AHB

//// Bit timing settings
//// http://www.bittiming.can-wiki.info/
// Setting is currently 400 kbit/s CAN_BTR should be 0x001b0006
// Resync time quanta jump width
#define PARAM_SJW CAN_BTR_SJW_1TQ
// Time segment 1 time quanta width
#define PARAM_TS1 CAN_BTR_TS1_12TQ //CAN_BTR_TS1_11TQ
// Time segment 2 time quanta width
#define PARAM_TS2 CAN_BTR_TS2_2TQ //CAN_BTR_TS2_2TQ
// Baudrate prescaler
#define PARAM_BRP 7
// sample point 86.7

#define CAN1_RX_PORT GPIOB
#define CAN1_RX_PIN GPIO8
#define CAN1_RX_RCC RCC_GPIOB
#define CAN1_RX_AF GPIO_AF9
//#define CAN CAN1 ID ??
#define CAN1_TX_PORT GPIOB
#define CAN1_TX_PIN GPIO9
#define CAN1_TX_RCC RCC_GPIOB
#define CAN1_TX_AF GPIO_AF9
//#define CAN1_NVIC NVIC_CEC_CAN_IRQ           Not defined for F4
// Which could replace it?
#define CAN1_NVIC_TX NVIC_CAN1_TX_IRQ
#define CAN1_NVIC_RX0 NVIC_CAN1_RX0_IRQ
#define CAN1_NVIC_RX1 NVIC_CAN1_RX1_IRQ
#define CAN1_NVIC_SCE NVIC_CAN1_SCE_IRQ

/**
 * @brief Frame of stantard transmitted CAN messages
 * @struct can_tx_msg canmsgs.h
 * @param std_id    Unique identifier which also represents the message priority
 * @param ext_id    Dominant for standard frame. Recessive for extended frame
 * @param rtr       Dominant for data frames. Recessive for request frames
 * @param fmi       ID of the matched filter
 * @param dlc       Data length code. Number of bytes of data
 * @param data      Data to be transmitted
 * @param crc       Cyclic redundancy check. Error detecting code
 * @param ack       Acknowledge the receipt of a valid CAN frame (dominant)
 * @param ts       {Timestamp. Pointer to store the message timestamp.
 *                  Only valid on time triggered CAN. Use NULL to ignore.}
 */
struct Can_tx_msg {
  uint32_t std_id;
  bool ext_id;
  bool rtr;
  uint8_t fmi;
  uint8_t dlc;
  uint8_t data[8];
  uint8_t crc;
  uint8_t ack;
  uint16_t ts;
};

/**
 * @brief Frame of stantard received CAN messages
 * @struct Can_rx_msg canmsgs.h
 * @param std_id    Unique identifier which also represents the message priority
 * @param ext_id    Dominant for standard frame. Recessive for extended frame
 * @param rtr       Dominant for data frames. Recessive for request frames
 * @param fmi       ID of the matched filter
 * @param dlc       Data length code. Number of bytes of data
 * @param data      Data to be transmitted
 * @param crc       Cyclic redundancy check. Error detecting code
 * @param ack       Acknowledge the receipt of a valid CAN frame (dominant)
 * @param ts       {Timestamp. Pointer to store the message timestamp.
 *                  Only valid on time triggered CAN. Use NULL to ignore.}
 */
struct Can_rx_msg {
  uint32_t std_id;
  bool ext_id;
  bool rtr;
  uint8_t fmi;
  uint8_t dlc;
  uint8_t data[8];
  uint8_t crc;
  uint8_t ack;
  uint16_t ts;
};

typedef struct Can_tx_msg Can_tx_msg;
typedef struct Can_rx_msg Can_rx_msg;

/**
 * @brief FIFO linked list to store impeding CAN messages (software side) 
 * @struct can_msg_buffer_list_t canmsgs.h
 * @param data A can mesage
 * @param next Pointer to the next element in the list
 */
typedef struct can_msg_buffer_list_t can_msg_buffer_list_t;
struct can_msg_buffer_list_t{
    Can_rx_msg data;
    can_msg_buffer_list_t * next;
};
// can_msg_buffer_list_t * can_msg_buffer_list;

/**
 * @brief Appends a can message at the end of the global can buffer list
 * @param rx_msg The can message to be appended
 */
void _can_msg_buffer_append( Can_rx_msg rx_msg );

/**
 * @brief Pops the first element of the global can buffer list
 * @param rx_msg Pointer to the variable where the can message will be stored
 * @return 0: a can message was found and stored in rx_msg, 1: the list was empty
 */
int can_msg_buffer_pop( Can_rx_msg * rx_msg );
/**
 * @brief Startup configuration of the CAN system
 *
 */
void can_setup();

/**
 * @brief This function manages messages pending on FIFO 0 and 1
 *
 */
void cec_can_isr();

/**
 * @brief This function receives the message and push it in a FIFO
 * @param fifo    The FIFO in which the message is pushed
 */
void receive(uint8_t fifo);

/**
 * @brief This function transmits a message
 * @param id        id of the message to be transmitted
 * @param tx_msg    structure of the message to transmit
 */
// {In order to transmit a message, the application must select one empty 
// transmit mailbox, set up the identifier, the data length code (DLC) and the
// data before requesting the transmission by setting the corresponding TXRQ bit
// in the CAN_TIxR register.}
void transmit(uint32_t id, Can_tx_msg tx_msg);

void can1_rx1_isr();
void can1_rx0_isr();
void can1_sce_isr();