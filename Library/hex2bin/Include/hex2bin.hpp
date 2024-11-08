/*
 * hex2bin.hpp
 *
 */
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

#ifndef HEX2BIN_HPP_
#define HEX2BIN_HPP_

#include "../../FSM/Include/fsm.hpp"

#define  DATA 0
#define  EOH  1
#define  SEGMENT_ADDRESS 2
#define  LINEAR_ADDRESS 4
#define  START_ADDRESS 5

class Hex2BinStateMachine : public Fsm<char> {
  private:
// State parameters:
    int count;
    int value;
    int size;
    int index;

// processing results:
    int sum;
    int *dataArray;
    int recordAddress;
    int segmentAddress;
    int linearAddress;
    long phMemoryAddress;
    int rType;
    int cSum;

    int initial(char *);
//    int record(char *);
    int dataSize(char *);
    int address(char *);
    int recordType(char *);
    int data(char *);
    int checkSum(char *);
    void processRecord();
    void debug(char * t, char *s);
  public:
    Hex2BinStateMachine();
};

class Hex2BinEFSMachine : public Efsm<char> {
  private:
// State parameters:
    int count;
    int value;
    int size;
    int index;

// processing results:
    int sum;
    int *dataArray;
    int recordAddress;
    int segmentAddress;
    int linearAddress;
    long phMemoryAddress;
    int rType;
    int cSum;
// States implementation:
    int initial(Phase, char *);
//    int record(Phase, char *);
    int dataSize(Phase, char *);
    int address(Phase, char *);
    int recordType(Phase, char *);
    int data(Phase, char *);
    int checkSum(Phase, char *);
// Helper functions:
    int processRecord();
    void debug(char * t, Phase, char *s);
  public:
    Hex2BinEFSMachine();
};

#endif /* HEX2BIN_HPP_ */
