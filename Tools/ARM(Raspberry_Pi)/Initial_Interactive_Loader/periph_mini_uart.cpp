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

#include "../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1

extern "C"
unsigned char uart_recv() {
  volatile unsigned int status;
  do {
//    status = *pAUX_MU_LSR_REG;
    status = *pAUX_MU_STAT_REG;
    if ((status & 0x10) != 0) {
      return '*';
    }
  } while ((status & 0x01) == 0);
  return (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
}
//------------------------------------------------------------------------

volatile bool flag;
volatile unsigned int txhead;
volatile unsigned int txtail;
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
    (*pAUX_MU_IER_REG) = 0x7;  // Enable mini UART receive & transmit interrupts
    flag = true;
  }
}

extern "C"
void sendString(char* buff) {
  char * i = buff;
  int r;
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
    (*pAUX_MU_IER_REG) = 0x7;  // Enable mini UART receive & transmit interrupts
    flag = true;
  }
}
//------------------------------------------------------------------------
volatile unsigned int rxhead;
volatile unsigned int rxtail;
#define RXBUFMASK 0xFF
volatile unsigned char rxbuffer[RXBUFMASK+1];

extern "C"
unsigned char read_rx_buffer() {
  do {} while (rxtail == rxhead);
  unsigned char c = rxbuffer[rxtail];
  rxtail = (rxtail + 1) & RXBUFMASK;
  return c;
}
//------------------------------------------------------------------------

extern "C"
void uart_init() {

  (*pDISABLE_IRQ_1) = (*pDISABLE_IRQ_1) | 0x20000000; // Disable AUX interrupts (29 bit)
  (*pAUX_ENABLES) = 1;
  (*pAUX_MU_IER_REG) = 0;  // Disable mini UART receive interrupts
  (*pAUX_MU_CNTL_REG) = 0; // Transmit & Receive disable
  (*pAUX_MU_LCR_REG) = 3;  // 8-bits mode
  (*pAUX_MU_MCR_REG) = 0;  // modem: RTS is high
  (*pAUX_MU_IIR_REG) = 0x06; // Clear trans/rec FIFO
//     ((250,000,000/115200)/8)-1 = 270
  (*pAUX_MU_BAUD_REG) = 270;  // Set baud rate
//     ((250,000,000/9600)/8)-1 = 3254
//  (*pAUX_MU_BAUD_REG) = 3254;
  Gpio gpio14(14), gpio15(15);
  gpio14.setFunction(2);
  gpio15.setFunction(2);
  gpio14.setPullUpDown(0);
  gpio15.setPullUpDown(0);
  rxhead = rxtail = 0;
  txhead = txtail = 0;
  flag = false;
  (*pAUX_MU_CNTL_REG) = 3; // Transmit & Receive enable
  (*pAUX_MU_IER_REG) = 0x5;  // Enable mini UART receive interrupts
  (*pENABLE_IRQ_1) = (*pENABLE_IRQ_1) | 0x20000000; // Enable AUX interrupts (29 bit)
}

extern "C"
void irq_handler_mini_uart() {
  unsigned int status;
//  EXIT_CRITICAL()              // unmask interrupts
  do {
    status = *pAUX_MU_IIR_REG;
    if (status & 0x1) break;
    if (status & 0x4) { // receiver holds a valid byte
      rxbuffer[rxhead] = (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
      rxhead = (rxhead + 1) & RXBUFMASK;
    }
    if (status & 0x2) { // transmit holding register empty
      if (txhead != txtail ) {
        (*pAUX_MU_IO_REG) = txbuffer[txtail];
        txtail = (txtail + 1) & TXBUFMASK;
      } else {
        flag = false;
        (*pAUX_MU_IER_REG) = 0x5;  // Disable mini UART transmit interrupts
      }
    }
  } while ((status & 0x1) == 0);
//  ENTER_CRITICAL()                     // mask interrupts
}

