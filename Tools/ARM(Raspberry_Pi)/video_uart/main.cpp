
#include "../../../Library/Display/Include/formatter.hpp"
#include "MonospaceDefault.hpp"

extern "C" int notmain();
extern "C" void MailboxWrite(unsigned int, unsigned int);
extern "C" unsigned int MailboxRead(unsigned int);

extern "C" void uart_init();
extern "C" unsigned int uart_recv();

extern "C" void sendString(char*);

volatile unsigned int* mailbox = (unsigned int*) 0x3F00B880;
volatile unsigned int* fbinfo_addr = (unsigned int*) 0x40040000;
unsigned int baseAddress= 0;
int maxX = 80;
int maxXpx = maxX * 8;
int maxY = 25;

char out[80];
FormatParser fp;

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

void putChar2VRAM(const char s, int x, int y, const int fgColor, const int bgColor) {
    unsigned char *font = fontSet[s];
    unsigned int *rowPntr, *vbPntr = (unsigned int*) baseAddress;
    unsigned char mask;
    vbPntr += y * maxXpx * 16 + x * 8;

    for (int row = 0, shift_down = 0; row < 16; row++, shift_down += maxXpx) {
      mask = 0x01;
      unsigned int* rowPntr = vbPntr + shift_down;
      while (mask != 0) {
        *(rowPntr++) = ((font[row] & mask) ? fgColor : bgColor) | 0xFF000000;
        mask <<= 1;
      }
    }
}

int notmain() {
  maxX = 80;
  maxXpx = maxX * 8;
  maxY = 25;

  unsigned int ra, *vbPntr;

  *(fbinfo_addr)     = 640; /* #0 Physical Width */
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
  unsigned int va = MailboxRead(1);
  fp.format(out, "video channel %h.\n\r", va);
  sendString(out);
  unsigned int* rb = (unsigned int*) fbinfo_addr;
  for (ra = 0; ra < 10; ra++) {
    fp.format(out, "(%h) := %h.\n\r", rb, *rb);
    rb++;
    sendString(out);
  }

  vbPntr = (unsigned int*) (*(fbinfo_addr + 8));
  baseAddress = *(fbinfo_addr + 8);

  fp.format(out, "video buffer pointer %h.\n\r", vbPntr);
  sendString(out);
  fp.format(out, "video buffer pointer %h.\n\r", baseAddress);
  sendString(out);

  for (ra = 0; ra < 640 * 8; ra++) {
      (*vbPntr++) = (0xFF0000FF);
  }

  for (ra = 0; ra < 640 * 8; ra++) {
    (*vbPntr++) = (0xFF00FF00);
  }

  for (ra = 0; ra < 640 * 8; ra++) {
    (*vbPntr++) = (0xFFFF0000);
  }

  for (ra = 0; ra < 640 * 8; ra++) {
    (*vbPntr++) = (0xFF000000);
  }
  vbPntr += 640 * 20;
  for (ra = 0; ra < 640; ra++) {
    (*vbPntr++) = 0xFFFFFFFF;
  }

  vbPntr += 640 * 20;
  int symb, i, j;
  for (symb = 32; symb < 32 + 80; symb++) {
    unsigned char *font = fontSet[symb];
    for (j = 0; j < 16; j++) {
      int ja = j * 640;
      unsigned char mask = 0x01;
      for (i = 0; i < 8; i++) {
        *(vbPntr + i + ja) = (font[j] & mask) ? 0xFF000000 : 0xFFF0F0F0;
        mask <<= 1;
      }
    }
    vbPntr += 8;
  }

  for (i = 0; i < 80; i++) {
    putChar2VRAM((32 + i), i, 17, 0x008080, 0xf8f8f8);
  }
/**
 *   Base point                            font set
 *    |
 * 0 [0] [1] [2] [3] [4] [5] [6] [7]  ----> [7,6,5,4,3,2,1,0]
 *    |                           |__________|             |
 *    |____________________________________________________|
 * 1 [+640] ....
 * 2 [+1280]
 *
 *
 *15 [+640*15]
 */
  return 0;
}
