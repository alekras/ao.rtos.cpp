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

#ifndef ARM_DEBUG_TOOLS_HPP_
#define ARM_DEBUG_TOOLS_HPP_

#include "bcm_registers.hpp"
#include "os_cpu.hpp"
#include "formatter.hpp"

extern "C" unsigned int * get_sp(void);
extern "C" unsigned int get_cpsr(void);
extern "C" unsigned int get_spsr(void);

extern char out[200]; // @debug
extern FormatParser fp1; // @debug

#define DUMP_BUFFER 0x30000000
#define DUMP_BUFFER_END 0x3000F000

extern "C" void dump_debug_init();
extern "C" void dump_debug_message(char * msg);

template <class ... Args>
void dump_debug_f_message(char * format, Args ... args) {
  fp1.format(out, format, args...);
  dump_debug_message(out);
}

extern "C" void dump_memory(unsigned int * a, int w);

extern "C" void dump_stack(DWORD * ssp, DWORD * csp, DWORD cpsr, DWORD spsr);
extern "C" void dump_stack_(DWORD * sp);

extern "C" void undefined_instruction_exception(DWORD * sp);
extern "C" void prefetch_abort_exception(DWORD * sp);
extern "C" void data_abort_exception(DWORD * sp);
extern "C" void reset_exception(DWORD * sp);
extern "C" void unpredicted_exception(DWORD * sp);

#endif /* ARM_DEBUG_TOOLS_HPP_ */
