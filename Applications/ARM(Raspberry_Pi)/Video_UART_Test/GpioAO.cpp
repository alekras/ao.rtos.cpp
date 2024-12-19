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

#include "GpioAO.hpp"

extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug
extern "C" unsigned int * get_sp(void); // @debug

GpioAO::GpioAO(DWORD prio) : ISAObject(prio, 2) {
  initGpio();
  counter = 0;
  period = 20;
  outputString = new String(120);
  logMsg = new Message((DWORD_S)prio, (DWORD_S)1, (DWORD)0, MessageType::string, logging);
  inMsg = new Message((DWORD_S)prio, (DWORD_S)prio, (DWORD)0, MessageType::word, io_in);
}

void
GpioAO::initGpio() {
  (*pDISABLE_IRQ_2) = (*pDISABLE_IRQ_2) | 0x001e0000; // Disable GPIO interrupts
  gpio21 = new Gpio(21);
  gpio21->setFunction(0);
  gpio21->setPullUpDown(0);
  gpio21->enableRisingEdgeDetect();
  (*pENABLE_IRQ_2) = (*pENABLE_IRQ_2) | 0x001e0000; // Enable GPIO interrupts
}

AO_STACK *
GpioAO::serviceInterrupt(AO_STACK * stkp) {
//  fp1.format(out, " >> GpioAO::srvIntrr: ISAObj=%h stack=%h prio=%d\r\n", o, stkp, getPriority());  // @debug
//  dump_debug_message(out);  // @debug
  DWORD status = (*pPENDING_IRQ_2);
  DWORD event = gpio21->readEventDetectStatus();
  gpio21->clearEventDetectStatus();
  DWORD temp = (*pSYS_TIMER_COUNT_LO);
  impulseWidth = temp - lastTimeStamp;
  lastTimeStamp = temp;
  lastStatus = status | event;
  counter++;
//  inMsg->setWord(status | event);
//  putIncomingMessage(inMsg);
//  fp1.format(out, " << GpioAO::srvIntrr: stack=%h prio=%d\r\n", stkp, getPriority());  // @debug
//  dump_debug_message(out);  // @debug
  return stkp;
}

DWORD
GpioAO::processMessage(Message * msg) {
  switch (msg->getMessageID()) {
    case io_in :
      {
        lastStatus = msg->getWord();
        counter++;
      }
      break;
    case tick :              // Message from Timer
      if (--period == 0) {
        DWORD status = (*pPENDING_IRQ_2);
        DWORD event = gpio21->readEventDetectStatus();
        fp.format((char *)outputString->getChars(),
            "<GPIO> impulse=%d counter=%d, status=%8h, cur. status=%8h, buff load=%d\r\n", impulseWidth, counter, lastStatus, status | event, incomingBufferLoad());
        logMsg->setString(outputString);
        putOutgoingMessage(logMsg);
        period = 20;
        counter = 0;
      }
      break;
    default :
      return 1;
  }
  return 1;
}

void
GpioAO::run() {
//  fp1.format(out, " inside run() : obj=%h, stack=%h[real=%h] prio=%d ready=%d\r\n", this, getSP(), get_sp(), getPriority(), isReady());  // @debug
//  dump_debug_message(out);  // @debug
//  if (period == 18) {
//    gpio21->clearEventDetectStatus();
//    (*pENABLE_IRQ_2) = (*pENABLE_IRQ_2) | 0x001e0000; // Enable GPIO interrupts
//  }
}

