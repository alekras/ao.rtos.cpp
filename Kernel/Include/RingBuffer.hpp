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

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include "Message.hpp"

/**
 * Class RingBuffer keeps incoming messages. Pointer wrPo points to first empty element of buffer,
 * ready to accept an incoming message. rdPo points to first buffer element ready to be read out.
 * Pointers are moving thru buffer cyclically when pointer reach the bottom of buffer it go to
 * top.
 */
template <class MessageType>
class RingBuffer {
/***************** Fields ***************/
 private:
/** wrPo keeps index of element of queue that empty and ready to accept new message.*/
	volatile DWORD wrPo;
/** rdPo keeps index of element of queue that ready to be read.*/
	volatile DWORD rdPo;
/** Size of queue.*/
  DWORD N;
/** Array of Messages */
  MessageType *queue; //[AO_RINGBUFFER_LENGTH];
/***************** Methods ***************/

 public:
/** Constructor that creates RingBuffer with size of queue.
 *  @param int size - length of queue.
 */
    RingBuffer( DWORD size );
/** Default constructor.*/
    RingBuffer();
    ~RingBuffer();

/** The method writes message msg to buffer.
 *  @param MessageType * msg - reference to incoming message
 *  @return int - 1 if success, 0 if buffer is full.
 */
    void put( MessageType * msg );

/** The method reads first available element of buffer, saves it in (*msg) and moves a read pointer.
 *  @param MessageType * msg - reference to destination Message object
 *  @return int - 1 if success, 0 if buffer is empty.
 */
    void get( MessageType * msg );

/** The method returns pointer to first available element of buffer.
 *  @return pointer MessageType * , 0 if buffer is empty.
 */
    MessageType * read();

/** The method removes first available element of buffer and moves a read pointer.
 *  @return int - 1 if success, 0 if buffer is empty.
 */
    void remove();

/** The method reads first available element of the buffer, and writes it
 *  to other RingBufferand.
 *  @param RingBuffer * msg - reference to other Ring Buffer
 *  @return int - 1 if success, 0 if buffers is empty or full.
 */
//    DWORD move( AObject * other );

/** The method returns amount of elements that are available for reading.
 */
    DWORD bufferLoad() {
      DWORD_S d = wrPo - rdPo;
      return ((d >= 0) ? 0 : N) + d;
    };
/** The method returns buffer space available for writing.
 */
    DWORD bufferVacancy() {
      DWORD_S d = wrPo - rdPo;
      return ((d >= 0) ? N : 0) - d - 1;
    };

    inline bool isEmpty() { return (bufferLoad() == 0); };
    inline bool isFull() { return (bufferVacancy() == 0); };
};

#endif
