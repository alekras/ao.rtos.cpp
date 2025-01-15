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

#include "RingBuffer.hpp"
#include "AObject.hpp"

template <class MessageType>
RingBuffer<MessageType>::RingBuffer(DWORD n) : wrPo(0), rdPo(0) {
  N = ( n > AO_RINGBUFFER_LENGTH ) ? AO_RINGBUFFER_LENGTH : n;
  queue = new MessageType[N];
}

template <class MessageType>
RingBuffer<MessageType>::RingBuffer() : N(AO_RINGBUFFER_LENGTH), wrPo(0), rdPo(0) {
  queue = new MessageType[N];
}

template <class MessageType>
RingBuffer<MessageType>::~RingBuffer() {
  delete [] queue;
}

template <class MessageType>
void
RingBuffer<MessageType>::put( MessageType * message ) {
  queue[wrPo++] = *message;
  wrPo = wrPo % N;   // set pointer to next element and revert to 0 if wrPo >= N (implements a ring)
}

template <class MessageType>
void
RingBuffer<MessageType>::get( MessageType * message ) {
  *message = queue[rdPo++];
  rdPo = rdPo % N;   // go to next element and revert to 0 if rdPo >= N (implements a ring)
}

template <class MessageType>
MessageType *
RingBuffer<MessageType>::read() {
  return &queue[rdPo];
}

template <class MessageType>
void
RingBuffer<MessageType>::remove() {
  rdPo = (++rdPo) % N;   // go to next element and revert to 0 if rdPo >= N (implements a ring)
}

template class RingBuffer<Message>;
template class RingBuffer<BYTE>;
