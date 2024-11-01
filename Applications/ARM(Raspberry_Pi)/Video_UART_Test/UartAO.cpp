/*
   Copyright (C) 2007-2024 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#include "./Include/UartAO.hpp"

UartAO::UartAO(DWORD prio) : ISAObject( prio, 1 ) {
  initUart();
  logMsg = new Message(0, 0, (BYTE *)outputString, logging);
  receivedSymbol[0] = 0;
  receivedSymbol[1] = 0;
  out = new Message(0, 0, (BYTE *)receivedSymbol, command);
}

void
UartAO::initUart() {
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

AO_STACK *
UartAO::serviceInterrupt(AO_STACK * stkp) {
  unsigned int status;
  do {
    status = *pAUX_MU_IIR_REG;
    if (status & 0x1) break;
    if (status & 0x4) { // receiver holds a valid byte
      rxbuffer[rxhead] = (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
      rxhead = (rxhead + 1) & RXBUFMASK;
      ready = 1;
    }
    if (status & 0x2) { // transmit holding register empty
      if (txhead != txtail ) {
        (*pAUX_MU_IO_REG) = txbuffer[txtail];
        txtail = (txtail + 1) & TXBUFMASK;
        ready = 1;
      } else {
        flag = false;
        (*pAUX_MU_IER_REG) = 0x5;  // Disable mini UART transmit interrupts
      }
    }
  } while ((status & 0x1) == 0);
	return stkp;
}

void
UartAO::run() {
  while (rxtail != rxhead) {
    receivedSymbol[0] = rxbuffer[rxtail];
    rxtail = (rxtail + 1) & RXBUFMASK;
    putOutgoingMessage(out);
    processMessage( out ); // make echo
  }
}

DWORD
UartAO::processMessage(Message * msg) {
  switch (msg->getMessageID()) {
    case command :
    case logging :
      {
        BYTE *string = msg->getString();
        while (*string != 0) {
          txbuffer[txhead] = *(string++);
          txhead = (txhead + 1) & TXBUFMASK;
        }
        if(!flag) { // transmit is not active
          (*pAUX_MU_IO_REG) = (unsigned int) txbuffer[txtail];
          txtail = (txtail + 1) & TXBUFMASK;
          (*pAUX_MU_IER_REG) = 0x7;  // Enable mini UART receive & transmit interrupts
          flag = true;
        }
      }
      break;
    default :
      return 1;
  }
  return 1;
}
