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
#ifndef _OS_CPU_HPP
#define _OS_CPU_HPP

#define cdecl
/*
***************************************************************************
*                          DATA TYPES
*                     (Compiler Specific)
***************************************************************************
*/

typedef unsigned char    BOOLEAN;
typedef unsigned char    BYTE;                     /* Unsigned  8 bit type */
typedef signed   char    BYTE_S;                   /* Signed    8 bit type */
typedef unsigned int     WORD;                     /* Unsigned 32 bit type */
typedef signed   int     WORD_S;                   /* Signed   32 bit type */
typedef unsigned int     DWORD;                    /* Unsigned 32 bit type */
typedef signed   int     DWORD_S;                  /* Signed   32 bit type */

typedef unsigned int     AO_STACK;                 /* Each stack entry is 32-bit wide */

/*
***************************************************************************
*                   ARM
*
* CPU interrupt enable/disable macros:
* Disable/Enable interrupts using simple instructions.
*
***************************************************************************
*/

extern "C" void enable_irq();
extern "C" void disable_irq();

/* Disable interrupts                        */
#define  ENTER_CRITICAL() asm("cpsid i"); //disable_irq();
/* Enable  interrupts                        */
#define  EXIT_CRITICAL() asm("cpsie i");  //enable_irq();

/*
*********************************************************************************************************
*                           ARM
* Context switching by using program swi interrupt
*********************************************************************************************************
*/
#define  AO_CONTEXT_SW() asm("svc #0");

#endif /* _OS_CPU_HPP */
