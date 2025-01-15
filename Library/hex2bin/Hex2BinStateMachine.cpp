/*
 * Hex2BinStateMachine.cpp
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

#include "Include/hex2bin.hpp"

/*---------------------- Finite state machine hex2bin -------------------------*/

/** Constructor. */
Hex2BinStateMachine::Hex2BinStateMachine() : Fsm((State)&Hex2BinStateMachine::initial) {
  phMemoryAddress = -1;
  TRANSITION(&Hex2BinStateMachine::initial);
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

void Hex2BinStateMachine::debug(char * t, char *s) {
  std::cout << ">>> " << t << "(" << *s
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
Hex2BinStateMachine::initial(char *s) {
  debug("initial", s);
  switch(*s) {
    case ':':
      TRANSITION(&Hex2BinStateMachine::dataSize);
/** >>> enter to data size state **/
      count = 0;
      sum = 0;
      recordAddress = 0;
      index = 0;
      value = 0;
/** <<< enter to data size state **/
      break;
    default :
      break;
  }
  return 0;
}

/****************************************
 * DataSize state.
 */
int
Hex2BinStateMachine::dataSize(char *s) {
  if (s) {
    debug("dataSize", s);
    value = convert(value, *s);
    switch (++count) {
      case 2:
        size = value;
        dataArray = new int[size];
        sum += value;
        value = 0;
        count = 0;
        TRANSITION(&Hex2BinStateMachine::address);
        break;
      default:
        break;
    }
  } else {
    TRANSITION(&Hex2BinStateMachine::initial);
  }
  return 0;
}

/****************************************
 * Address state.
 */
int
Hex2BinStateMachine::address(char *s) {
  if (s) {
    debug("address", s);
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
        value = 0;
        count = 0;
        TRANSITION(&Hex2BinStateMachine::recordType);
        break;
      default:
        break;
    }
  } else {
    TRANSITION(&Hex2BinStateMachine::initial);
  }
  return 0;
}

/****************************************
 * Record type state.
 */
int
Hex2BinStateMachine::recordType(char *s) {
  if (s) {
    debug("recType", s);
    value = convert(value, *s);
    switch (++count) {
      case 2:
        rType = value;
        sum += value;
        if (size > 0) {
          TRANSITION(&Hex2BinStateMachine::data);
          count = 0;
          value = 0;
          index = 0;
          size--;
        } else {
          TRANSITION(&Hex2BinStateMachine::checkSum);
        };
        break;
      default:
        break;
    }
  }
  return 0;
}

/****************************************
 * Data state.
 */
int
Hex2BinStateMachine::data(char *s) {
  if (s) {
    debug("data", s);
    value = convert(value, *s);
    switch (++count) {
      case 2:
        if (size == 0) {
          TRANSITION(&Hex2BinStateMachine::checkSum);
          dataArray[index++] = value;
          sum += value;
          count = 0;
          value = 0;
        } else {
          TRANSITION(&Hex2BinStateMachine::data);
          dataArray[index++] = value;
          sum += value;
          count = 0;
          value = 0;
          size--;
        };
        break;
      default:
        break;
    }
  }
  return 0;
}

/****************************************
 * Check Sum state.
 */
int
Hex2BinStateMachine::checkSum(char *s) {
  if (s) {
    debug("checkSum", s);
    value = convert(value, *s);
    switch (++count) {
      case 2:
        TRANSITION(&Hex2BinStateMachine::initial);
        cSum = value;
        processRecord();

        break;
      default:
        break;
    }
  }
  return 0;
}

void
Hex2BinStateMachine::processRecord() {
  char c = ' ';
  debug("processRecord", &c);
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
      break;
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

}
