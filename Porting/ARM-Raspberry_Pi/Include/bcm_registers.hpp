/*
   Copyright (C) 2007-2025 by krasnop@bellsouth.net (Alexei Krasnopolski)

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

/*
 * BCM2835 ARM Peripherals
 * http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 * https://elinux.org/BCM2835_datasheet_errata
 * https://wiki.osdev.org/Raspberry_Pi_Bare_Bones
 * https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-1/#visual-output-and-running-code
 * https://github.com/BrianSidebotham/arm-tutorial-rpi/
 */

// RPI2 is defined in build.xml or in Makefile
#if defined( RPI2 )
    #define PERIPHERAL_BASE 0x3F000000
#elif defined( RPI3 )
    #define PERIPHERAL_BASE 0x3F000000
#elif defined( RPI4 )
    #define PERIPHERAL_BASE 0xFE000000
#else
    #define PERIPHERAL_BASE 0x20000000
#endif

#ifndef BCM_REGISTERS_HPP_
#define BCM_REGISTERS_HPP_

/************** INTERRUPTS *****************/
/** Basic IRQs pending status register */
extern volatile unsigned int* pPENDING_IRQ_B;
/** IRQ 1 pending status register */
extern volatile unsigned int* pPENDING_IRQ_1;
/** IRQ 2 pending status register */
extern volatile unsigned int* pPENDING_IRQ_2;
/** Enable IRQ 1 register */
extern volatile unsigned int* pENABLE_IRQ_1;
/** Enable IRQ 2 register */
extern volatile unsigned int* pENABLE_IRQ_2;
/** Enable Basic IRQs register */
extern volatile unsigned int* pENABLE_IRQ_B;
/** Disable IRQ 1 register */
extern volatile unsigned int* pDISABLE_IRQ_1;
/** Disable IRQ 2 register */
extern volatile unsigned int* pDISABLE_IRQ_2;
/** Disable Basic IRQs register */
extern volatile unsigned int* pDISABLE_IRQ_B;

/************** ARM TIMER *****************/
/** Load register (W) */
extern volatile unsigned int* pARM_TIMER_LD;
/** Value register (R) */
extern volatile unsigned int* pARM_TIMER_VAL;
/** Control (R/W) */
extern volatile unsigned int* pARM_TIMER_CTL;
/** IRQ Clear/Ack (W) */
extern volatile unsigned int* pARM_TIMER_IRQ;
/** RAW IRQ (R)*/
extern volatile unsigned int* pARM_TIMER_RAW_IRQ;
/** Masked IRQ (R) */
extern volatile unsigned int* pARM_TIMER_MSK_IRQ;
/** Reload (W) */
extern volatile unsigned int* pARM_TIMER_RLD;
/** Pre-divider (R/W) */
extern volatile unsigned int* pARM_TIMER_PDIV;
/** Free running counter (R) */
extern volatile unsigned int* pARM_TIMER_CNT;

/************** AUXILIARY PERIPHERAL *****************/
/** Auxiliary Interrupt status */
extern volatile unsigned int* pAUX_IRQ;
/** Auxiliary enables */
extern volatile unsigned int* pAUX_ENABLES;
/** Mini UART I/O Data */
extern volatile unsigned int* pAUX_MU_IO_REG;
/** Mini UART Interrupt Enable */
extern volatile unsigned int* pAUX_MU_IER_REG;
/** Mini UART Interrupt Identify (Interrupt status) */
extern volatile unsigned int* pAUX_MU_IIR_REG;
/** Mini UART Line Control */
extern volatile unsigned int* pAUX_MU_LCR_REG;
/** Mini UART Modem Control */
extern volatile unsigned int* pAUX_MU_MCR_REG;
/** Mini UART Line Status */
extern volatile unsigned int* pAUX_MU_LSR_REG;
/** Mini UART Modem Status */
extern volatile unsigned int* pAUX_MU_MSR_REG;
/** Mini UART Scratch */
extern volatile unsigned int* pAUX_MU_SCRATCH;
/** Mini UART Extra Control */
extern volatile unsigned int* pAUX_MU_CNTL_REG;
/** Mini UART Extra Status */
extern volatile unsigned int* pAUX_MU_STAT_REG;
/** Mini UART Baudrate */
extern volatile unsigned int* pAUX_MU_BAUD_REG;

/** System Timer Control/Status */
extern volatile unsigned int* pSYS_TIMER_CNTRL_STAT;
/** System Timer Counter Lower 32 bits */
extern volatile unsigned int* pSYS_TIMER_COUNT_LO;
/** System Timer Counter Higher 32 bits */
extern volatile unsigned int* pSYS_TIMER_COUNT_HI;
/** System Timer Compare 0 */
extern volatile unsigned int* pSYS_TIMER_CMP_0;
/** System Timer Compare 1 */
extern volatile unsigned int* pSYS_TIMER_CMP_1;
/** System Timer Compare 2 */
extern volatile unsigned int* pSYS_TIMER_CMP_2;
/** System Timer Compare 3 */
extern volatile unsigned int* pSYS_TIMER_CMP_3;

/** GPU addresses **/
extern volatile unsigned int* mailbox;

extern unsigned int exception_vectors;

#endif /* BCM_REGISTERS_HPP_ */
