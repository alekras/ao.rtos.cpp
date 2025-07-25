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

/** Helper file to gather the all cpp files to one */

#include "bcm_registers.hpp"
#include "os_cpu.hpp"

extern "C" void irq_handler_arm_timer();
extern "C" void irq_handler_mini_uart();

extern "C"
void irq_vectors_setup() {
  unsigned int * ptrD = 0;
  unsigned int * ptrS = &exception_vectors;
  int i;
  for (i = 0; i < 16; i++) {
    *(ptrD + i) = *(ptrS + i);
  }
  (*pDISABLE_IRQ_B) = 0xffffffff;
  (*pDISABLE_IRQ_1) = 0xffffffff;
  (*pDISABLE_IRQ_2) = 0xffffffff;
}

extern "C"
AO_STACK * processInterrupt( DWORD iN, AO_STACK * stp ) {
  irq_handler_arm_timer();
  return stp;
}

extern "C"
AO_STACK * isr(AO_STACK *sp) {
  AO_STACK* ret_sp = sp;
  if ((*pPENDING_IRQ_B) & 0x00000001) { // ARM Timer Interrupt
    ret_sp = processInterrupt(0, sp);
    (*pARM_TIMER_IRQ) = 0;
  }

  if ((*pPENDING_IRQ_1) & 0x20000000) {
    irq_handler_mini_uart();
  }

  return ret_sp;
}

extern "C" void * processSysCommand( DWORD size, DWORD type) {
  return 0;
}
