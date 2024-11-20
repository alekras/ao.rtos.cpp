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

#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include "../../../Library/Display/Include/formatter.hpp"
extern char out[200]; // @debug
extern FormatParser fp1; // @debug
extern "C" void dump_debug_message(char *); //@debug

#include "commonDef.hpp"
#include "String.hpp"

enum class MessageType : BYTE {
  signal,
  binary,
  string
};

//extern DWORD stringLength(BYTE *);
/**
 *  class Message encapsulates fields:
 *  src -
 *  dest -
 *  payload -
 *  type -
 *  messageId -
 */
class Message {
 private:
  DWORD_S src, dest;
  DWORD payload;
  MessageType type;
  BYTE messageId;

 public:
  Message() : src(-1), dest(-1), payload(0), type(MessageType::signal), messageId(no) {}

  Message(DWORD_S src, DWORD_S dest, DWORD data, MessageType type, MessageID mid) :
    src(src), dest(dest), payload(data), type(type), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, DWORD data, MessageID mid) :
    src(src), dest(dest), payload(data), type(MessageType::binary), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, String *string, MessageID mid) :
    src(src), dest(dest), payload((DWORD)string), type(MessageType::string), messageId(mid) {}

  ~Message() {
    if (type == MessageType::string) {
      delete (String*)payload;
    }
  }

  Message(const Message &msg) {src = msg.src; dest = msg.dest; payload = msg.payload; type = msg.type; messageId = msg.messageId;}

  void operator = (const Message &msg) {
    src = msg.src;
    dest = msg.dest;
    payload = msg.payload;
    type = msg.type;
    messageId = msg.messageId;
  }

  inline MessageID getMessageID() {return (MessageID) messageId;}
  inline void setMessageID(MessageID mid) {messageId = (MessageID) mid;}
  inline MessageType getType() {return type;}
  DWORD getBinaryData() {
    switch (type) {
      case MessageType::binary :
        return (DWORD) payload;
      case MessageType::string :
        return (DWORD) -1;
    }
    return (DWORD) -1;
  }
  inline void setBinaryData(DWORD d) {payload = d;}
  String * getString() {
    switch (type) {
      case MessageType::binary :
        return (String*) -1;
      case MessageType::string :
        return (String*) payload;
    }
    return (String*) -1;
  }
  inline void setString(String *d) {payload = (DWORD)d;}
  inline DWORD_S getSource() {return src;}
  inline DWORD_S getDestination() {return dest;}
};

#endif /* _MESSAGE_HPP */
