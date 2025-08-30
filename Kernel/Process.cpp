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

#include "Process.hpp"

Process::Process(DWORD prio) : priority(prio), ready(1), stop(0) {
  stack = new AO_STACK[AO_STACK_LENGTH];
}

Process::~Process() {
  delete [] stack;
}
/**
void
Process::init( AObject * subClassThis, void cdecl (*fp)( AObject * ) ) {
  sp = &stack[AO_STACK_LENGTH];    // Load stack pointer
  *(--sp) = (DWORD)subClassThis;   // Simulate call to function with argument <this>
  *(--sp) = 0;                     // return address (fictive)
  *(--sp) = 0x00000202;            // SW = Interrupts enabled
  *(--sp) = 0x00000008;            // selector of code segment
  *(--sp) = (DWORD)fp;             // Put pointer to function staticRun() as point of interrupt
  *(--sp) = 0xAAAA;                // EAX = 0xAAAA
  *(--sp) = 0xCCCC;                // ECX = 0xCCCC
  *(--sp) = 0xDDDD;                // EDX = 0xDDDD
  *(--sp) = 0xBBBB;                // EBX = 0xBBBB
  *(--sp) = 0x0000;                // ESP = 0x0000
  *(--sp) = 0x1111;                // EBP = 0x1111
  *(--sp) = 0x2222;                // ESI = 0x2222
  *(--sp) = 0x3333;                // EDI = 0x3333
}
**/

void
Process::init( AObject * subClassThis, void cdecl (*fp)( AObject * ) ) {
  sp = &stack[AO_STACK_LENGTH];    // Load stack pointer
  *(--sp) = 0x153;                 // spsr to restore after exception -
//  *(--sp) = 0x150;                 // spsr to restore after exception -
  *(--sp) = (DWORD)fp;             // lr address to return after exception
  *(--sp) = (DWORD)fp;             // lr current return address for svc mode
  *(--sp) = 0xF00C;                // r12
  *(--sp) = 0xF00B;                // r11
  *(--sp) = 0xF00A;                // r10
  *(--sp) = 0xF009;                // r9
  *(--sp) = 0xF008;                // r8
  *(--sp) = 0xF007;                // r7
  *(--sp) = 0xF006;                // r6
  *(--sp) = 0xF005;                // r5
  *(--sp) = 0xF004;                // r4
  *(--sp) = 0xF003;                // r3
  *(--sp) = 0xF002;                // r2
  *(--sp) = 0xF001;                // r1
  *(--sp) = (DWORD)subClassThis;   // r0=<this> Simulate call to function with argument <this>
}
