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

#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "formatter.hpp"

extern "C" AO_STACK * processInterrupt( DWORD iN, AO_STACK * stp );
extern "C" unsigned int * get_sp(void);
extern "C" unsigned int get_cpsr(void);
extern "C" unsigned int get_spsr(void);

extern char out[200]; // @debug
extern FormatParser fp1; // @debug

extern "C"
void irq_vectors_setup() {
  unsigned int * ptrD = 0;
  unsigned int * ptrS = &exception_vectors;
  int i;
  for (i = 0; i < 16; i++) {
    *(ptrD + i) = *(ptrS + i);
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
void dump_memory(unsigned int * a, int w) {
  for(int i = 0; i < w; i++) {
    fp1.format(out, "%h) %h\r\n", (a + i), *(a + i));
    dump_debug_message(out);
  }
}

char* reg_names[] = {"r0","r1","r2","r3","r4","r5","r6", "r7","r8","r9","r10","r11","r12","lr_svc","lr_irq","spsr", "ukn", "ukn"};
extern "C"
void dump_stack(unsigned int * ssp, unsigned int * csp, unsigned int cpsr, unsigned int spsr) {
  fp1.format(out, "-- current SP=%8h CPSR=%8h SPSR=%8h --\r\n", csp, cpsr, spsr);
  dump_debug_message(out);
  for(int i = 0; i < 18; i++) {
    fp1.format(out, "%8h) %8h (%s)\r\n", (ssp + i), *(ssp + i), reg_names[i]);
    dump_debug_message(out);
  }
}

extern "C"
void dump_stack_(unsigned int * sp) {
  fp1.format(out, "-- current SP=%8h CPSR=%8h SPSR=%8h --\r\n", get_sp(), get_cpsr(), get_spsr());
  dump_debug_message(out);
  for(int i = 0; i < 18; i++) {
    fp1.format(out, "%8h) %8h (%s)\r\n", (sp + i), *(sp + i), reg_names[i]);
    dump_debug_message(out);
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

Gpio *gpio47;

extern "C"
void led_setup() {
  gpio47->setFunction(1);
  gpio47->clearLevel();
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

  return ret_sp;
}

extern "C"
void undefined_instruction_exeption(unsigned int * sp) {
  dump_debug_message(" *** undefined instruction exception ***\r\n");
  dump_stack_(sp);
}

extern "C"
void prefetch_abort_exeption(unsigned int * sp) {
  dump_debug_message(" *** prefetch abort exception ***\r\n");
  dump_stack_(sp);
}

extern "C"
void data_abort_exeption(unsigned int * sp) {
  dump_debug_message(" *** data abort exception ***\r\n");
  dump_stack_(sp);
}
