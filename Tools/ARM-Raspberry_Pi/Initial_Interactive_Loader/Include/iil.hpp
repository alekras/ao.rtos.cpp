/*
 * iil.hpp
 *
 */
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

#ifndef IIL_HPP_
#define IIL_HPP_

#include "../../../../Library/FSM/Include/fsm.hpp"
#include "../../../../Library/Display/Include/formatter.hpp"

#define DUMP_BUFFER 0x30000000
#define DUMP_BUFFER_END 0x3000F000

class InitialInteractiveLoaderSM : public Efsm<char> {
  private:
// State parameters:
    int count;
    int value;
    int size;
    int index;
    int width;
    int subFormat;

// processing results:
    int startAddress;
    int currentAddress;
    int *arguments;
    int bytes;
    int cSum;
    int substate;
// States implementation:
    int initial(Phase, char *);
    int display(Phase, char *);
    int outputDump(Phase, char *);
    int format(Phase, char *);
    int go(Phase, char *);
    int load(Phase, char *);
    int help(Phase, char *);
// Helper functions:
    void runCommand(int, int);
    void runOutputDump();
    void runHex2Bin();
    void debug();
  public:
    InitialInteractiveLoaderSM();
};

#endif /* IIL_HPP_ */
