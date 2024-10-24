/*
   Copyright (C) 2010-2014 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#ifndef _FSM_HPP
#define _FSM_HPP

template <class MessageType>
class Fsm {
  public:
    typedef int (Fsm::*State)(MessageType* msg);
             Fsm(State initial) : myState(initial){};
//    virtual ~Fsm(){};
    int     init(){return dispatch((MessageType*)0);};
    int     dispatch(MessageType* msg){return (this->*myState)(msg);};
  protected:
    void     tran(State target){myState = target;};
    #define TRANSITION(target_) tran(static_cast<State>(target_))
    State myState;
};

enum Phase
{
  ENTER,
  EVENT,
  EXIT
};  

template <class MessageType>
class Efsm {
  public:
    typedef int (Efsm::*State)(Phase phase, MessageType* msg);
             Efsm(State initial) : myState(initial){(this->*myState)(ENTER, (MessageType*)0);};
//    virtual ~Fsm(){};
//    int     init(){return dispatch((MessageType*)0);};
    int     dispatch(MessageType* msg){return (this->*myState)(EVENT, msg);};
  protected:
    void     tran(State target){
        (this->*myState)(EXIT, (MessageType*)0); 
        myState = target; 
        (this->*myState)(ENTER, (MessageType*)0);
      };
    #define TRANSITION(target_) tran(static_cast<State>(target_))
    State myState;
};

#endif /* _FSM_HPP */
