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

#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm2835.hpp"
#include "../../../Library/Display/Include/formatter.hpp"

extern "C" void sendString(char*);
extern "C" void irq_handler_arm_timer();
extern "C" void irq_handler_mini_uart();
extern char out[200];
extern FormatParser fp;

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
void dump_memory(unsigned int * a, int w) {
  for(int i = 0; i < w; i++) {
    fp.format(out, "%h) %h\r\n", (a + i), *(a + i));
    sendString(out);
  }
}

#define DUMP_BUFFER 0x30000000
#define DUMP_BUFFER_END 0x3000F000
char * dump_pointer;
unsigned int * dump_pointer_storage;
extern "C"
void dump_debug_init() {
  dump_pointer_storage = (unsigned int *)DUMP_BUFFER;
  dump_pointer = (char *)(DUMP_BUFFER + 4);
  *dump_pointer_storage = (unsigned int)dump_pointer;
  *dump_pointer = 0;
}

extern "C"
void dump_debug_message(char * msg) {
  if (dump_pointer > (char*)DUMP_BUFFER_END)
    return;
  do {
    *(dump_pointer++) = *msg;
  } while (*(msg++) != 0);
  *dump_pointer_storage = (unsigned int)dump_pointer;
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

extern "C"
void undefined_instruction_exeption(unsigned int * sp) {
  dump_debug_message("undefined instruction exception\r\n");
}

extern "C"
void prefetch_abort_exeption(unsigned int * sp) {
  dump_debug_message("prefetch abort exception\r\n");
}

extern "C"
void data_abort_exeption(unsigned int * sp) {
  dump_debug_message("data abort exception\r\n");
}

extern "C" void * processSysCommand( DWORD size, DWORD type) {
  return 0;
}
