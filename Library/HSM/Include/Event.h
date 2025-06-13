/*
   Copyright (C) 2010 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#ifndef _Event_H
#define _Event_H

#include "State.h"

/** Class Event does not used in this implementation of HSM (see Message from Kernel/Include/RingBuffer.hpp) */
class Event
{
  private:
// State of HSM when the Event was generated
    State *source;
    int id;
  
  public:
    Event( State * _source_, int n ){ source = _source_; id = n; };
    State * getSource(){ return source; };
    int getID(){ return id; };
};
#endif
