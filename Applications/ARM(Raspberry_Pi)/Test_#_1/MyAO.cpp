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

#include "MyAO.hpp"
extern MemoryManager* mm;
extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug

MyAO::MyAO(DWORD prio, DWORD c ) : AObject(prio) {
  gpio10 = new Gpio(10);
  gpio10->setFunction(1);
  gpio10->setLevel();
  gpio22 = new Gpio(22);
  gpio22->setFunction(1);
  gpio22->setLevel();
  gpio47 = new Gpio(47);
  gpio47->setFunction(1);
  gpio47->setLevel();
  counter = 0;
  second = 4;
  lineIdx = 0;
  outputString = new String(160);
  outputString1 = new String(160);
  logMsg = new Message(prio, 1, 0, MessageType::string, logging);
  commandMsg = new Message(prio, 31, 0, MessageType::word, command);
}

DWORD
MyAO::processMessage(Message * e) {
  switch (e->getMessageID()) {
    case tick :              // Message from Timer
      if (--second == 0) {     // filter ticks to seconds
        second = 240; // each 1 minutes
        counter++;
/* Sys timer has freq = 1MGz, so sys registers hi/lo contains amount of microseconds.
 * To get minutes we have to divide hi/lo registers by (10^6 * 60).
 * 10^6 * 60 = 10^7 * 2 * 3 = 2^8 * 5^7 * 3 = 2^8 * 234375.
 * So we need to shift right both registers in 8 bits and divide by 234375
 * Max Hours ~ 305
 */
        DWORD timerLo = (*pSYS_TIMER_COUNT_LO);
        DWORD timerHi = (*pSYS_TIMER_COUNT_HI);
        DWORD timeUSec = timerLo % 1000;
        DWORD timeMSec = timerLo / 1000;
        timeMSec = timeMSec % 1000;
        DWORD timeSec = timeMSec / 1000;
        timeSec = timeSec % 60;
        DWORD timeMin = (((timerHi << 24) & 0xff000000) | ((timerLo >> 8) & 0x00ffffff)) / 234375;
        DWORD timeHour = timeMin / 60;
        timeMin = timeMin % 60;

        fp.format((char *)outputString->getChars(),
            "<1> T: [%4d:%02d:%02d %3d.%03d] Active object[%d] count=%d buffer=%d\r\n",
            timeHour, timeMin, timeSec, timeMSec, timeUSec, getPriority(), counter, incomingBufferLoad());
        logMsg->setString(outputString);
        putOutgoingMessage(logMsg);

//        Binary *bin = new Binary(3);
//        DWORD *array = bin->getData();
//        array[0] = 77;
//        array[1] = 78;
//        array[2] = 79;
//        Message *binMsg = new Message(priority, priority, bin, binmsg);
//        fp1.format(out, "> new Message binMsg=%h\n\r", binMsg); //@debug
//        dump_debug_message(out); //@debug
//        putOutgoingMessage(binMsg);
//        delete binMsg;
      }
      switch (second % 4) {
        case 0:
          gpio22->clearLevel();
          gpio10->clearLevel();
          gpio47->clearLevel();
          break;
        case 1:
          gpio22->clearLevel();
          gpio10->setLevel();
          break;
        case 2:
          gpio22->setLevel();
          gpio10->clearLevel();
          gpio47->setLevel();
          break;
        case 3:
          gpio22->setLevel();
          gpio10->setLevel();
          break;
        default:
          break;
      }
      return 1;
    case command :
      {
        BYTE s = e->getChar();
        if (s == '\n' || s == '\r') {
          receivedLine[lineIdx] = 0;
          lineIdx = 0;
          fp.format((char *)outputString->getChars(),
              "Command line %s\r\n", receivedLine);
          logMsg->setString(outputString);
          putOutgoingMessage(logMsg);
          if (receivedLine[0] == 'd') {
            putOutgoingMessage(commandMsg);
          }
        } else {
          receivedLine[lineIdx++] = s;
        }
      }
      return 1;
    default:
      return 1;
  }
}

void
MyAO::run() {
//  fp1.format(out, " inside run() : obj=%h, stack=%h[real=%h] prio=%d ready=%d\r\n", this, getSP(), get_sp(), getPriority(), isReady());  // @debug
//  dump_debug_message(out);  // @debug
}

void MyAO::log(BYTE level, char* text) {
}
