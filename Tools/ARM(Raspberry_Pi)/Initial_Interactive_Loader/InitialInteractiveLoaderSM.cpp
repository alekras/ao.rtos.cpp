/*
 * InitialInteractiveLoaderSM.cpp
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

#include "Include/iil.hpp"
#include "Include/hex2bin.hpp"

extern "C" unsigned int uart_recv();
extern "C" unsigned char read_rx_buffer();
extern "C" unsigned int uart_send(char);
extern "C" void sendString(char*);
extern "C" void disable_irq();
extern "C" void GOTO(unsigned int);
extern "C" int convert(int, char);

/*---------------------- Finite state machine IIL -------------------------*/

/** Constructor. */
InitialInteractiveLoaderSM::InitialInteractiveLoaderSM() : Efsm((State)&InitialInteractiveLoaderSM::initial) {
  width = 1;
}

char*
int2hexString(unsigned int source, char* text, int length) {
  static char digits[] = {'0','1','2','3','4','5','6','7','8','9',
                          'A','B','C','D','E','F'};
  int i = length;
  while( i > 0 ) {
    *(text + (--i)) = digits[source & 0xF];
    source = source >> 4;
  }
  return text + length;
}

void InitialInteractiveLoaderSM::debug() {
}
/****************************************
 * Initial state.
 */
