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
#include "os_cpu.hpp"

extern "C" void initDisplay();
extern "C" void putChar2VRAM(const char, int, int, const int, const int);

int baseAddress= 0xB8000;
int maxX = 80;
int maxY = 25;

void initDisplay() {
}

void putChar2VRAM(const char s, int x, int y, const int fgColor, const int bgColor) {
  BYTE  *pscr = (BYTE *) (baseAddress + (y * maxX + x) * 2);
  *pscr++ = s;                         // Put character in video RAM
  *pscr   = (BYTE) (fgColor | bgColor);              // Put video attribute in video RAM
};

