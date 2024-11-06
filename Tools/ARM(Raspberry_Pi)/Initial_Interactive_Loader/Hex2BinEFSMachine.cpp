/*
 * Hex2BinEFSMachine.cpp
 *
 */
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

#include "Include/hex2bin.hpp"
extern "C" void sendString(char*);
extern "C" int convert(int, char);

/*---------------------- Finite state machine hex2bin -------------------------*/

/** Constructor. */
Hex2BinEFSMachine::Hex2BinEFSMachine() : Efsm((State)&Hex2BinEFSMachine::initial) {
  phMemoryAddress = (unsigned char*) 0;
  addressType = 0;
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
//  char out[80];
//  FormatParser fp;
//  fp.format(out, ">>> %s:%d input:%c\n\r", t, phase, ((s==0)? ' ': *s));
//  sendString(out);
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
//      dataArray = new int[size];
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
      return processRecord();
  }
  return 0;
}

int
Hex2BinEFSMachine::processRecord() {
  char out[80];

  int a, a1, ma;
  switch(rType) {
    case DATA:
      if (addressType == 1) {
        phMemoryAddress = (unsigned char*) (linearAddress + recordAddress);
      } else if (addressType == 2) {
        phMemoryAddress = (unsigned char*) (segmentAddress + recordAddress);
      } else {
        phMemoryAddress = (unsigned char*) (recordAddress);
      }

//      fp.format(out, "<%8h>", phMemoryAddress);
//      sendString(out);
      for( int i = 0; i < index; i++ ) {
//        fp.format(out, " %2h", dataArray[i]);
//        sendString(out);
        *(phMemoryAddress++) = dataArray[i];
      }
//      sendString("\n\r");
     break;
    case EOH:
      sendString("end of file.\n\r");
      return 1;
    case SEGMENT_ADDRESS:
      addressType = 2;
      segmentAddress = (256 * dataArray[0] + dataArray[1]) << 4;
      fp.format(out, "extended segment address: %8h (or start address)\n\r", segmentAddress);
      sendString(out);
      break;
    case LINEAR_ADDRESS:
      addressType = 1;
      linearAddress = 256 * dataArray[0] + dataArray[1];
      linearAddress <<= 16;
      fp.format(out, "extended linear address: %8h\n\r", linearAddress);
      sendString(out);
      break;
    case START_ADDRESS: case START_ADDRESS_EX:
      a = 256 * dataArray[0] + dataArray[1];
      a <<= 16;
      a = a + 256 * dataArray[2] + dataArray[3];
//      fp.format(out, "starting address: %8h\n\r", a);
//      sendString(out);
      break;
    default:
      break;
  }
//  fp.format(out, "--- calculated sum: %2h\n\r    read sum: %2h\n\r", (1 + (~sum) & 0xFF), cSum);
//  sendString(out);
  if(( 1 + (~sum) & 0xFF ) != cSum) {
    sendString(" --- Wrong check sum!\n\r");
  }
  return 0;
}
