/*
   Copyright (C) 2010-2025 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP
#include "formatter.hpp"
#include "display_hw.hpp"

void debugPrint(int, char);
void debugPrint(int, const char*);

class Display {
  private:
    int x, y;
    int maxX, maxY;
    int frgColor;
    int bcgColor;

  public:
    Display();
    void  clearScreen( int ch );
    void  clearColomn( int ch, int x );
    void  clearRow( int ch, int y );
    void  setColor( ColorDef::fgColor c1, ColorDef::bgColor c2, ColorDef::blink blk = ColorDef::NOBLINK ) {
      frgColor = c1 | blk;
      bcgColor = c2;
    };
    void  setPosition( int coln, int row ) {
      y = row;
      x = coln;
    };
    void  print( char s );
    void  print( const char * );
    void  println( const char * );
    void  print( int x, int y, char s ) {
      setPosition( x, y );
      print( s );
    };
    void  print( int x, int y, const char *s ) {
      setPosition( x, y );
      print( s );
    };
    void  printf(const char * f, ... );
    static void  sprintf( char *out, const char * f, ... );
};

#endif /* _DISPLAY_HPP */
