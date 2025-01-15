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
extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug
extern "C" unsigned int * get_sp(void); //@debug

#include "ISAObject.hpp"
#include "AOScheduler.hpp"

ISAObject * ISAObject::interruptTable[AO_INTERRUPT_TABLE_LENGTH];
WORD ISAObject::nestedLevel;

ISAObject::ISAObject( DWORD prio, DWORD intNumber ) : AObject( prio ) {
  registerInterrupt( intNumber );
}

void
ISAObject::registerInterrupt( DWORD intNumber ) {
  interruptTable[intNumber] = this;
}

AO_STACK *
cdecl
processInterrupt( DWORD iN, AO_STACK * stp ) {
  ISAObject::nestedLevel++;    // count nested entering to the interrupt processing
//  EXIT_CRITICAL()              // unmask interrupts
//  fp1.format(out, "> procIntr: iN=#%d stack=%h nestedLvl=%d\n\r", iN, stp, ISAObject::nestedLevel);
//  dump_debug_message(out);

  ISAObject * obj = ISAObject::interruptTable[iN];   // get IS Active Object corresponded iN
  if( obj != (ISAObject *)0 ) {
    stp = obj->serviceInterrupt( stp );      // run interrupt processing
  }

//  EXIT_CRITICAL()              // ??? unmask interrupts
  if( (ISAObject::nestedLevel == 1) && (iN != SCHEDULER_INTERRUPT_NUMBER)) {     // need epilog code ?
    AOScheduler *scheduler = (AOScheduler *)ISAObject::interruptTable[SCHEDULER_INTERRUPT_NUMBER];  // if it is not nested interrupt
    stp = scheduler->serviceInterrupt(obj, stp);                           // then run scheduler
  }

  ENTER_CRITICAL()                     // mask interrupts
  ISAObject::nestedLevel--;
//  fp1.format(out, "< procIntr: iN=#%d stack=%h nestedLvl=%d\n\r", iN, stp, ISAObject::nestedLevel);  // @debug
//  dump_debug_message(out);  // @debug
  return stp;                          // cause context switch
}

void
ISAObject::run() {
}
