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

#include "DebugAOScheduler.hpp"
//extern char out[200]; // @debug
//extern FormatParser fp1; // @debug

DebugAOScheduler::DebugAOScheduler() : AOScheduler() {
  tickCounter = 20;
  output = new String(160);
  logMsg = new Message(priority, 1, 0, MessageType::string, logging);
}

void
DebugAOScheduler::logAOInfo( AObject * obj ) {
  fp.format((char *)output->getChars(),
      "    %3d     %1d   %3d     %3d     %8h\r\n",
      obj->getPriority(), obj->isReady(), obj->incomingBufferLoad(), obj->outgoingBufferLoad(), obj->getSP());
  logMsg->setString(output);
  putOutgoingMessage(logMsg);
}

AO_STACK *
DebugAOScheduler::serviceInterrupt(ISAObject * isAobj, AO_STACK * stkp) {
  if (tickCounter == 0) {
    tickCounter++;
    DWORD interr;
    if (isAobj == 0) {
      interr = this->getPriority();
    } else {
      interr = isAobj->getPriority();
    }
    fp.format((char *)output->getChars(),
        "<AOScheduler> curr prio=%d interrupt=%d, curr stack=%8h\r\n   -prio- -rdy- -in.ld- -out.ld- -stack-\r\n",
        getCurrentPriority(), interr, stkp);
    logMsg->setString(output);
    putOutgoingMessage(logMsg);

    iterateAObjects((void (AOScheduler::*)(AObject*))&DebugAOScheduler::logAOInfo);
  }

  return AOScheduler::serviceInterrupt(isAobj, stkp);
}

DWORD
DebugAOScheduler::processMessage(Message * e) {
  switch (e->getMessageID()) {
    case tick :              // Message from Timer
      if (++tickCounter == 40) {     // filter ticks to 10 seconds
        tickCounter = 0;

        MemoryManager::MemoryStatistics stat;
        mm->getStatistics(&stat);
        fp1.format((char *)output->getChars(),
            "<AOScheduler> Memory: %8h - %8h available=%8h blocks=%3d allocated blocks=%3d\r\n",
            stat.start, stat.end, stat.available, stat.blocks, stat.allocatedBlocks);
        logMsg->setString(output);
        putOutgoingMessage(logMsg);
      }
      return 1;
    default:
      return 1;
  }
}