/*
   Copyright (C) 2007-2014 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

MyAO::MyAO(DWORD prio, DWORD c ) : AObject(prio) {
  counter = 0;
  second = 10;
  lineIdx = 0;
  logMsg = new Message(0, 0, (BYTE *)outputString, logging);
}

DWORD
MyAO::processMessage(Message * e) {
  switch (e->getMessageID()) {
    case tick :              // Message from Timer
      if (--second == 0) {     // filter ticks to seconds
        second = 10;
        counter++;
        fp.format(outputString,
            "<1> Active object #%d count=%7d buffer=%3d\r\n",
            getPriority(), counter, incomingBufferLoad());
        putOutgoingMessage(logMsg);
      }
      switch (second % 2) {
        case 0:
          gpio22->clearLevel();
          break;
        case 1:
          gpio22->setLevel();
          break;
        default:
          break;
      }
      return 1;
      case command :
      {
        BYTE s = *(e->getString());
        if (s == '\n' || s == '\r') {
          receivedLine[lineIdx] = 0;
          lineIdx = 0;
          fp.format(outputString,
              "Command line %s\r\n", receivedLine);
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

void MyAO::log(BYTE level, char* text) {

}
