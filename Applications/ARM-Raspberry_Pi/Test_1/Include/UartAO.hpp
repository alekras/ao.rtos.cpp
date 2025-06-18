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

#ifndef UARTAO_HPP_
#define UARTAO_HPP_

#include "../../../Porting/ARM-Raspberry_Pi/Include/os_cpu.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"
#include "gpio.hpp"
//#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm2835.hpp"
#include "ISAObject.hpp"
#include "application.hpp"

class UartAO : public ISAObject {
 private:
  volatile bool flag;
  RingBuffer<BYTE> *txRingBuffer;
  RingBuffer<BYTE> *rxRingBuffer;

  Message *comMsg;
  volatile int sec;

  Message *inMsg;

 protected:
  virtual DWORD processMessage(Message*);
  virtual AO_STACK * serviceInterrupt(AO_STACK *stp);
  void initUart();

 public:
          UartAO(DWORD);
};

#endif /* UARTAO_HPP_ */
