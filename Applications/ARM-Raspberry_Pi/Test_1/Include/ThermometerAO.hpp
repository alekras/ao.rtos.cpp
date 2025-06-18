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

#ifndef THERMOMETERAO_HPP_
#define THERMOMETERAO_HPP_

#include "../../../Porting/ARM-Raspberry_Pi/Include/os_cpu.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"
#include "gpio.hpp"
//#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm2835.hpp"
#include "../../../Library/Display/Include/formatter.hpp"
#include "ISAObject.hpp"
#include "application.hpp"

class ThermometerAO : public ISAObject {
 private:
  volatile DWORD period;
  String *outputString;
  Message *logMsg, *eventMsg;
 public:
  Gpio *gpio16, *gpio20;

 protected:
  virtual DWORD processMessage(Message*);
  virtual AO_STACK * serviceInterrupt(AO_STACK *);
  virtual void run();
  void initLed();

 public:
          ThermometerAO(DWORD);
     void initSysTimer(DWORD delay);
     void logMessage(BYTE *msg);
     void wait_us(DWORD usec);
     void reset();
     void writeByte2DS(DWORD data);
     DWORD read2BytesFromDS();
};

#endif /* THERMOMETERAO_HPP_ */
