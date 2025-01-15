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

#include "Include/DelayAO.hpp"
#include "Include/DS18B20SM.hpp"

extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug
extern "C" unsigned int * get_sp(void); // @debug

DelayAO::DelayAO(DWORD prio) : ISAObject(prio, 3) {
  initLed();
//  initSysTimer(1000000);
  dsSM = new DS18B20SM(this);
  period = 20;
  outputString = new String(20);
  logMsg = new Message((DWORD_S)prio, (DWORD_S)1, (DWORD)0, MessageType::string, logging);
  eventMsg = new Message((DWORD_S)prio, (DWORD_S)prio, (DWORD)0, MessageType::word, io_in);
}

void
DelayAO::initLed() {
  gpio20 = new Gpio(20);
  gpio20->setAsOutput();
  gpio20->setPullUpDown(2);
  gpio20->setLevel();
}

void
DelayAO::initSysTimer(DWORD microSeconds) {
// Counter is running on freq = 1MHz and 1,000,000 cycles = 0xF4240
  (*pSYS_TIMER_CMP_1) = (*pSYS_TIMER_COUNT_LO) + microSeconds;
  (*pSYS_TIMER_CNTRL_STAT) = 2; // Clear CMP1 detected
  (*pENABLE_IRQ_1) = 0x00000002; // Enable system timer interrupts
}

void
DelayAO::logMessage(BYTE *msg) {
  outputString->setChars(msg);
  logMsg->setString(outputString);
  putOutgoingMessage(logMsg);
}

AO_STACK *
DelayAO::serviceInterrupt(AO_STACK * stkp) {
  (*pDISABLE_IRQ_1) = 0x00000002; // Disable system timer interrupts
  eventMsg->setWord(gpio20->readLevel());
  putIncomingMessage(eventMsg);
  return stkp;
}

DWORD
DelayAO::processMessage(Message * msg) {
  switch (msg->getMessageID()) {
    case tick :              // Message from Timer
      if (--period == 0) {
        period= 60; // 15 sec
        dsSM->dispatch(msg);
      }
      break;
    case io_in:
      dsSM->dispatch(msg);
      break;
    default :
      return 1;
  }
  return 1;
}

void
DelayAO::run() {
}

