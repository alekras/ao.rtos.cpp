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

#include "ThermometerAO.hpp"

ThermometerAO::ThermometerAO(DWORD prio) : ISAObject(prio, 3) {
  initLed();
  period = 20;
  outputString = new String(80);
  logMsg = new Message((DWORD_S)prio, (DWORD_S)1, (DWORD)0, MessageType::string, logging);
  eventMsg = new Message((DWORD_S)prio, (DWORD_S)prio, (DWORD)0, MessageType::word, io_in);
}

void
ThermometerAO::initLed() {
  gpio16 = new Gpio(16);
  gpio16->setAsInput();
  gpio16->setPullUpDown(0);

  gpio20 = new Gpio(20);
  gpio20->setAsOutput();
  gpio20->setPullUpDown(2);
  gpio20->setLevel();
}

void
ThermometerAO::initSysTimer(DWORD microSeconds) {
// Counter is running on freq = 1MHz and 1,000,000 cycles = 0xF4240
  (*pSYS_TIMER_CNTRL_STAT) = 2; // Clear CMP1 detected
  (*pSYS_TIMER_CMP_1) = (*pSYS_TIMER_COUNT_LO) + microSeconds;
  (*pENABLE_IRQ_1) = 0x00000002; // Enable system timer interrupts
}

void
ThermometerAO::wait_us(DWORD usec) {
  volatile DWORD temp = (*pSYS_TIMER_COUNT_LO);
  while(usec > (*pSYS_TIMER_COUNT_LO) - temp);
}

void
ThermometerAO::reset() {
  gpio16->setLevel();
  gpio16->setAsOutput();
  gpio16->clearLevel();
  ENTER_CRITICAL()                     // disable interrupts
  wait_us(800);
  gpio16->setAsInput();
  wait_us(60);
  volatile DWORD p0 = gpio16->readLevel();
  wait_us(600);
  EXIT_CRITICAL();                     // allow interrupts
  if (p0 == 0) {
//    fp.format((char *)outputString->getChars(), "<DS18B20> is present. %d\n\r",p0);
//    logMsg->setString(outputString);
//    putOutgoingMessage(logMsg);
  } else {
    fp.format((char *)outputString->getChars(), "<DS18B20> does not present. %d\n\r", p0);
    logMsg->setString(outputString);
    putOutgoingMessage(logMsg);
  }
}

void
ThermometerAO::writeByte2DS(DWORD data) {
  gpio16->setLevel();
  gpio16->setAsOutput();
  for (int i = 0; i < 16; i++) {
    gpio16->clearLevel();
    ENTER_CRITICAL()                     // disable interrupts
    if (data & 0x01 == 1) {
      wait_us(5);
      gpio16->setLevel();
      wait_us(100);
    } else {
      wait_us(100);
      gpio16->setLevel();
    }
    EXIT_CRITICAL();                     // allow interrupts
    data = data >> 1;
  }
}

DWORD
ThermometerAO::read2BytesFromDS() {
  DWORD result = 0;
  for (int i = 0; i < 16; i++) {
    gpio16->setLevel();
    gpio16->setAsOutput();
    ENTER_CRITICAL()                     // disable interrupts
    gpio16->clearLevel();
    wait_us(1);
    gpio16->setAsInput();
    wait_us(5);
    DWORD l0 = gpio16->readLevel();
    wait_us(50);
    EXIT_CRITICAL();                     // allow interrupts
    if (l0 == 1) {
      result = result | 0x10000;
    }
    result = result >> 1;
  }
  return result;
}

AO_STACK *
ThermometerAO::serviceInterrupt(AO_STACK * stkp) {
  (*pDISABLE_IRQ_1) = 0x00000002; // Disable system timer interrupts
  eventMsg->setWord(gpio20->readLevel());
  putIncomingMessage(eventMsg);
  return stkp;
}

DWORD
ThermometerAO::processMessage(Message * msg) {
  DWORD temperature;
  switch (msg->getMessageID()) {
    case tick :              // Message from Timer
      if (--period == 0) {
        period= 60; // 15 sec

        reset();
        writeByte2DS(0x44CC);

        gpio20->clearLevel();
        initSysTimer(750000);
      }
      break;
    case io_in:              // Message from ISR
      gpio20->setLevel();

      reset();
      writeByte2DS(0xBECC);
      temperature = read2BytesFromDS();

      fp.format((char *)outputString->getChars(), "<DS18B20> temperature= %h, t=%d.%04d%cC\n\r",
          temperature, (temperature >> 4), (temperature & 0xF) * 625, (char)186);
      logMsg->setString(outputString);
      putOutgoingMessage(logMsg);
      break;
    default :
      return 1;
  }
  return 1;
}

void
ThermometerAO::run() {
}

