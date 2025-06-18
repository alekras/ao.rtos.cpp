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
#include "Include/display_hw.hpp"
#include "Include/MonospaceDefault.hpp"
#include "../../../Library/Display/Include/formatter.hpp"
#include "Include/rpi-mailbox-interface.h"
extern "C" void RPI_PropertyInit( void );
extern "C" void RPI_PropertyAddTag( rpi_mailbox_tag_t tag, ... );
extern "C" int RPI_PropertyProcess( void );
extern "C" rpi_mailbox_property_t* RPI_PropertyGet( rpi_mailbox_tag_t tag );
//extern "C" void sendString(char*);

extern "C" void initDisplay();
extern "C" void putChar2VRAM(char, int, int, const int, const int);

unsigned int *baseAddress;

extern int pt[];

void initDisplay() {
  char out[80];
  FormatParser fp;
  volatile unsigned int * vbPntr;
  unsigned int ra;
  rpi_mailbox_property_t *mp;

  /* Initialise a framebuffer using the property mailbox interface */
  //fp.format(out, "Before Init() pt= %h, %h.\n\r", pt, &pt[0]);
  //sendString(out);

  RPI_PropertyInit();

  //fp.format(out, "After Init() pt= %h, %h.\n\r", pt, &pt[0]);
  //sendString(out);

  RPI_PropertyAddTag( TAG_ALLOCATE_BUFFER );
  RPI_PropertyAddTag( TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT );
  RPI_PropertyAddTag( TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT * 2 );
  RPI_PropertyAddTag( TAG_SET_DEPTH, 32 );
  RPI_PropertyAddTag( TAG_GET_PITCH );
  RPI_PropertyAddTag( TAG_GET_PHYSICAL_SIZE );
  RPI_PropertyAddTag( TAG_GET_DEPTH );

  //fp.format(out, "Before process() pt= %h, %h.\n\r", pt, &pt[0]);
  //sendString(out);

  RPI_PropertyProcess();

  //fp.format(out, "After process() pt= %h, %h.\n\r", pt, &pt[0]);
  //sendString(out);

  if( ( mp = RPI_PropertyGet( TAG_GET_PHYSICAL_SIZE ) ) )
  {
      int width = mp->data.buffer_32[0];
      int height = mp->data.buffer_32[1];

//      fp.format(out, "width= %d; height= %d.\n\r", width, height);
//      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_GET_DEPTH ) ) )
  {
      int bpp = mp->data.buffer_32[0];
//      fp.format(out, "depth= %d.\n\r", bpp);
//      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_GET_PITCH ) ) )
  {
      int pitch_bytes = mp->data.buffer_32[0];
//      fp.format(out, "pitch= %d.\n\r", pitch_bytes);
//      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_ALLOCATE_BUFFER ) ) )
  {
    baseAddress = (unsigned int *)(mp->data.buffer_32[0] & ~0xC0000000);
//      fp.format(out, "Framebuffer address: %h.\n\r", (int)baseAddress);
//      sendString(out);
  }

  vbPntr = baseAddress;
  for (ra = 0; ra < SCREEN_WIDTH; ra++) {
      (*vbPntr++) = (0xFFFFFFFF);
  }
  vbPntr = baseAddress + SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
  for (ra = 0; ra < SCREEN_WIDTH; ra++) {
      (*vbPntr++) = (0xFFFFFFFF);
  }
  vbPntr = baseAddress;
  for (ra = 0; ra < SCREEN_HEIGHT; ra++) {
      (*vbPntr) = (0xFFFFFFFF);
      vbPntr += SCREEN_WIDTH;
  }
  vbPntr = baseAddress + SCREEN_WIDTH - 1;
  for (ra = 0; ra < SCREEN_HEIGHT; ra++) {
      (*vbPntr) = (0xFFFFFFFF);
      vbPntr += SCREEN_WIDTH;
  }

  vbPntr = baseAddress;

  vbPntr += SCREEN_WIDTH * 20;
  int symb, column, row;
  for (symb = 0; symb < 90; symb++) {
    unsigned char *font = fontSet8x16[symb];
    for (row = 0; row < 16; row++) {
      int ja = row * SCREEN_WIDTH;
      unsigned char mask = 0x01;
      for (column = 0; column < 8; column++) {
        *(vbPntr + column + ja) = (font[row] & mask) ? 0xFFFFFFFF : 0xFF000000;
        mask <<= 1;
      }
    }
    vbPntr += 8;
  }

//  fp.format(out, "After Display Init() video buffer= %h.\n\r", baseAddress);
//  sendString(out);
  vbPntr = baseAddress;
  char * fpointer = out;
  int i = 1;
  while((*fpointer) != 0) {
    putChar2VRAM(*(fpointer++), i++, 17, 0xffffff, 0x00);
  }
}

void putChar2VRAM_original(const char s, int x, int y, const int fgColor, const int bgColor) {
    unsigned char *font = fontSet8x16[s - 32];
    unsigned int *rowPntr, *vbPntr = (unsigned int*) baseAddress;
    unsigned char mask;
    vbPntr += y * SCREEN_WIDTH * FONT_HEIGHT + x * FONT_WIDTH;
    for (int row = 0, shift_down = 0; row < 16; row++, shift_down += SCREEN_WIDTH) {
      mask = 0x01;
      rowPntr = vbPntr + shift_down;
      while (mask != 0) {
        *(rowPntr++) = ((font[row] & mask) ? fgColor : bgColor) | 0xFF000000;
        mask <<= 1;
      }
    }
}

void putChar2VRAM(const char s, int x, int y, const int fgColor, const int bgColor) {
    unsigned char *font = fontSet16x16[s - 32];
    volatile unsigned int *vbPntr = baseAddress;
    volatile unsigned int* rowPntr;
    unsigned char mask;
    vbPntr += y * SCREEN_WIDTH * FONT_HEIGHT + x * FONT_WIDTH;

    for (int row = 0, shift_down = 0; row < FONT_HEIGHT; row++, shift_down += SCREEN_WIDTH) {
      rowPntr = vbPntr + shift_down;
      for (int byteNumber = 0; byteNumber < FONT_WIDTH_IN_BYTES; byteNumber++) {
        mask = 0x80;
        for (int column = 0; column < 8; column++) {
          *(rowPntr++) = ((font[(row * 2) + byteNumber] & mask) ? fgColor : bgColor) | 0xFF000000;
          mask >>= 1;
        }
      }
    }
}
