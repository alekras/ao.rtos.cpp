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
  counter = 0;
  second = 4;
  lineIdx = 0;
  outputString = new String(160);
  outputString1 = new String(160);
  logMsg = new Message(0, 1, 0, MessageType::string, logging);
  logMsg1 = new Message(0, 1, 0, MessageType::string, logging);
}

DWORD
MyAO::processMessage(Message * e) {
  switch (e->getMessageID()) {
    case tick :              // Message from Timer
      if (--second == 0) {     // filter ticks to seconds
        second = 240; // each 1 minutes
        counter++;

        fp.format((char *)outputString->getChars(),
            "<1> Active object #%d count=%d buffer=%d\r\n",
            getPriority(), counter, incomingBufferLoad());
        logMsg->setString(outputString);
        putOutgoingMessage(logMsg);

//        MemoryManager::MemoryStatistics stat;
//        mm->getStatistics(&stat);
//        fp.format((char *)outputString1->getChars(),
//            "<2> Memory statistics: from %8h to %8h available=%8h blocks=%3d allocated blocks=%3d\r\n",
//            stat.start, stat.end, stat.available, stat.blocks, stat.allocatedBlocks);
//        logMsg1->setString(outputString1);
//        putOutgoingMessage(logMsg1);

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
          break;
        case 1:
          gpio22->clearLevel();
          gpio10->setLevel();
          break;
        case 2:
          gpio22->setLevel();
          gpio10->clearLevel();
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
