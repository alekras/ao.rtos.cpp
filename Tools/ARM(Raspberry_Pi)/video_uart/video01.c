
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner
// 4
// 6
// 8  TX out
// 10 RX in

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "image_data.h"

//extern void PUT32 ( unsigned int, unsigned int );
//extern void PUT16 ( unsigned int, unsigned int );
//extern void PUT8 ( unsigned int, unsigned int );
//extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
//extern void BRANCHTO ( unsigned int );
//extern void dummy ( unsigned int );

extern void uart_init(void);
extern unsigned int uart_lcr(void);
extern void uart_flush(void);
extern void uart_send(unsigned int);
extern unsigned int uart_recv(void);
extern unsigned int uart_check(void);
extern void hexstring(unsigned int);
extern void hexstrings(unsigned int);
extern void timer_init(void);
extern unsigned int timer_tick(void);

volatile unsigned int* mailbox = (unsigned int*) 0x2000B880;
volatile unsigned int* fbinfo_addr = (unsigned int*) 0x40040000;

void MailboxWrite(unsigned int fbinfo_addr, unsigned int channel) {
  while (((*(mailbox + 6)) & 0x80000000) != 0) {}
  *(mailbox + 8) = fbinfo_addr + channel;
}

unsigned int MailboxRead(unsigned int channel) {
  unsigned int ra;
  do {
    while (((*(mailbox + 6)) & 0x40000000) != 0);
    ra = *mailbox;
  } while ((ra & 0xF) != channel);

  return ra;
}

//------------------------------------------------------------------------
int notmain ( void ) {
  unsigned int ra, *rb;
  unsigned int ry, rx;

  uart_init();
  hexstring(0x12345678);
  hexstring(GETPC());
  timer_init();

  *(fbinfo_addr)        = 640; /* #0 Physical Width */
  *(fbinfo_addr + 1) = 480; /* #4 Physical Height */
  *(fbinfo_addr + 2) = 640; /* #8 Virtual Width */
  *(fbinfo_addr + 3) = 480; /* #12 Virtual Height */
  *(fbinfo_addr + 4) =   0; /* #16 GPU - Pitch */
  *(fbinfo_addr + 5) =  32; /* #20 Bit Depth */
  *(fbinfo_addr + 6) =   0; /* #24 X */
  *(fbinfo_addr + 7) =   0; /* #28 Y */
  *(fbinfo_addr + 8) =   0; /* #32 GPU - Pointer */
  *(fbinfo_addr + 9) =   0; /* #36 GPU - Size */


  MailboxWrite((unsigned int) fbinfo_addr, 1);
  hexstring(MailboxRead(1));
  rb = (unsigned int*) fbinfo_addr;
  for (ra = 0; ra < 10; ra++) {
    hexstrings((unsigned int) rb); hexstring(*rb);
    rb += 1;
  }

  rb = (unsigned int*) (*(fbinfo_addr + 8));
  hexstring((unsigned int) rb);

  for (ra = 0; ra < 10000; ra++) {
      (*rb) = ~(ra & 0xFF);
      rb += 1;
  }

  for (ra = 0; ra < 10000; ra++) {
    (*rb) = ~((ra & 0xFF) << 8);
      rb += 1;
  }

  for (ra = 0; ra < 10000; ra++) {
    (*rb) = ~((ra & 0xFF) << 16);
    rb += 1;
  }

  for(ra=0;ra<10000;ra++) {
    (*rb) = ~((ra & 0xFF) << 24);
      rb += 1;
  }

  rb = (unsigned int*) (*(fbinfo_addr + 8));
  hexstring((unsigned int) rb);
  ra=0;
  for (ry = 0; ry < 480; ry++) {
    for (rx = 0; rx < 480; rx++) {
      (*rb) = image_data[ra++];
      rb += 1;
    }
    for (; rx < 640; rx++) {
      (*rb) = 0;
      rb += 1;
    }
  }

  return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
