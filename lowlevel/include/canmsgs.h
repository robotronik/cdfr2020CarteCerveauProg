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
 * @author NPXav Benano Trukbidule JamesWright
 */

#pragma once

#include "gpio.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

//// Bit timing settings
//// Assuming 48MHz base clock, 87.5% sample point, 500 kBit/s data rate
//// http://www.bittiming.can-wiki.info/
// Resync time quanta jump width
#define PARAM_SJW CAN_BTR_SJW_1TQ
// Time segment 1 time quanta width
#define PARAM_TS1 CAN_BTR_TS1_11TQ // 13,
// Time segment 2 time quanta width
#define PARAM_TS2 CAN_BTR_TS2_4TQ // 2,
// Baudrate prescaler
#define PARAM_BRP 6

#define CAN1_RX_PORT GPIOB
#define CAN1_RX_PIN GPIO8
#define CAN1_RX_RCC RCC_GPIOB
//#define CAN CAN1 ID ??
#define CAN1_TX_PORT GPIOB
#define CAN1_TX_PIN GPIO9
#define CAN1_TX_RCC RCC_GPIOB
//#define CAN1_NVIC NVIC_CEC_CAN_IRQ           Not defined for F4
// Which could replace it?
#define CAN1_NVIC_TX NVIC_CAN1_TX_IRQ
#define CAN1_NVIC_RX0 NVIC_CAN1_RX0_IRQ
#define CAN1_NVIC_RX1 NVIC_CAN1_RX1_IRQ
#define CAN1_NVIC_SCE NVIC_CAN1_SCE_IRQ

/**
 * @brief Frame of stantard received CAN messages and transmitted CAN messages
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
struct can_tx_msg {
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

struct can_rx_msg {
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

typedef struct can_tx_msg can_tx_msg;
typedef struct can_rx_msg can_rx_msg;

/**
 * @brief This function setup the CAN system
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
void transmit(uint32_t id, can_tx_msg tx_msg);
