/*
   Copyright (C) 2007-2015 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"

extern "C"
void sys_timer_setup() {
// Counter is running on freq = 1MHz and 1,000,000 cycles = 0xF4240
  (*pSYS_TIMER_CMP_1) = (*pSYS_TIMER_COUNT_LO) + 0xF4240;
  (*pSYS_TIMER_CNTRL_STAT) = 2; // Clear CMP1 detected
  (*pENABLE_IRQ_1) = 0x00000002; // Enable system timer interrupts
}

Gpio *gpio47, *gpio10, *gpio22;

extern "C"
void arm_timer_setup(int p_msec) {
// ARM timer freq = 250MHz divider = 0x7C = 124, so timer freq = 250/(124 + 1) = 2MHz

  (*pARM_TIMER_LD) = 2000 * p_msec; // = armTimerFreq * p_msec / 1000

  (*pARM_TIMER_CTL) = 0x000000A2;
  (*pARM_TIMER_PDIV) = 0x0000007C; //
  (*pENABLE_IRQ_B) = 0x00000001; // Enable ARM timer interrupts
}

extern "C"
void led_setup() {
  gpio47->setFunction(1);
  gpio47->clearLevel();
  gpio10->setFunction(1);
  gpio10->setLevel();
  gpio22->setFunction(1);
  gpio22->setLevel();
}

extern "C"
void irq_handler_sys_timer() {
  static volatile int tic = 0;

  if (tic++ % 2 == 0) {
    gpio47->clearLevel();
  } else {
    gpio47->setLevel();
  }

  (*pSYS_TIMER_CMP_1) = (*pSYS_TIMER_COUNT_LO) + 0xF4240;
  (*pSYS_TIMER_CNTRL_STAT) = 2;
}