int
InitialInteractiveLoaderSM::initial(Phase phase, char *s) {
//  sendString("initial\n\r");
  switch(phase) {
    case ENTER:
      sendString(">");
      count = 0;
      value = 0;
      break;
    case EVENT:
      switch (*s) {
      case 'M': case 'm':
        TRANSITION(&InitialInteractiveLoaderSM::outputDump);
        break;
      case 'D': case 'd':
        TRANSITION(&InitialInteractiveLoaderSM::display);
        break;
        case 'W': case 'w':
          width = 2;
          TRANSITION(&InitialInteractiveLoaderSM::format);
          break;
        case 'B': case 'b':
          width = 1;
          TRANSITION(&InitialInteractiveLoaderSM::format);
          break;
        case 'G': case 'g':
          TRANSITION(&InitialInteractiveLoaderSM::go);
          break;
        case 'L': case 'l':
          TRANSITION(&InitialInteractiveLoaderSM::load);
          break;
        case 'H': case 'h':
          TRANSITION(&InitialInteractiveLoaderSM::help);
          break;
        case '\n': case '\r':
          sendString("\n\r");
          TRANSITION(&InitialInteractiveLoaderSM::initial);
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
 * Display state.
 */
int
InitialInteractiveLoaderSM::display(Phase phase, char *s) {
//  sendString("display\n\r");
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      substate = 0;
      break;
    case EVENT:
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        case ','  :
          substate++;
          currentAddress = value;
          value = 0;
          break;
        default :
          char d = *s;
          if ((d >= '0' && d <= '9') || (d >= 'A' && d <= 'F') || (d >= 'a' && d <= 'f')) {
            value = convert(value, d);
          } else {
            debug(); // TODO process error
          }
          break;
      }
      if (count >= 3) {
        if (substate == 0) {
          if (value > 0) {
            currentAddress = value;
          }
        } else if (substate == 1) {
          if (value == 0) {
            bytes = 16;
          } else {
            bytes = value;
          }
        } else {
          debug(); // TODO process error
        }
        sendString("\n\r");
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      runCommand(currentAddress, bytes);
      currentAddress += bytes;
      break;
  }
  return 0;
}

/****************************************
 * Output debug dump string state.
 */
int
InitialInteractiveLoaderSM::outputDump(Phase phase, char *s) {
  switch(phase) {
    case ENTER:
      count = 0;
      break;
    case EVENT:
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        default :
          break;
      }
      if (count >= 3) {
        sendString("\n\r");
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      runOutputDump();
      break;
  }
  return 0;
}

#define DUMP_BUFFER 0x30000000
#define DUMP_BUFFER_END 0x3000F000

void
InitialInteractiveLoaderSM::runOutputDump() {
  char buff[120], *dump_pointer;
  unsigned int * dump_pointer_storage;
  int i;

  dump_pointer_storage = (unsigned int *)DUMP_BUFFER;
  dump_pointer = (char *)(DUMP_BUFFER + 4);
  while ((dump_pointer < (char*)DUMP_BUFFER_END) && (dump_pointer < (char*)(*dump_pointer_storage))) {
    i = 0;
    do {
      buff[i++] = *dump_pointer;
    } while (*(dump_pointer++) != 0);
//    while (*dump_pointer != 0) {
//      buff[i++] = *(dump_pointer++);
//    }
//    buff[i] = *(dump_pointer++);
    sendString(buff);
  }
}

/****************************************
 * Format state.
 */
int
InitialInteractiveLoaderSM::format(Phase phase, char *s) {
//  sendString("format\n\r");
  switch(phase) {
    case ENTER:
      count = 0;
      break;
    case EVENT:
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        case 'W': case 'w' :
          width = 4;
          break;
        default :
          break;
      }
      if (count >= 3) {
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Go state.
 */
int
InitialInteractiveLoaderSM::go(Phase phase, char *s) {
//  sendString("Go\n\r");
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        default :
          char d = *s;
          if ((d >= '0' && d <= '9') || (d >= 'A' && d <= 'F') || (d >= 'a' && d <= 'f')) {
            value = convert(value, d);
          } else {
            debug(); // TODO: process error
          }
          break;
      }
      if (count >= 3) {
        sendString("\n\r");
        startAddress = value;
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      disable_irq();
      GOTO(startAddress); // TODO assembler code for goto
      break;
  }
  return 0;
}

void
InitialInteractiveLoaderSM::runCommand(int address, int count) {
  char buff[80], *ptr;
  volatile unsigned int* memory = (unsigned int*) address;
  while (count > 0) {
    int bytesInLine = 0;
    ptr = int2hexString(address, buff, 8);
    *(ptr++) = '>';
    while ((bytesInLine < 16) && (count > 0)) {
      int bytesInCode = 4;
      unsigned int code = *(memory++);
      while (bytesInCode > 0) {
        bytesInCode -= width;
        *(ptr++) = ' ';
        ptr = int2hexString(code, ptr, 2 * width);
        code >>= (8 * width);
        count -= width;
        bytesInLine += width;
        if (count <= 0) break;
      }
    }
    *(ptr++) = '\r';
    *(ptr++) = '\n';
    *(ptr++) = 0;
    address += 16;
    sendString(buff);
  }
}

/****************************************
 * Load state.
 */
int
InitialInteractiveLoaderSM::load(Phase phase, char *s) {
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
//      sendString("=>Load\n\r");
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        default :
          break;
      }
      if (count >= 3) {
        sendString("\n\r");
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      runHex2Bin();
      break;
  }
  return 0;
}

/****************************************
 * Help state.
 */
int
InitialInteractiveLoaderSM::help(Phase phase, char *s) {
  switch(phase) {
    case ENTER:
      value = 0;
      count = 0;
      break;
    case EVENT:
//      sendString("=>Load\n\r");
      switch (*s) {
        case '\n' :
          count += 3;
          break;
        case '\r' :
          count += 5;
          break;
        default :
          break;
      }
      if (count >= 3) {
        sendString("\n\r");
        TRANSITION(&InitialInteractiveLoaderSM::initial);
      }
      break;
    case EXIT:
      sendString("\n\rInitial Interactive Loader\n\r");
      sendString("Commands:\n\r");
      sendString(" D<aaaa>,<cccc> - output memory content from <aaaa> to <aaaa> + <cccc>.\n\r");
      sendString(" D              - continue output memory content.\n\r");
      sendString(" M              - output debug dump content in text format.\n\r");
      sendString(" B              - set output memory format in bytes.\n\r");
      sendString(" W              - set output memory format in words (2 bytes).\n\r");
      sendString(" WW             - set output memory format in double words (4 bytes).\n\r");
      sendString(" L              - load hex file.\n\r");
      sendString(" H              - output help message.\n\r");
      sendString(" G<aaaa>        - run code at <aaaa> address.\n\r");
      break;
  }
  return 0;
}

void
InitialInteractiveLoaderSM::runHex2Bin() {
  char out[80];
//  FormatParser fp;

//  sendString("->runHex2Bin\n\r");
  unsigned char c;
  unsigned char* memPtr = (unsigned char*) INPUT_HEX_BUFFER;
  int i = 0;
  unsigned char* pattern = (unsigned char*) ":00000001FF\r\n";

  do {
    c = read_rx_buffer(); //uart_recv();
    *(memPtr++) = c;
    if (c == *(pattern + i)) {
      i++;
    } else {
      i = 0;
    }
    if (0 == *(pattern + i)) {
      break;
    }
  } while (1);

//  memPtr = (unsigned char*) 0x100000;
//  i = 0;
//  do {
//    c = *(memPtr++);
//    uart_send(c);
//    fp.format(out, "=%c i:%d patt:%c\n\r", c, i, *(pattern + i));
//    sendString(out);
//    if (c == *(pattern + i)) {
//      i++;
//    } else {
//      i = 0;
//    }
//    if (0 == *(pattern + i)) {
//      break;
//    }
//  } while (1);

  Hex2BinEFSMachine fsm;
  memPtr = (unsigned char*) 0x100000;
  do {
    if (fsm.dispatch((char*) (memPtr++)) > 0 ) {
      break;
    }
  } while (1);
}
