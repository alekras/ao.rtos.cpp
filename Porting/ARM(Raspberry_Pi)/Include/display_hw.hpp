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

#ifndef _DISPLAY_HW_HPP
#define _DISPLAY_HW_HPP

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 1024
#define SCREEN_ROWS (SCREEN_HEIGHT / FONT_HEIGHT)
#define SCREEN_COLUMNS (SCREEN_WIDTH / FONT_WIDTH)
#define SCREEN_DEPTH  32
#define FONT_WIDTH  16
#define FONT_WIDTH_IN_BYTES 2
#define FONT_HEIGHT 16

class ColorDef {
  public:
  typedef enum fgColor
  { FGND_BLACK = 0x000000,
    FGND_BLUE  = 0x0000ff,
    FGND_GREEN = 0x008000,
    FGND_CYAN  = 0x00ffff,
    FGND_RED   = 0x8b0000,
    FGND_PURPLE = 0x800080,
    FGND_BROWN = 0xa52a2a,
    FGND_LIGHT_GRAY = 0xd3d3d3,
    FGND_DARK_GRAY = 0x808080,
    FGND_LIGHT_BLUE = 0xadd8e6,
    FGND_LIGHT_GREEN = 0x90ee90,
    FGND_LIGHT_CYAN = 0xe0ffff,
    FGND_LIGHT_RED = 0xff0000,
    FGND_LIGHT_PURPLE = 0xff00ff,
    FGND_YELLOW = 0xffff00,
    FGND_WHITE = 0xffffff
  } fgColor;

  typedef enum bgColor
  { BGND_BLACK = 0x000000,
    BGND_BLUE = 0x0000ff,
    BGND_GREEN = 0x008000,
    BGND_CYAN = 0x00ffff,
    BGND_RED = 0xff0000,
    BGND_PURPLE = 0x800080,
    BGND_BROWN = 0xa52a2a,
    BGND_LIGHT_GRAY = 0xd3d3d3
  } bgColor;

  typedef enum blink
  { BLINK = 0,
    NOBLINK = 0
  } blink;

};

#endif /* _DISPLAY_HW_HPP */
