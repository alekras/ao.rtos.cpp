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

#include "arm_debug_tools.hpp"

char *dump_pointer;
unsigned int dump_index;
unsigned int *dump_index_storage, *dump_max_index_storage;
char out[200]; // @debug
FormatParser fp1; // @debug


void dump_debug_init() {
  dump_index_storage = (unsigned int *)DUMP_BUFFER;
  dump_max_index_storage = (unsigned int *)(DUMP_BUFFER + 4);
  dump_pointer = (char *)(DUMP_BUFFER + 8);
  dump_index = 0;
  *dump_index_storage = dump_index;
  *dump_max_index_storage = dump_index;
  *(dump_pointer + dump_index) = 0;
}

void dump_debug_message(char * msg) {
  do {
    *(dump_pointer + dump_index) = *msg;
    dump_index = (dump_index + 1) & DUMP_INDEX_MASK;
  } while (*(msg++) != 0);
  *dump_index_storage = dump_index;
  if (dump_index > *dump_max_index_storage) {
    *dump_max_index_storage = dump_index;
  }
}

unsigned int get_dump_index() {
  return dump_index;
}

unsigned int get_dump_max_index() {
  return *((unsigned int *)DUMP_BUFFER);
}

void dump_memory(unsigned int * a, int w) {
  for(int i = 0; i < w; i++) {
    fp1.format(out, "%h) %h\r\n", (a + i), *(a + i));
    dump_debug_message(out);
  }
}

char* reg_names[] = {"r0","r1","r2","r3","r4","r5","r6", "r7","r8","r9","r10","r11","r12","lr_svc","lr_irq","spsr", "ukn", "ukn"};

void dump_stack(unsigned int * ssp, unsigned int * csp, unsigned int cpsr, unsigned int spsr) {
  fp1.format(out, "-- current SP=%8h CPSR=%8h SPSR=%8h --\r\n", csp, cpsr, spsr);
  dump_debug_message(out);
  for(int i = 0; i < 18; i++) {
    fp1.format(out, "%8h) %8h (%s)\r\n", (ssp + i), *(ssp + i), reg_names[i]);
    dump_debug_message(out);
  }
}

void dump_stack_(unsigned int * sp) {
  fp1.format(out, "-- current SP=%8h CPSR=%8h SPSR=%8h --\r\n", get_sp(), get_cpsr(), get_spsr());
  dump_debug_message(out);
  for(int i = 0; i < 18; i++) {
    fp1.format(out, "%8h) %8h (%s)\r\n", (sp + i), *(sp + i), reg_names[i]);
    dump_debug_message(out);
  }
}

void undefined_instruction_exception(unsigned int * sp) {
  dump_debug_message(" *** undefined instruction exception ***\r\n");
  dump_stack_(sp);
}

void prefetch_abort_exception(unsigned int * sp) {
  dump_debug_message(" *** prefetch abort exception ***\r\n");
  dump_stack_(sp);
}

void data_abort_exception(unsigned int * sp) {
  dump_debug_message(" *** data abort exception ***\r\n");
  dump_stack_(sp);
}

void reset_exception(unsigned int * sp) {
  dump_debug_message(" *** reset exception ***\r\n");
  dump_stack_(sp);
}

void unpredicted_exception(unsigned int * sp) {
  dump_debug_message(" *** unpredicted exception ***\r\n");
  dump_stack_(sp);
}
