/*
 * Hex2BinEFSMachine.cpp
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

#include "Include/hex2bin.hpp"

/*---------------------- Finite state machine hex2bin -------------------------*/

/** Constructor. */
Hex2BinEFSMachine::Hex2BinEFSMachine() : Efsm((State)&Hex2BinEFSMachine::initial) {
  phMemoryAddress = -1;
}

int
convert(int v, char d) {
  int l;
  if (d >= '0' && d <= '9') {
    l = d - '0';
  } else if (d >= 'A' && d <= 'F') {
    l = d - 'A' + 10;
  } else if (d >= 'a' && d <= 'f') {
    l = d - 'a' + 10;
  } else {
    l = 0;
  }
  return (v << 4) | l;
}

void Hex2BinEFSMachine::debug(char * t, Phase phase, char *s) {
  char event;
  if (s) {
    event = *s;
  } else {
    event = '_';
  }
  return;
  std::cout << ">>> " << t << "(" << phase << ", " << event
      << ")\t\t count: " << count
      << ", value: " << value
      << ", size: " << size
      << ", index: " << index
      << ", sum: " << sum
      << ";   rType: " << rType
      << ", cSum: " << cSum
      << ", recordAddress: " << recordAddress
      << ", linearAddress: " << linearAddress
      << "\n";
}
/****************************************
 * Initial state.
 */
int
Hex2BinEFSMachine::initial(Phase phase, char *s) {
  debug("initial", phase, s);
  switch(phase) {
    case ENTER:
      count = 0;
      sum = 0;
      recordAddress = 0;
      index = 0;
      value = 0;
      break;
    case EVENT:
      if (rType == EOH) return 1;
      switch (*s) {
        case ':':
          TRANSITION(&Hex2BinEFSMachine::dataSize);
          break;
        default :
          break;
      }
      break;
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * DataSize state.
 */
int
Hex2BinEFSMachine::dataSize(Phase phase, char *s) {
  debug("dataSize", phase, s);
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
      value = convert(value, *s);
      switch (++count) {
        case 2:
          size = value;
          sum += value;
          TRANSITION(&Hex2BinEFSMachine::address);
          break;
        default:
          break;
      }
      break;
    case EXIT:
      dataArray = new int[size];
      break;
  }
  return 0;
}

/****************************************
 * Address state.
 */
int
Hex2BinEFSMachine::address(Phase phase, char *s) {
  debug("address", phase, s);
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
      value = convert(value, *s);
      switch (++count) {
        case 2:
          recordAddress = value;
          sum += value;
          value = 0;
          break;
        case 4:
          recordAddress = (recordAddress << 8) | value;
          sum += value;
          TRANSITION(&Hex2BinEFSMachine::recordType);
          break;
        default:
          break;
      }
      break;
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Record type state.
 */
int
Hex2BinEFSMachine::recordType(Phase phase, char *s) {
  debug("recType", phase, s);
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
      value = convert(value, *s);
      switch (++count) {
        case 2:
          rType = value;
          sum += value;
          if (size > 0) {
            TRANSITION(&Hex2BinEFSMachine::data);
          } else {
            TRANSITION(&Hex2BinEFSMachine::checkSum);
          };
          break;
        default:
          break;
      }
      break;
    case EXIT:
      index = 0;
      break;
  }
  return 0;
}

/****************************************
 * Data state.
 */
int
Hex2BinEFSMachine::data(Phase phase, char *s) {
  debug("data", phase, s);
  switch(phase) {
    case ENTER:
      count = 0;
      value = 0;
      size--;
      break;
    case EVENT:
      value = convert(value, *s);
      switch (++count) {
        case 2:
          if (size == 0) {
            TRANSITION(&Hex2BinEFSMachine::checkSum);
          } else {
            TRANSITION(&Hex2BinEFSMachine::data);
          };
          break;
        default:
          break;
      }
      break;
    case EXIT:
      dataArray[index++] = value;
      sum += value;
      break;
  }
  return 0;
}

/****************************************
 * Check Sum state.
 */
int
Hex2BinEFSMachine::checkSum(Phase phase, char *s) {
  debug("checkSum", phase, s);
  switch(phase) {
    case ENTER:
      count = 0;
      value = 0;
      break;
    case EVENT:
      value = convert(value, *s);
      switch (++count) {
        case 2:
          TRANSITION(&Hex2BinEFSMachine::initial);
          break;
        default:
          break;
      }
      break;
    case EXIT:
      cSum = value;
       processRecord();
      break;
  }
  return 0;
}

int
Hex2BinEFSMachine::processRecord() {
  char c = ' ';
  debug("processRecord", (Phase)0, &c);
  int a, a1, ma;
  switch(rType) {
    case DATA:
      ma = (segmentAddress << 4) + recordAddress;
      if(phMemoryAddress < 0) {
        phMemoryAddress = ma;
      }

      while( ma > phMemoryAddress) {
        printf(" %02X", 0);
        phMemoryAddress++;
      }
      printf("<%04X:%04X>", segmentAddress, recordAddress);
      for( int i = 0; i < index; i++ ) {
        printf(" %02X", dataArray[i]);
        phMemoryAddress++;
      }
      printf("\n");
     break;
    case EOH:
      printf("end of file.\n");
      return 1;
    case SEGMENT_ADDRESS:
      segmentAddress = 256 * dataArray[0] + dataArray[1];
      printf("\nextended segment address:  %04X", segmentAddress);
      break;
    case 3:
      break;
    case LINEAR_ADDRESS:
      linearAddress = 256 * dataArray[0] + dataArray[1];
      printf("\nextended linear address:  %08X", linearAddress);
      break;
    case START_ADDRESS:
      a = 256 * dataArray[0] + dataArray[1];
      a1 = 256 * dataArray[2] + dataArray[3];
      printf("\nstarting address: %04X %04X", a, a1);
      break;
    default:
      break;
  }
  printf(" --- calculated summ: %02X, ", (1 + (~sum) & 0xFF));
  printf(" readed summ: %02X\n", cSum);
  if(( 1 + (~sum) & 0xFF ) != cSum)
    printf(" --- Wrong cs!\n");
  return 0;
}
