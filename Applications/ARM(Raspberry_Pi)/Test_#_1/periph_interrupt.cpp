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

#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "formatter.hpp"

extern "C" AO_STACK * processInterrupt( DWORD iN, AO_STACK * stp );

extern "C"
void irq_vectors_setup() {
  unsigned int * ptrD = 0;
  unsigned int * ptrS = &exception_vectors;
  int i;
  for (i = 0; i < 16; i++) {
    *(ptrD + i) = *(ptrS + i);
  }
}

extern "C"
void arm_timer_setup(int p_msec) {
// ARM timer freq = 250MHz divider = 0x7C = 124, so timer freq = 250/(124 + 1) = 2MHz

  (*pARM_TIMER_LD) = 2000 * p_msec; // = armTimerFreq * p_msec / 1000

  (*pARM_TIMER_CTL) = 0x000000A2;
  (*pARM_TIMER_PDIV) = 0x0000007C; //
  (*pENABLE_IRQ_B) = 0x00000001; // Enable ARM timer interrupts
}

// TO DO - move to assembler code arm_cpu.s ???
extern "C"
AO_STACK * isr(AO_STACK *sp) {
  AO_STACK* ret_sp = sp;
  if ((*pPENDING_IRQ_B) & 0x00000001) { // ARM Timer Interrupt
    (*pARM_TIMER_IRQ) = 0;
    return processInterrupt(0, sp);
  }

  if ((*pPENDING_IRQ_1) & 0x20000000) {
    return processInterrupt(1, sp);   // mini UART Interrupt (AUX interrupt)
  }

  if ((*pPENDING_IRQ_2) & 0x001e0000) {
    return processInterrupt(2, sp);   // GPIO Interrupt
  }

  if (((*pPENDING_IRQ_1) & 0x00000002) && ((*pSYS_TIMER_CNTRL_STAT) & 0x2)) {
    return processInterrupt(3, sp);   // SYS timer compare interrupt
  }

  return ret_sp;
}
