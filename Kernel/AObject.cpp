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
extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug
#include "AObject.hpp"

typedef void cdecl (*fp)( AObject * );           // helper typedef for casting function run()

void cdecl
AObject::staticStart( AObject * ao ) {
  ao->start();
}

AObject::AObject( DWORD prio ) : Process( prio ) {
  list = new ListenerList(AO_LISTENERS_LIST_LENGTH);
  incomingRingBuffer = new RingBuffer<Message>(AO_RINGBUFFER_LENGTH);
  outgoingRingBuffer = new RingBuffer<Message>(AO_RINGBUFFER_LENGTH);
  Process::init( this, &staticStart );
}

AObject::~AObject() {
  delete list;
  delete incomingRingBuffer;
  delete outgoingRingBuffer;
}

void
AObject::start() {
  Message msg;
  int failedProcess = 0;   // count of failed try to process incoming events from buffer.
  while ( stop == 0 ) {    // this is infinite loop while stop = 0;
    run();

    if ( incomingRingBuffer->get( &msg ) == 0 ) { // try to read event from buffer
      if (priority == AO_SCHEDULED_LIST_LENGTH - 1) { // is it scheduler
        ready = 1;          // scheduler is always ready to run
      } else {
        ready = 0;          // have no any events to process
        AO_CONTEXT_SW();    // pass CPU control to others AO by invoking of scheduler
      }
    } else {
      Message rev( msg );                   // create a clone of msg; TODO: because msg can be changed during processing by HSM ==not good==
      if( processMessage( &msg ) == 0 ) {  // processMessage() can not complete a proceeding of the event
        putIncomingMessage( &rev );         // return the event back to the buffer ( to tail)
        failedProcess++;
// In this point AO can continue to process other events. But if size of incoming buffer equals
// to failedProcess it means that buffer contains only failed events so it is time to allow
// other AO to run.
        if( incomingRingBuffer->bufferLoad() <= failedProcess ) {
          failedProcess = 0;
          ready = 0;                    // allow scheduler to activate another AO
          AO_CONTEXT_SW();              // schedule a tasks ...
        }
      }
    }
  }
}

void
AObject::run() {
}

/**
 * we can override this method in superclass
 */
DWORD
AObject::processMessage(Message *) {
  return 0;
}

void
AObject::publishMessages(AObject **scheduledAOTable) {
    Message msg;
    while (outgoingRingBuffer->get( &msg ) != 0) {
      if (msg.getDestination() > 0) {   // if a message has explicitly defined destination
        AObject *destObj = scheduledAOTable[msg.getDestination()];
        if (destObj != 0) {      // if a destination AO exist
          destObj->putIncomingMessage(&msg);  // put the message to income buffer of defined AO,
        }
      } else {                              // otherwise shot all listeners
        if (msg.getType() != MessageType::string) {   // for string type of a message only explicit destination allowed
          for (int i = 0; i < list->length(); i++) {  // send message to the all AOs theirs are listening to this Active object
            list->elementAt(i)->putIncomingMessage(&msg);
          }
        }
      }
// TODO: return the Message to outgoingRingBuffer if an insertion is failed (ONLY for failed destination) !!!
    }
}

void
AObject::addListener(AObject * obj) {
  list->add(obj);
}

void
AObject::removeListener(AObject * obj) {
  list->remove(obj);
}

DWORD
AObject::putIncomingMessage(Message * msg) {
  ready = 1; // this AO is ready to run. Scheduler will give it control during next schedule time
             // when this priority will be highest in the system.
// put incoming message to the buffer for further processing
  return incomingRingBuffer->write(msg);
}

void AObject::log(BYTE level, char* text){
  if (level > LOGGING_LEVEL) {
    int size = 80;
    char *logText = new char[size];
    char *s = logText;
    while (*text != 0) {
      *s++ = *text++;
    }
    *s = 0;
    Message msg(priority, 0, (BYTE*) logText, logging);
    putOutgoingMessage(&msg);
  }
}
