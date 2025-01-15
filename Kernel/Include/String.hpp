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

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include "commonDef.hpp"

class String {
 private:
  BYTE * characters;
  DWORD size;

 public:
  String() : size(1) {
    characters = new BYTE[size];
    characters[0] = 0;
  }

  String(DWORD sz) : size(sz + 1) {
    characters = new BYTE[size];
    characters[0] = 0;
  }

  String(BYTE *chars) {
    size = length(chars);
    characters = new BYTE[size];
    DWORD_S i = size;
    while(--i >= 0) {
      characters[i] = chars[i];
    }
  }

  ~String() {
    delete [] characters;
  }

  static DWORD length(BYTE * chars ) {
    DWORD i = 0;
    while (chars[i++] != 0) {}
    return i;
  }

  DWORD length() {
    return size;
  }

  BYTE * getChars() {
    return characters;
  }

  void setChars(BYTE * chars) {
    delete [] characters;
    size = length(chars);
    characters = new BYTE[size];
    DWORD_S i = size;
    while(--i >= 0) {
      characters[i] = chars[i];
    }
  }
};

#endif /* _STRING_HPP_ */
