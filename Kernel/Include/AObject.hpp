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

#ifndef _AOBJECT_HPP
#define _AOBJECT_HPP

#include "commonDef.hpp"
#include "ListenerList.hpp"
#include "RingBuffer.hpp"
#include "Process.hpp"

/**
 * Class is responsible for main functionality of the active object such as
 * sending and receiving of events (or messages), managing of the message buffer.
 * This is a superclass for all RTOS components.
 */
class AObject : public Process {
/***************** Fields ***************/
 private:
/** List of the active objects that are registered to receive events from this active object.*/
   ListenerList *list;
/** ringBuffer keeps the incoming events for farther processing by the AO.*/
   RingBuffer<Message> *incomingRingBuffer;
/** ringBuffer keeps the outgoing events for farther processing by scheduler.*/
   RingBuffer<Message> *outgoingRingBuffer;

/***************** Methods ***************/
 protected:
/**
 * Method run executes custom code that implements given AO behavior.
 */
   virtual void run();

 private:
/**
 * Method start executes infinite loop that includes method run(),
 * retrieves events from buffer and passes its to processMessage()
 */
  void start();

/**
 * Helper static method to initiate run()
 */
   static void cdecl staticStart( AObject * ao );

/** Other AO or scheduler invoke this method to deliver message to this AO for processing.
 *  @param e - incoming message.
 */
   DWORD putIncomingMessage( Message * e );

 protected:
/**
 *  Method processes the coming event. As the method is virtual
 *  the subclasses can override it. The method for this subclass implements AO
 *  behavior itself (together with run()) or just passes
 *  event to state machine that can implement more compose functionality.
 */
   virtual DWORD processMessage( Message * );

/**
 * To send message to other AO then put this message to outgoing buffer.
 */
   inline DWORD putOutgoingMessage( Message * msg ) {return outgoingRingBuffer->write( msg );};

/**
 * Read next available message from incoming buffer.
 */
   inline DWORD getIncomingMessage( Message * msg ) {return incomingRingBuffer->get( msg );};

 public:
/**
 * Constructors
 *  @param prio - priority of the active object.
 */
     AObject( DWORD prio );
    ~AObject();

/**
 *  Method publishes messages generated by the active object for all its listeners
 */
   void publishMessages(AObject **);

/** adds an active object to listener list of the given active object.
 *  @param ao - reference to active object.
 */
   void addListener( AObject * ao );

/**
 * removes an active object from listener list of the given active object.
 *  @param ao - reference to active object.
 */
   void removeListener( AObject * ao );

/**
 * {Debug} Functions return level of loading of ring buffer.
 *  @return int - number of elements available for reading
 */
   inline DWORD incomingBufferLoad(){ return incomingRingBuffer->bufferLoad(); };
   inline DWORD outgoingBufferLoad(){ return outgoingRingBuffer->bufferLoad(); };
/**
 * Helper function for logging service.
 *  @param level - logging level (info = 0, error = 1, debug = 2).
 *  @param text - logging text.
 */
   virtual void log(BYTE level, char* text);
};
#endif /* _AOBJECT_HPP */
