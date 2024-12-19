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

#ifndef DEBUGAOSCHEDULER_HPP_
#define DEBUGAOSCHEDULER_HPP_

#include "../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm2835.hpp"
#include "AOScheduler.hpp"
#include "application.hpp"

class DebugAOScheduler : public AOScheduler {
 private:

  String *output, *intOutput;
  Message *logMsg, *intLogMsg;
  volatile int tickCounter;
  FormatParser fp, intFp;

 protected:
  virtual DWORD processMessage(Message*);
  virtual AO_STACK * serviceInterrupt(AO_STACK * stp);

 public:
          DebugAOScheduler();
  void logAOInfo( AObject * ao );

};

#endif /* DEBUGAOSCHEDULER_HPP_ */
