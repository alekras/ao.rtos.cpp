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

#include "./Include/arm_debug_tools.hpp"

void dump_debug_init() {
  dump_pointer_storage = (unsigned int *)DUMP_BUFFER;
  dump_pointer = (char *)(DUMP_BUFFER + 4);
  *dump_pointer_storage = (unsigned int)dump_pointer;
  *dump_pointer = 0;
}

void dump_debug_message(char * msg) {
  if (dump_pointer > (char*)DUMP_BUFFER_END)
    return;
  do {
    *(dump_pointer++) = *msg;
  } while (*(msg++) != 0);
  *dump_pointer_storage = (unsigned int)dump_pointer;
}

void dump_memory(unsigned int * a, int w) {
  for(int i = 0; i < w; i++) {
    fp1.format(out, "%h) %h\r\n", (a + i), *(a + i));
    dump_debug_message(out);
  }
}

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
