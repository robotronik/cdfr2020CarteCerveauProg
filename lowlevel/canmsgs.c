/**
 * @file
 *
 * This file is part of cdfr2020CarteCerveauProg
 *
 * @brief This implements the setup of CAN protocol to allow F3, F4 and other
 *        potential computers to communicate
 *        Source: https://www.rhye.org/post/stm32-with-opencm3-3-canbus/
 *
 * @date  10/2020
 *
 * Licence :
 *
 * Robotronik Phelma
 * @author NPXav Benano JamesWright
 */

#include "canmsgs.h"
#include <stdlib.h>

void can_setup() {
  // Enable clock to the CAN peripheral
  rcc_periph_clock_enable(RCC_CAN1);

  // Reset the CAN peripheral
  can_reset(CAN1);

  // Initialize the CAN peripheral
  can_init(CAN1,  // The CAN ID
           false, // Time Trigger Communication mode? No
           true,  // Automatic bus-off management? Yes
           true,  // Automatic wakeup mode? Wakeup on message rx
           false, // No automatic transmit? Do auto-retry
           false, // Receive FIFO locked mode? Discard older messages over newer
           false, // Transmit FIFO priority ? Tx priority based on identifier
           // Bit timing settings
           PARAM_SJW, // Resync time quanta jump width
           PARAM_TS1, // Time segment 1 time quanta width
           PARAM_TS2, // Time segment 2 time quanta width
           PARAM_BRP, // Baudrate prescaler
           // Loopback mode
           false, // If set, CAN can transmit but not receive
           // Silent mode
           false); // If set, CAN can receive but not transmit

  // Enable CAN interrupts for FIFO message pending (FMPIE)
  // no idea why there are two interrupts to enable here
  // actually there are two FIFO so two interrupt for new messages to check
  can_enable_irq(CAN1_RX_PORT, CAN_IER_FMPIE0 | CAN_IER_FMPIE1);

  // Transmit mailbox empty interrupt enable
  //can_enable_irq(CAN1_RX_PORT, CAN_IER_TMEIE);

  // Error interrupt enable
  //can_enable_irq(CAN1_RX_PORT, CAN_IER_ERRIE);

  //enable the entries related to both FIFO in NVIC table
  nvic_enable_irq(CAN1_NVIC_RX0);
  nvic_enable_irq(CAN1_NVIC_RX1);


  //set alternate function on the pin to use CAN
  _gpio_setup_pin_af(CAN1_RX_RCC,CAN1_RX_PORT,CAN1_RX_PIN,CAN1_RX_AF,GPIO_PUPD_NONE,GPIO_OTYPE_PP);
  _gpio_setup_pin_af(CAN1_TX_RCC,CAN1_TX_PORT,CAN1_TX_PIN,CAN1_TX_AF,GPIO_PUPD_NONE,GPIO_OTYPE_PP);



  // filter for later
  //    // Initialisation filter 0
  //    can_filter_id_mask_16bit_init(
  //      0,                                 // Filter bank 0
  //      0x000 << 5, 0x001 << 5,            // LSB == 0
  //  		0x000 << 5, 0x001 << 5,            // Not used
  //  		0,                                 // FIFO 0
  //  		true);                             // enables the filter
  //
  //    // Initialisation filter 1
  //    can_filter_id_mask_16bit_init(
  //      1,                                 // Filter bank 1
  //      0x000 << 5, 0x001 << 5,            // LSB == 0
  //  		0x000 << 5, 0x001 << 5,            // Not used
  //  		1,                                 // FIFO 1
  //  		true);

}

// ISR for both FIFO reception
// TODO name FIFO_O and FIFO_1
void can_rx0_isr(){
  receive(0);
}

void can_rx1_isr(){
  receive(1);
}

void receive(uint8_t fifo) {
  // Copy CAN message data into main ram
  Can_rx_msg rx_msg;
  can_receive(CAN1,
              fifo, // FIFO ID
              true, // Automatically release FIFO after rx
              &rx_msg.std_id, &rx_msg.ext_id, &rx_msg.rtr, &rx_msg.fmi,
              &rx_msg.dlc, rx_msg.data, &rx_msg.ts);
  // Append the message a global can message buffer
  _can_msg_buffer_append( rx_msg );
}

void _can_msg_buffer_append( Can_rx_msg rx_msg ){
  // Edge case: the linked list is empty
  if( NULL == can_msg_buffer_list ){
    can_msg_buffer_list = calloc(1, sizeof(can_msg_buffer_list_t));
    can_msg_buffer_list->next = NULL;
    can_msg_buffer_list->data = rx_msg;
    return;
  }
  // Adding the message to the end of the linked list
  can_msg_buffer_list_t * p = can_msg_buffer_list;
  while( NULL != p->next ){
    p = p->next;
  }
  p->next = calloc(1, sizeof(can_msg_buffer_list_t));
  p->next->data = rx_msg;
  p->next->next = NULL;
}

int can_msg_buffer_pop( Can_rx_msg * rx_msg ){
  // Checking if the linked list is empty
  if( NULL == can_msg_buffer_list ){
    return 1;
  }
  // Fetching and deleting the first element of the linked list
  *rx_msg = can_msg_buffer_list->data;
  can_msg_buffer_list_t * buffer = can_msg_buffer_list;
  can_msg_buffer_list = can_msg_buffer_list->next;
  free(buffer);
  return 0;
}

void transmit(uint32_t id, Can_tx_msg tx_msg) {
  can_transmit(CAN1, id, tx_msg.ext_id, tx_msg.rtr, tx_msg.dlc,
               &tx_msg.data);
}

// TODO user function that takes in argument the message data and set all the parameters
