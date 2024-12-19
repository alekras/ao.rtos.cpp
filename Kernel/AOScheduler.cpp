/*
   Copyright (C) 2007-2024 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

//extern char out[200]; // @debug
//extern FormatParser fp1; // @debug
extern "C" void dump_stack(unsigned int *, unsigned int *, unsigned int, unsigned int);
extern "C" unsigned int * get_sp(void);
extern "C" unsigned int get_cpsr(void);
extern "C" unsigned int get_spsr(void);

#include "AOScheduler.hpp"

/**
 *  Layout of the scheduledAOTable array:
 *  scheduledAOTable[0]  -  Active Object with priority=0
 *  scheduledAOTable[1]  -  Active Object with priority=1
 *         .
 *         .
 *         .
 *  scheduledAOTable[N-2]  -  Active Object with priority=N-2
 *  scheduledAOTable[N-1]  -  Scheduler (currentPrio = prio = AO_SCHEDULED_LIST_LENGTH - 1)
 *  scheduledAOTable[N]  -  Idle Active Object (prio = AO_SCHEDULED_LIST_LENGTH)
 */
AOScheduler::AOScheduler() : ISAObject( AO_SCHEDULED_LIST_LENGTH - 1, SCHEDULER_INTERRUPT_NUMBER ) {
  currentPrio = priority;
  N = AO_SCHEDULED_LIST_LENGTH;       // set up table limit
  scheduledAOTable = new AObject*[AO_SCHEDULED_LIST_LENGTH + 1];
  for( int i = 0; i < N + 1; i++ ) {     // clean up array
    scheduledAOTable[i] = (AObject*)0;
  }
  scheduledAOTable[priority] = this;  // put itself in table
}

AOScheduler::~AOScheduler() {
  delete [] scheduledAOTable;
}

DWORD
AOScheduler::add( AObject * obj ) {
  DWORD prio = obj->getPriority();
  if( prio >= (N - 1) || scheduledAOTable[prio] != (AObject *)0 )
    return 0; // priority of AO have to be less than (N-1) (index of scheduler in the table)
              // and element has to be empty
  scheduledAOTable[prio] = obj;
  currentPrio = prio;
  return 1;
}

DWORD
AOScheduler::remove( AObject * obj ) {
  DWORD prio = obj->getPriority();
  if( prio >= (N - 1) || scheduledAOTable[prio] == (AObject *)0 )
    return 0;
  scheduledAOTable[prio] = (AObject *)0;
  return 1;
}

void
AOScheduler::iterateAObjects(void (AOScheduler::*fp)( AObject * )){
  AObject * obj;
  for( int i = 0; i < N; i++ ) {
    obj = scheduledAOTable[i];
    if( (obj != (AObject *)0) ) {
      (this->*fp)(obj);
    }
  }
}

AO_STACK *
AOScheduler::serviceInterrupt(ISAObject * obj, AO_STACK * stkp ) {
  scheduledAOTable[currentPrio]->setSP( stkp ); // save a pointer to stack of a current Active object

  if (obj != (ISAObject *)0) {
    obj->publishMessages(scheduledAOTable);
    currentPrio = obj->getPriority();        // set new current AO priority
    stkp = obj->getSP();      // stack pointer of new current AO to switch on CPU context.
  }
  return stkp;
}

AO_STACK *
AOScheduler::serviceInterrupt( AO_STACK * stkp ) {
//  fp1.format(out, " >> AOScheduler::srvIntrr: stack=%h curPrio=%d\r\n", stkp, currentPrio);  // @debug
//  dump_debug_message(out);  // @debug
  AObject * obj;
  scheduledAOTable[currentPrio]->setSP( stkp ); // save a pointer to stack of a current Active object

  int j = N + 1;
  for( int i = 0; i < N; i++ ) {   // navigate thru the table and transfer messages from given AO to listeners
    obj = scheduledAOTable[i];
    if( (obj != (AObject *)0) ) {   // element has to be not null
      obj->publishMessages(scheduledAOTable);
      if ((i < j) && (obj->isReady() == 1)) {  // ready AO with highest priority
        currentPrio = obj->getPriority();        // set new current AO priority
        stkp = obj->getSP();      // stack pointer of new current AO to switch on CPU context.
        j = i;  // mark ready AO with max priority
      }
//      fp1.format(out, " -- publish msg's for ao[%d] obj=%8h stack=%8h ready=%d\r\n", i, obj, obj->getSP(), obj->isReady());  // @debug
//      dump_debug_message(out);  // @debug
    }
  }

//  fp1.format(out, " << AOScheduler::srvIntrr(2): stack=%h curPrio=%d\r\n", this->sp, currentPrio);  // @debug
//  dump_debug_message(out);  // @debug
  return stkp;
}

void
AOScheduler::startOS() {
  currentPrio = priority + 1;       // create idle AO and put it in the table to take-off RTOS
  AObject idleObject( currentPrio );
  scheduledAOTable[currentPrio] = &idleObject;
//  currentPrio = priority;           // starting with the scheduler AO
  ready = 1;                        // scheduler ISAO is always ready to run
  EXIT_CRITICAL()
  AO_CONTEXT_SW()                   // program interrupt will activate scheduler in multitask enviroment
// never come in here
}

void *
cdecl
processSysCommand( DWORD size, DWORD type) {
//  fp1.format(out, "> processSysCommand: size=%d type=%d\r\n", size, type);  // @debug
//  dump_debug_message(out);  // @debug
  switch (type) {
    case 1:
      return mm->malloc(size);
    case 2:
      mm->free((void *)size);
      return 0;
    default:
      return 0;
  }
}

void
AOScheduler::run() {
//  dump_debug_message("> run: scheduler\r\n");  // @debug
//  int * p = new int[5];
//  fp1.format(out, "> run: p=%d\r\n", p);  // @debug
//  dump_debug_message(out);  // @debug
//  delete[] p;
}

DWORD
AOScheduler::processMessage( Message * msg ) {
  return 1;
}
