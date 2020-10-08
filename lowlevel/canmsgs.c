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

  //  CAN1_NVIC_SCE was picked randomly among 4 other define (in order to compile)
  nvic_enable_irq(CAN1_NVIC_SCE);

  // enable gpio clock
  rcc_periph_clock_enable(CAN1_RX_RCC);
  rcc_periph_clock_enable(CAN1_TX_RCC);

  // Route the CAN signal to our selected GPIOs
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
                  CAN1_RX_PIN | CAN1_TX_PIN);
  gpio_set_af(GPIOA, GPIO_AF9, CAN1_RX_PIN | CAN1_TX_PIN);
}

// TODO can rx i guess
// void cec_can_isr() {
//   // Message pending on FIFO 0?
//   if (CAN_RF0R(GPIOX) & CAN_RF0R_FMP0_MASK) {
//     receive(0);
//   }
//   // Message pending on FIFO 1?
//   if (CAN_RF1R(GPIOX) & CAN_RF1R_FMP1_MASK) {
//     receive(1);
//   }
// }

// void can_rx1_isr(){
//   can_rx_isr(1,CAN_RF1R(CAN1)&3);
// }

void receive(uint8_t fifo) {
  // Copy CAN message data into main ram
  can_rx_msg rx_msg;
  can_receive(CAN1,
              fifo, // FIFO ID
              true, // Automatically release FIFO after rx
              &rx_msg.std_id, &rx_msg.ext_id, &rx_msg.rtr, &rx_msg.fmi,
              &rx_msg.dlc, rx_msg.data, &rx_msg.ts);
}

// void transmit(uint32_t id, can_tx_msg tx_msg) {
//   can_transmit(CAN1, id, tx_msg.ext_id, tx_msg.rtr, tx_message.dlc,
//                &tx_msg.data);
// }
