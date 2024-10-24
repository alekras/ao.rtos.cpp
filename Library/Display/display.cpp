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

#include "Include/display.hpp"

extern "C" void initDisplay();
extern "C" void putChar2VRAM(const char, int, int, const int, const int);
//extern "C" void debugPrint(int, char);
//extern "C" void debugPrint(int, const char*);

void debugPrint(int pos, char s) {
  putChar2VRAM(s, pos, 0, ColorDef::FGND_LIGHT_GRAY, ColorDef::BGND_BLACK);
};

void debugPrint(int pos, const char* s) {
  while(*s != 0) {
    debugPrint(pos++, *s);
    s++;
  }
};

Display::Display() {
  initDisplay();
  setPosition( 0, 0 );
  maxX = SCREEN_COLUMNS;
  maxY = SCREEN_ROWS;
  setColor(ColorDef::FGND_WHITE, ColorDef::BGND_BLACK, ColorDef::NOBLINK);
};

void
Display::clearScreen(int ch) {
  int sx = x;
  int sy = y;
  for(int i = 0; i < (maxX * maxY); i++) {         // PC display has 80 columns and 25 lines
    print(ch);
  }
  x = sx;
  y = sy;
}

void
Display::clearColomn(int ch, int pX) {
  int sx = x;
  int sy = y;
  for(y = 0; y < maxY; y++) {
    x = pX;
    print(ch);
  }
  x = sx;
  y = sy;
}

void
Display::clearRow(int ch, int y) {
  int sx = x;
  int sy = y;
  for(x = 0; x < maxX; x++) {
    print(ch);
  }
  x = sx;
  y = sy;
}

void
Display::print(char s) {
  putChar2VRAM(s, x, y, frgColor, bcgColor);
  if(++x >= maxX) {
    x = 0;
    y++;
  };
}

void
Display::print(const char * str ) {
  char * s = (char*) str;
  while( (*s) != 0 ) {
    print( *s++ );
  }
}

void
Display::println(const char * str ) {
  char * s = (char*) str;
  while( (*s) != 0 ) {
    print( *s++ );
  }
  x = 0;
  y++;
}

void
Display::printf(const char * format, ... ) {
  char out[250];
  va_list ap;
  va_start(ap, format);
  FormatParser fp(out, ap);
  fp.init();

  while((! fp.isFinish()) && (! fp.isError())) {
    fp.dispatch(format++);
  }
  va_end(ap);
  print(out);
}

void
Display::sprintf(char * out, const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  FormatParser fp(out, ap);
  fp.init();

  while((! fp.isFinish()) && (! fp.isError())) {
    fp.dispatch(format++);
  }
  va_end(ap);
}
