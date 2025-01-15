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

#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include "commonDef.hpp"
#include "String.hpp"
#include "Binary.hpp"

enum class MessageType : BYTE {
  onechar,
  binary,
  string,
  word
};

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
  Message() : src(-1), dest(-1), payload(0), type(MessageType::word), messageId(no) {}

  Message(DWORD_S src, DWORD_S dest, DWORD data, MessageType type, MessageID mid) :
    src(src), dest(dest), payload(data), type(type), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, DWORD data, MessageID mid) :
    src(src), dest(dest), payload(data), type(MessageType::word), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, Binary *binary, MessageID mid) :
    src(src), dest(dest), payload((DWORD)binary), type(MessageType::binary), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, String *string, MessageID mid) :
    src(src), dest(dest), payload((DWORD)string), type(MessageType::string), messageId(mid) {}

  Message(DWORD_S src, DWORD_S dest, char character, MessageID mid) :
    src(src), dest(dest), payload((DWORD)character), type(MessageType::onechar), messageId(mid) {}

  ~Message() {
    switch (type) {
      case MessageType::binary :
        delete (Binary*)payload;
        break;
      case MessageType::string :
        delete (String*)payload;
        break;
      default:
        break;
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

  Binary* getBinary() {
    switch (type) {
      case MessageType::binary :
        return (Binary*) payload;
      default :
        return (Binary*) -1;
    }
  }
  inline void setBinary(Binary* d) {payload = (DWORD)d; type = MessageType::binary;}

  String * getString() {
    switch (type) {
      case MessageType::string :
        return (String*) payload;
      default :
        return (String*) -1;
    }
  }
  inline void setString(String *d) {payload = (DWORD)d; type = MessageType::string;}

  inline BYTE getChar() {return (BYTE) payload;}
  inline void setChar(BYTE ch) {payload = (DWORD) ch;}

  inline DWORD getWord() {return payload;}
  inline void setWord(DWORD wd) {payload = wd;}

  inline DWORD_S getSource() {return src;}
  inline DWORD_S getDestination() {return dest;}
};

#endif /* _MESSAGE_HPP */
