/*
   Copyright (C) 2007-2015 by krasnop@bellsouth.net (Alexei Krasnopolski)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include "gpio.hpp"
#include "bcm_registers.hpp"
#include "arm_debug_tools.hpp"

unsigned char read_rx_buffer();

unsigned char uart_recv_not_interrupt() {
  volatile unsigned int status;
  do {
    status = *pAUX_MU_LSR_REG;
//    status = *pAUX_MU_STAT_REG;
    if ((status & 0x2) != 0) {
      return '*';
    }
  } while ((status & 0x01) == 0);
  return (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
}

extern "C"
unsigned char uart_recv() {
  return read_rx_buffer();
//  return uart_recv_not_interrupt();
}

//------------------------------------------------------------------------

volatile bool flag;
volatile int txhead;
volatile int txtail;
#define TXBUFMASK 0xFFF
volatile unsigned char txbuffer[TXBUFMASK+1];

//------------------------------------------------------------------------
extern "C"
void uart_send(char c) {
  txbuffer[txhead] = c;
  txhead = (txhead + 1) & TXBUFMASK;
  if(!flag) { // transmit is not active
    (*pAUX_MU_IO_REG) = (unsigned int) txbuffer[txtail];
    txtail = (txtail + 1) & TXBUFMASK;
    (*pAUX_MU_IER_REG) = (*pAUX_MU_IER_REG) | 0x2;  // Enable mini UART transmit interrupts
    flag = true;
  }
}

extern "C"
void sendString(char* buff) {
  char * i = buff;
  int r;
  if (*i == 0) {
    return;
  }
  while (*i != 0) {
    r = txtail - txhead;
    if (r > 0 && r < 3) {
      for (int j = 0; j < 1000; j++) {
        r = txtail - txhead;
        if (r > 0 && r > 20) {
          break;
        }
      }
    }
    txbuffer[txhead] = *(i++);
    txhead = (txhead + 1) & TXBUFMASK;
  }
  if(!flag) { // transmit is not active
    (*pAUX_MU_IO_REG) = (unsigned int) txbuffer[txtail];
    txtail = (txtail + 1) & TXBUFMASK;
    (*pAUX_MU_IER_REG) = (*pAUX_MU_IER_REG) | 0x2;  // Enable mini UART transmit interrupts
    flag = true;
  }
//  dump_debug_f_message("Exit sendString(). txtail=%d txhead=%d i=%d r=%d\n\r", txtail, txhead, i, r);
}
//------------------------------------------------------------------------
volatile unsigned int rxhead;
volatile unsigned int rxtail;
#define RXBUFMASK 0xFF
volatile unsigned char rxbuffer[RXBUFMASK+1];

unsigned char read_rx_buffer() {
  do {} while (rxtail == rxhead);
  unsigned char c = rxbuffer[rxtail];
  rxtail = (rxtail + 1) & RXBUFMASK;
  return c;
}
//------------------------------------------------------------------------

extern "C"
void uart_init() {
  (*pDISABLE_IRQ_1) = 0x20000000; // Disable AUX interrupts (29 bit)
  (*pAUX_ENABLES) = 1; // Enable mini UART

/**
  GPIO14  TXD0 and TXD1
  GPIO15  RXD0 and RXD1
**/
    Gpio gpio14(14), gpio15(15);
    gpio14.setFunction(2);
    gpio15.setFunction(2);
    gpio14.setPullUpDown(0);
    gpio15.setPullUpDown(0);

  (*pAUX_MU_IER_REG) = 0;  // Disable mini UART transmit/receive interrupts
  (*pAUX_MU_CNTL_REG) = 0; // Transmit & Receive disable
  (*pAUX_MU_LCR_REG) = 0x03;  // DLAB = 0, 8-bits mode
  (*pAUX_MU_MCR_REG) = 0x0;  // modem: RTS level is high
//     ((250,000,000/115200)/8)-1 = 270
//     ((250,000,000/9600)/8)-1 = 3254
  (*pAUX_MU_BAUD_REG) = 270;  // Set baud rate

  rxhead = rxtail = 0;
  txhead = txtail = 0;
  flag = false;

  (*pAUX_MU_IER_REG) = 0x1;  // Enable mini UART receive interrupts
  (*pAUX_MU_CNTL_REG) = 0x03; // Transmit & Receive enable
  (*pAUX_MU_IIR_REG) = 0x04; // Clear transmit FIFO
  (*pAUX_MU_IIR_REG) = 0x02; // Clear receive FIFO
  (*pENABLE_IRQ_1) = 0x20000000; // Enable AUX interrupts (29 bit)
}

extern "C"
void irq_handler_mini_uart() {
  volatile unsigned int interrupt_status;
//  EXIT_CRITICAL()              // unmask interrupts
  do {
    interrupt_status = *pAUX_MU_IIR_REG;
    if (interrupt_status & 0x1) break;
    if (interrupt_status & 0x4) {  // receiver holds a valid byte
      rxbuffer[rxhead] = (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
      rxhead = (rxhead + 1) & RXBUFMASK;
    }
    if (interrupt_status & 0x2) { // transmit holding register empty
      if (txhead != txtail ) {
        (*pAUX_MU_IO_REG) = txbuffer[txtail];
        txtail = (txtail + 1) & TXBUFMASK;
      } else {
        flag = false;
        (*pAUX_MU_IER_REG) = (*pAUX_MU_IER_REG) & (~0x2);  // Disable mini UART transmit interrupts
      }
    }
  } while ((interrupt_status & 0x1) == 0);
//  ENTER_CRITICAL()                     // mask interrupts
}

