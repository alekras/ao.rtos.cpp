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
extern DWORD stringLength(BYTE *);

MyAO::MyAO(DWORD prio, DWORD c ) : AObject(prio) {
  counter = 0;
  second = 5;
  lineIdx = 0;
  logMsg = new Message(0, 1, 0, MessageType::string, logging);
  logMsg1 = new Message(0, 1, 0, MessageType::string, logging);
  logMsg2 = new Message(0, 1, 0, MessageType::string, logging);
}

DWORD
MyAO::processMessage(Message * e) {
  switch (e->getMessageID()) {
    case tick :              // Message from Timer
      if (--second == 0) {     // filter ticks to seconds
        second = 50;
        counter++;
        fp.format((char *)outputString,
            "<1> Active object #%d count=%d buffer=%d\r\n",
            getPriority(), counter, incomingBufferLoad());
        String *os = new String(outputString);
        logMsg->setString(os);
        putOutgoingMessage(logMsg);
        delete os;

        MemoryManager::MemoryStatistics stat;
        mm->getStatistics(&stat);
        fp.format((char *)outputString1,
            "<2> Memory statistics: from %8h to %8h available=%8h blocks=%3d allocated blocks=%3d\r\n",
            stat.start, stat.end, stat.available, stat.blocks, stat.allocatedBlocks);
        os = new String(outputString1);
        logMsg1->setString(os);
        putOutgoingMessage(logMsg1);
        delete os;
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
        BYTE s = e->getBinaryData();
        if (s == '\n' || s == '\r') {
          receivedLine[lineIdx] = 0;
          lineIdx = 0;
          fp.format((char *)outputString,
              "Command line %s\r\n", receivedLine);
          String *os = new String(outputString);
          logMsg->setString(os);
          putOutgoingMessage(logMsg);
          delete os;
        } else {
          receivedLine[lineIdx++] = s;
        }
      }
      return 1;
    default:
      return 1;
  }
}

void MyAO::log(BYTE level, char* text) {
}
