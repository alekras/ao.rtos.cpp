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

#include "GpioAO.hpp"
#include "arm_debug_tools.hpp"

GpioAO::GpioAO(DWORD prio) : ISAObject(prio, 2) {
  initGpio();
  counter = 0;
  period = 20;
  status = 0;
  lastTimeStamp = 0;
  lastStatus = 0;
  outputString = new String(120);
  logMsg = new Message((DWORD_S)prio, (DWORD_S)1, (DWORD)0, MessageType::string, logging);
}

void
GpioAO::initGpio() {
  (*pDISABLE_IRQ_2) = 0x001e0000; // Disable GPIO interrupts
  gpio21 = new Gpio(21);
  gpio21->setAsInput();
  gpio21->setPullUpDown(2);
  gpio21->enableFallingEdgeDetect();
  gpio21->clearEventDetectStatus();
  (*pENABLE_IRQ_2) = 0x00020000; // Enable GPIO interrupts
}

AO_STACK *
GpioAO::serviceInterrupt(AO_STACK * stkp) {
  if (counter++ < 3) {
    lastStatus = (*pPENDING_IRQ_2);
//    fp1.format(out, "-- PendStatus=%8h CPU SP=%8h OS SP=%8h CPSR=%8h SPSR=%8h\r\n",
//      lastStatus, get_sp(), stkp, get_cpsr(), get_spsr());  // @debug
//    dump_debug_message(out);  // @debug
//    dump_lr(stkp);
//    dump_debug_message("-\r\n");
//  event = gpio21->readEventDetectStatus();
    tempTime = (*pSYS_TIMER_COUNT_LO);
    impulseWidth = tempTime - lastTimeStamp;
    lastTimeStamp = tempTime;
//  lastStatus = lastStatus | event;
//    unsigned int* debug_var[] = {(unsigned int*)&lastStatus, (unsigned int*)&counter, (unsigned int*)&stkp, 0};
//    intsToHex(out, debug_var);
//    dump_debug_message(out);  // @debug
//    status = 0;
  } else {
    (*pDISABLE_IRQ_2) = 0x001e0000; // Disable GPIO interrupts
  }
  gpio21->clearEventDetectStatus();
  return stkp;
}

DWORD
GpioAO::processMessage(Message * msg) {
  switch (msg->getMessageID()) {
    case tick :              // Message from Timer each 250 msec
      if (--period == 0) { // each 5 sec
        fp.format((char *)outputString->getChars(),
            "<GPIO> impulse=%d usec, counter=%d, last status=%8h now status=%8h.\r\n",
            impulseWidth, counter, lastStatus, (*pPENDING_IRQ_2));
        logMsg->setString(outputString);
        putOutgoingMessage(logMsg);
        period = 20;
        counter = 0;
        (*pENABLE_IRQ_2) = 0x00020000; // Enable GPIO interrupts
      }
      break;
    default :
      return 1;
  }
  return 1;
}

void
GpioAO::run() {
}

