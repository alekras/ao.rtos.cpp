/*
 * display.cpp
 *
 *  Created on: Aug 26, 2024
 *      Author: alexei
 */

#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"
#include "../../../Library/Display/Include/formatter.hpp"
#include "Include/MonospaceDefault.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/rpi-mailbox-interface.h"
extern "C" void RPI_PropertyInit( void );
extern "C" void RPI_PropertyAddTag( rpi_mailbox_tag_t tag, ... );
extern "C" int RPI_PropertyProcess( void );
extern "C" rpi_mailbox_property_t* RPI_PropertyGet( rpi_mailbox_tag_t tag );
extern "C" void sendString(char*);

volatile unsigned int * baseAddress = 0;

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 1024
#define SCREEN_DEPTH  32
#define FONT_WIDTH  16
#define FONT_WIDTH_IN_BYTES 2
#define FONT_HEIGHT 16

extern char out[80];
extern FormatParser fp;
extern int pt[];

void putChar2VRAM(const char s, int x, int y, const int fgColor, const int bgColor) {
    unsigned char *font = fontSet16x16[s - 32];
    volatile unsigned int *vbPntr = baseAddress;
    volatile unsigned int* rowPntr;
    unsigned char mask;
    vbPntr += y * SCREEN_WIDTH * FONT_HEIGHT + x * FONT_WIDTH;

    for (int row = 0, shift_down = 0; row < FONT_HEIGHT; row++, shift_down += SCREEN_WIDTH) {
      rowPntr = vbPntr + shift_down;
      for (int byteNumber = 0; byteNumber < FONT_WIDTH_IN_BYTES; byteNumber++) {
        mask = 0x01;
        for (int column = 0; column < 8; column++) {
          *(rowPntr++) = ((font[(row * 2) + byteNumber] & mask) ? fgColor : bgColor) | 0xFF000000;
          mask <<= 1;
        }
      }
    }
}

void initDisplay() {

  volatile unsigned int * vbPntr;
  unsigned int ra;
  rpi_mailbox_property_t *mp;

  /* Initialise a framebuffer using the property mailbox interface */
  fp.format(out, "Before Init() pt= %h, %h.\n\r", pt, &pt[0]);
  sendString(out);

  RPI_PropertyInit();

  fp.format(out, "After Init() pt= %h, %h.\n\r", pt, &pt[0]);
  sendString(out);

  RPI_PropertyAddTag( TAG_ALLOCATE_BUFFER );
  RPI_PropertyAddTag( TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT );
  RPI_PropertyAddTag( TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT * 2 );
  RPI_PropertyAddTag( TAG_SET_DEPTH, 32 );
  RPI_PropertyAddTag( TAG_GET_PITCH );
  RPI_PropertyAddTag( TAG_GET_PHYSICAL_SIZE );
  RPI_PropertyAddTag( TAG_GET_DEPTH );

  fp.format(out, "Before process() pt= %h, %h.\n\r", pt, &pt[0]);
  sendString(out);

  RPI_PropertyProcess();

  fp.format(out, "After process() pt= %h, %h.\n\r", pt, &pt[0]);
  sendString(out);

  if( ( mp = RPI_PropertyGet( TAG_GET_PHYSICAL_SIZE ) ) )
  {
      int width = mp->data.buffer_32[0];
      int height = mp->data.buffer_32[1];

      fp.format(out, "width= %d; height= %d.\n\r", width, height);
      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_GET_DEPTH ) ) )
  {
      int bpp = mp->data.buffer_32[0];
      fp.format(out, "depth= %d.\n\r", bpp);
      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_GET_PITCH ) ) )
  {
      int pitch_bytes = mp->data.buffer_32[0];
      fp.format(out, "pitch= %d.\n\r", pitch_bytes);
      sendString(out);
  }

  if( ( mp = RPI_PropertyGet( TAG_ALLOCATE_BUFFER ) ) )
  {
    baseAddress = (unsigned int *)(mp->data.buffer_32[0] & ~0xC0000000);
      fp.format(out, "Framebuffer address: %h.\n\r", (int)baseAddress);
      sendString(out);
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

  vbPntr = baseAddress;
  for (int i = 0; i < 50; i++) {
    putChar2VRAM((32 + i), i + 1, 17, 0xffffff, 0x00);
  }

}
