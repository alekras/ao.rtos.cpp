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
extern "C" unsigned int * get_sp(void); // @debug
#include "AObject.hpp"

typedef void cdecl (*fp)( AObject * );           // helper typedef for casting function staticStart()

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
  Message *msg;
  int failedProcess = 0;   // count of failed try to process incoming events from buffer.
  while ( stop == 0 ) {    // this is infinite loop while stop = 0;
    run();
//    fp1.format(out, " inside start() : obj=%h, prio=%d ready=%d\r\n", this, priority, ready);  // @debug
//    dump_debug_message(out);  // @debug

    if ( incomingRingBuffer->isEmpty() ) { // try to read event from buffer
      if (priority == (AO_SCHEDULED_LIST_LENGTH - 1)) { // is it scheduler
        ready = 1;          // scheduler is always ready to run
      } else {
        ready = 0;          // have no any events to process
        AO_CONTEXT_SW();    // pass CPU control to others AO by invoking of scheduler
      }
    } else {
      msg = incomingRingBuffer->read();
      if( processMessage( msg ) != 0 ) {
        incomingRingBuffer->remove();
        delete msg;
      } else {  // processMessage() can not complete a proceessing of the event
        failedProcess++;
        incomingRingBuffer->put(msg);
        incomingRingBuffer->remove();
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
  return 1;
}

void
AObject::publishMessages(AObject **scheduledAOTable) {
    Message *msg;
    while (!outgoingRingBuffer->isEmpty()) {
      msg = outgoingRingBuffer->read();
      fp1.format(out, " ** Message publish : msg=%h prio=%d type=%d string='%s'\r\n", msg, priority, msg->getType(), msg->getString());  // @debug
      dump_debug_message(out);  // @debug
      DWORD_S destPrio = msg->getDestination();
      if (destPrio > 0) {   // if a message has explicitly defined destination
        AObject *destObj = scheduledAOTable[destPrio];
        if (destObj != 0) {      // if a destination AO exist
          if (destObj->putIncomingMessage(msg) == 1) {  // put the message to income buffer of defined AO,
            outgoingRingBuffer->remove(); // remove message from outgoing buffer
          }
          // if the message has not be copied to other AO incoming buffer then
          // do not remove it and leave for processing in next steps
        } else {
          outgoingRingBuffer->remove();
        }
      } else {                              // otherwise shot all listeners
        if (msg->getType() != MessageType::string) {   // for string type of a message only explicit destination allowed
          for (int i = 0; i < list->length(); i++) {  // send message to the all listeners of the AO
// TODO: return the Message to outgoingRingBuffer if an insertion is failed (ONLY for failed destination) !!!
            list->elementAt(i)->putIncomingMessage(msg); // @todo check success of msg copy
          }
        }
        outgoingRingBuffer->remove();
      }
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
  if (incomingRingBuffer->isFull()) {
    return 0;
  } else {
    ready = 1; // this AO is ready to run. Scheduler will give it control during next schedule time
             // when this priority will be highest in the system.
// put incoming message to the buffer for further processing
    incomingRingBuffer->put(msg);
    return 1;
  }
}

DWORD
AObject::putOutgoingMessage( Message * msg ) {
  if (outgoingRingBuffer->isFull()) {
    return 0;
  } else {
    message = (*msg);
    if (message.getType() == MessageType::string) {
      String * originString = message.getString();
      String * newString = new String(originString->getChars());
//      fp1.format(out, " ** Message out : msg=%h prio=%d type=%d string='%s'\r\n", msg, priority, message.getType(), originString->getChars());  // @debug
//      dump_debug_message(out);  // @debug
      message.setString(newString);
    }
    if (message.getType() == MessageType::binary) {
      Binary * originBin = message.getBinary();
      Binary * newBin = new Binary(originBin->getData(), originBin->length());
//      fp1.format(out, " ** Message out : msg=%h prio=%d type=%d binary[0]='%h'\r\n", msg, priority, message.getType(), originBin->getData()[0]);  // @debug
//      dump_debug_message(out);  // @debug
      message.setBinary(newBin);
    }
    outgoingRingBuffer->put(&message);
    return 1;
  }
}

void AObject::log(BYTE level, char* text){
  if (level > LOGGING_LEVEL) {
    int size = 80;
    BYTE *logText = new BYTE[size];
    BYTE *s = logText;
    while (*text != 0) {
      *s++ = *text++;
    }
    *s = 0;
    Message msg(priority, 0, new String(logText), logging);
    putOutgoingMessage(&msg);
  }
}
