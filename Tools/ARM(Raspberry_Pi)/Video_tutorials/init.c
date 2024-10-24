/*
 * init.c
 *
 *  Created on: Aug 27, 2024
 *      Author: alexei
 */
#include "rpi-mailbox-interface.h"

static int pt[8192] __attribute__((aligned(16)));
static int pt_index = 0;

void init(void) {
/* Initialise a framebuffer... */
RPI_PropertyInit();
  pt[0] = 12;
  pt[1] = 0;
  pt_index = 2;
  pt[2] = 0;
RPI_PropertyAddTag( TAG_ALLOCATE_BUFFER );
  pt[2] = TAG_ALLOCATE_BUFFER;
  pt[3] = 8;
  pt[4] = 0; /* Request */
  pt[5] = 0; // ?? va_arg( vl, int );
  pt[6] = 0;
RPI_PropertyAddTag( TAG_SET_PHYSICAL_SIZE, 1280, 1024 );
  pt[7] = TAG_SET_PHYSICAL_SIZE;
  pt[8] = 8;
  pt[9] = 0; /* Request */
  pt[10] = 1280; /* Width */
  pt[11] = 1024; /* Height */
RPI_PropertyAddTag( TAG_SET_VIRTUAL_SIZE, 1280, 2048 );
  pt[12] = TAG_SET_VIRTUAL_SIZE;
  pt[13] = 8;
  pt[14] = 0; /* Request */
  pt[15] = 1280; /* Width */
  pt[16] = 2048; /* Height */
RPI_PropertyAddTag( TAG_SET_DEPTH, 32 );
  pt[17] = TAG_SET_DEPTH;
  pt[18] = 4;
  pt[19] = 0; /* Request */
  pt[20] = 32;
RPI_PropertyAddTag( TAG_GET_PITCH );
  pt[21] = TAG_GET_PITCH;
  pt[22] = 4;
  pt[23] = 0; /* Request */
  pt[24] = 0; /*skip*/
RPI_PropertyAddTag( TAG_GET_PHYSICAL_SIZE );
  pt[25] = TAG_GET_PHYSICAL_SIZE;
  pt[26] = 8;
  pt[27] = 0; /* Request */
  pt[28] = 0; /* skip */
  pt[29] = 0; /* skip */
RPI_PropertyAddTag( TAG_GET_DEPTH );
RPI_PropertyProcess();
  pt[0] = ( pt_index + 1 ) << 2; // pt_index = 21
  pt[1] = 0;

}
