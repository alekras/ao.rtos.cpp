/*
   Copyright (C) 2007-2025 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#include "UartAO.hpp"

// extern char out[200]; // @debug
// extern FormatParser fp1; // @debug
// extern "C" void dump_debug_message(char *); //@debug

UartAO::UartAO(DWORD prio) : ISAObject( prio, 1 ) {
  initUart();
  txRingBuffer = new RingBuffer<BYTE>(0xFF);
  rxRingBuffer = new RingBuffer<BYTE>(0xFF);
  comMsg = new Message((DWORD_S)prio, (DWORD_S)4, (DWORD)0, MessageType::onechar, command);
//  outMsg = new Message((DWORD_S)prio, (DWORD_S)prio, (DWORD)0, MessageType::binary, io_out);
  inMsg = new Message((DWORD_S)prio, (DWORD_S)prio, (DWORD)0, MessageType::onechar, io_in);
}

void
UartAO::initUart() {
  (*pDISABLE_IRQ_1) = 0x20000000; // Disable AUX interrupts (29 bit)
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
  flag = false;
  (*pAUX_MU_CNTL_REG) = 3; // Transmit & Receive enable
  (*pAUX_MU_IER_REG) = 0x5;  // Enable mini UART receive interrupts
  (*pENABLE_IRQ_1) = 0x20000000; // Enable AUX interrupts (29 bit)
}

AO_STACK *
UartAO::serviceInterrupt(AO_STACK * stkp) {
//  fp1.format(out, " >> UartAO::srvIntrr: stack=%h prio=%d\r\n", stkp, getPriority());  // @debug
//  dump_debug_message(out);  // @debug
  DWORD status;
  char c;
  do {
    status = *pAUX_MU_IIR_REG;
    if (status & 0x1) break;
    if (status & 0x4) { // receiver holds a valid byte
      inMsg->setChar(*pAUX_MU_IO_REG); // read from receiver
//      putOutgoingMessage(inMsg);
      putIncomingMessage(inMsg);
    }
    if (status & 0x2) { // transmit holding register empty
      if (txRingBuffer->isEmpty()) {
        flag = false;
        (*pAUX_MU_IER_REG) = 0x5;  // Disable mini UART transmit interrupts
      } else {
        txRingBuffer->get((BYTE*)pAUX_MU_IO_REG); // send to transmitter
        if (!flag) {
          (*pAUX_MU_IER_REG) = 0x7;  // Enable mini UART receive & transmit interrupts
          flag = true;
        }
//        putIncomingMessage(outMsg);
//        putOutgoingMessage(outMsg);
//        ready = 1;
      }
    }
  } while ((status & 0x1) == 0);
//  fp1.format(out, " << UartAO::srvIntrr: stack=%h prio=%d\r\n", stkp, getPriority());  // @debug
//  dump_debug_message(out);  // @debug
	return stkp;
}

DWORD
UartAO::processMessage(Message * msg) {
  switch (msg->getMessageID()) {
    case io_in :
      {
        BYTE c = msg->getChar();
        if(!flag) { // transmit is not active
          (*pAUX_MU_IO_REG) = c;
          (*pAUX_MU_IER_REG) = 0x7;  // Enable mini UART receive & transmit interrupts
          flag = true;
        } else {
          txRingBuffer->put(&c);
        }

        comMsg->setChar(c);
        putOutgoingMessage(comMsg);
      }
      break;
    case command :
    case logging :
      {
        String *string = msg->getString();
        DWORD l = string->length();
        DWORD available = txRingBuffer->bufferVacancy();
//        fp1.format(out, "> UartAO::processMessage l=%d avail=%d load=%d\n\r", l, available, txRingBuffer->bufferLoad()); //@debug
//        dump_debug_message(out); //@debug
        if (available < l) {
          return 0;
        }
        BYTE *stringPointer = string->getChars();
//        ENTER_CRITICAL()                     // mask interrupts
        while (*stringPointer != 0) {
          txRingBuffer->put(stringPointer++);
        }
//        EXIT_CRITICAL()              // ??? unmask interrupts
        if(!flag) { // transmit is not active
          txRingBuffer->get((BYTE*)pAUX_MU_IO_REG);
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
