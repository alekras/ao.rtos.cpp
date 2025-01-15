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

#include "Include/DS18B20SM.hpp"

/*---------------------- Finite state machine IIL -------------------------*/

/** Constructor. */
DS18B20SM::DS18B20SM(DelayAO *aObj) : Efsm((State)&DS18B20SM::initial) {
  ao = aObj;
  pin = ao->gpio20;
  byteToWrite = 0;
  writeByteSubState = 0;
  writeBitSubState =0;
  dsCommands = new BYTE [4] {0xCC, 0x44, 0xCC, 0xBE};
}

/****************************************
 * Initial state.
 */
int
DS18B20SM::initial(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      writeByteSubState = 0;
      break;
    case EVENT:
      switch (s->getMessageID()) {
      case tick:
        TRANSITION(&DS18B20SM::reset);
        break;
      case io_in:
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
 * Reset impulse state.
 */
int
DS18B20SM::reset(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      pin->setAsOutput();
      pin->clearLevel();
      resetSubState = 0;
      ao->initSysTimer(50 * TEN_MICROSEC);
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          switch (resetSubState) {
            case 0 :
              pin->setLevel();
              pin->setAsInput();
              resetSubState++;
              ao->initSysTimer(50 * TEN_MICROSEC);
              break;
            case 1 :
              pin->setAsOutput();
              if (s->getWord() == 0) {
                writeByteSubState = 0;
                ao->logMessage((BYTE*)"<DS18B20> is present.\r\n");
                TRANSITION(&DS18B20SM::writeByte);
              } else {
                ao->logMessage((BYTE*)"<DS18B20> does not present.\r\n");
                TRANSITION(&DS18B20SM::initial);
              }
              break;
          }
          break;
        case tick :
          break;
        default :
          break;
      }
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Writing byte state.
 */
int
DS18B20SM::writeByte(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      byteToWrite = dsCommands[writeByteSubState];
      writeBitSubState = 0;
      pin->clearLevel();
      pin->setAsOutput();
      if ((byteToWrite & 0x01) == 1) {
        pin->setLevel();
      }
      ao->initSysTimer(6 * TEN_MICROSEC);
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          if (writeBitSubState == 7) {
            writeByteSubState++;
            TRANSITION(&DS18B20SM::writeByte);
          } else {
            pin->setLevel();
            byteToWrite = byteToWrite >> 1;
            writeBitSubState++;
            pin->clearLevel();
            pin->setAsOutput();
            if ((byteToWrite & 0x01) == 1) {
              pin->setLevel();
            }
            ao->initSysTimer(6 * TEN_MICROSEC);
          }
          break;
        case tick :
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
 * Format state.
 */
int
DS18B20SM::writeBit(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          break;
        case tick :
          break;
        default :
          break;
      }
      TRANSITION(&DS18B20SM::writeByte);
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Go state.
 */
int
DS18B20SM::wait(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          break;
        case tick :
          break;
        default :
          break;
      }
      TRANSITION(&DS18B20SM::writeByte);
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Load state.
 */
int
DS18B20SM::readByte(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          break;
        case tick :
          break;
        default :
          break;
      }
      TRANSITION(&DS18B20SM::writeByte);
    case EXIT:
      break;
  }
  return 0;
}

/****************************************
 * Help state.
 */
int
DS18B20SM::readBit(Phase phase, Message *s) {
  switch(phase) {
    case ENTER:
      break;
    case EVENT:
      switch (s->getMessageID()) {
        case io_in :
          break;
        case tick :
          break;
        default :
          break;
      }
      TRANSITION(&DS18B20SM::writeByte);
    case EXIT:
      break;
  }
  return 0;
}

