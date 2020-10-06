/**
 * @file
 *
 * This file is part of cdfr2020CerveauProg
 *
 * @brief This implements the setup of communication between F3, F4 and other
 *        potential computers using CAN protocol.
 *
 * @date  10/2020
 *
 * Licence :
 *
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule JamesWright
*/

#pragma once

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

/**
 * @brief Frame of stantard received CAN messages and transmitted CAN messages
 * @struct can_tx_msg canmsgs.h
 * @param std_id    Unique identifier which also represents the message priority
 * @param ext_id    Dominant for standard frame. Recessive for extended frame
 * @param rtr       Dominant for data frames. Recessive for request frames
 * @param dlc       Data length code. Number of bytes of data
 * @param data      Data to be transmitted
 * @param crc       Cyclic redundancy check. Error detecting code
 * @param ack       Acknowledge the receipt of a valid CAN frame (dominant)
 */
struct can_tx_msg {
	uint32_t std_id;
	uint32_t ext_id;
	uint8_t rtr;
	uint8_t dlc;
	uint8_t data[8];
  uint8_t crc;
	uint8_t ack;
};

struct can_rx_msg {
	uint32_t std_id;
	uint32_t ext_id;
	uint8_t rtr;
  uint8_t fmi;
	uint8_t dlc;
	uint8_t data[8];
	uint8_t ts;
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
void transmit(uint32_t id, can_tx_msg tx_msg);
