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

#ifndef DS18B20_HPP_
#define DS18B20_HPP_

#include "../../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "../../../../Library/FSM/Include/fsm.hpp"
#include "../../../../Library/Display/Include/formatter.hpp"
#include "DelayAO.hpp"

#define TEN_MICROSEC 10000 // 10

class DS18B20SM : public Efsm<Message> {
  private:
// State parameters:
    BYTE *dsCommands;
    BYTE byteToWrite;
    DWORD resetSubState;
    DWORD writeByteSubState; // associated with DS commands
    DWORD writeBitSubState;  // associated with number of a bit in a byte.
    DelayAO *ao; // active object accepts a sys timer interrupts
    Gpio *pin;

// processing results:

// States implementation:
    int initial(Phase, Message *);
    int reset(Phase, Message *);
    int writeByte(Phase, Message *);
    int writeBit(Phase, Message *);
    int wait(Phase, Message *);
    int readByte(Phase, Message *);
    int readBit(Phase, Message *);
// Helper functions:
  public:
    DS18B20SM(DelayAO *ao);
};

#endif /* DS18B20_HPP_ */
