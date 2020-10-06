/**
 * @file
 *
 * This file is part of cdfr2020CarteCerveauProg
 *
 * @brief This implements the setup of CAN protocol to allow F3, F4 and other
 *        potential computers to communicate
 *
 *
 * @date  10/2020
 *
 * Licence :
 *
 * Robotronik Phelma
 * @author NPXav Benano JamesWright
*/

#include "comcan.h"

void can_setup(){
  // Enable clock to the CAN peripheral
  rcc_periph_clock_enable(RCC_CAN1);

  // Reset the CAN peripheral
  can_reset(CAN1);

  // Initialize the CAN peripheral
  can_init (
    CAN1,                                 // The CAN ID
    false,                                // Time Trigger Communication mode? No
    true,                                 // Automatic bus-off management? Yes
    true,                                 // Automatic wakeup mode? Wakeup on message rx
    false,                                // No automatic transmit? Do auto-retry
    false,                                // Receive FIFO locked mode? Discard older messages over newer
    false,                                // Transmit FIFO priority ? Tx priority based on identifier
    // Bit timing settings
    PARM_SJW,                             // Resync time quanta jump width
    PARM_TS1,                             // Time segment 1 time quanta width
    PARM_TS2,                             // Time segment 2 time quanta width
    PARM_BRP,                             // Baudrate prescaler
    // Loopback mode
    false,                                // If set, CAN can transmit but not receive
    // Silent mode
    false );                              // If set, CAN can receive but not transmit

    // Enable CAN interrupts for FIFO message pending (FMPIE)
    can_enable_irq(GPIOX, CAN_IER_FMPIE0 | CAN_IER_FMPIE1);

    // Initialisation filter 0
    can_filter_id_mask_16bit_init(
      0,                                 // Filter bank 0
      0x000 << 5, 0x001 << 5,            // LSB == 0
  		0x000 << 5, 0x001 << 5,            // Not used
  		0,                                 // FIFO 0
  		true);                             // enables the filter

    // Initialisation filter 1
    can_filter_id_mask_16bit_init(
      1,                                 // Filter bank 1
      0x000 << 5, 0x001 << 5,            // LSB == 0
  		0x000 << 5, 0x001 << 5,            // Not used
  		1,                                 // FIFO 1
  		true);
}
//nvic_enable_irq(NVIC_CEC_CAN_IRQ);

  // Route the CAN signal to our selected GPIOs
  const uint16_t pins = GPIO11 | GPIO12;
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, pins);
  gpio_set_af(GPIOA, GPIO_AF4, pins);

void cec_can_isr(){
    // Message pending on FIFO 0?
    if (CAN_RF0R(GPIOX) & CAN_RF0R_FMP0_MASK){
      receive(0);
    }
   // Message pending on FIFO 1?
    if (CAN_RF1R(GPIOX) & CAN_RF1R_FMP1_MASK){
      receive(1);
    }
  }

// void can_rx1_isr(){
//   can_rx_isr(1,CAN_RF1R(CAN1)&3);
// }

void receive(uint8_t fifo){
    // Copy CAN message data into main ram
    can_rx_msg rx_msg;
    can_receive(CAN1,
                fifo, //FIFO ID
                true, // Automatically release FIFO after rx
                &rx_msg.std_id, &rx_msg.ext_id, &rx_msg.rtr,
                &rx_msg.fmi, &rx_msg.dlc, &rx_msg.data, &rx_msg.ts);
  }

void transmit(uint32_t id, can_tx_msg tx_msg){
  can_transmit(CAN1, id, tx_msg.ext_id, tx_msg.rtr,
  		         tx_message.dlc, &tx_msg.data);
             }
