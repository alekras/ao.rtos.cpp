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
 * https://github.com/BrianSidebotham/arm-tutorial-rpi/
 */

// RPI2 is defined in build.xml
#ifdef RPI2
    #define PERIPHERAL_BASE 0x3F000000
#else
    #define PERIPHERAL_BASE 0x20000000
#endif

#include "gpio.hpp"

#ifndef BCM2835_HPP_
#define BCM2835_HPP_

/** GPIO Function select registers 0-5 (00...14) */
volatile unsigned int* Gpio::pGPFSEL = (unsigned int*) (PERIPHERAL_BASE + 0x200000);
/** GPIO Pin output set registers 0-1 (1c,20) */
volatile unsigned int* Gpio::pGPSET =  (unsigned int*) (PERIPHERAL_BASE + 0x20001C);
/** GPIO Pin output clear registers 0-1 (28,2C)*/
volatile unsigned int* Gpio::pGPCLR =  (unsigned int*) (PERIPHERAL_BASE + 0x200028);
/** GPIO Pin level registers 0-1 (34,38) */
volatile unsigned int* Gpio::pGPLEV =  (unsigned int*) (PERIPHERAL_BASE + 0x200034);
/** GPIO Pin event detect status registers 0-1 (40,44) */
volatile unsigned int* Gpio::pGPEDS =  (unsigned int*) (PERIPHERAL_BASE + 0x200040);
/** GPIO Pin rising edge detect Enable registers 0-1 (4C,50) */
volatile unsigned int* Gpio::pGPREN =  (unsigned int*) (PERIPHERAL_BASE + 0x20004C);
/** GPIO Pin falling edge detect Enable registers 0-1 (58,5C) */
volatile unsigned int* Gpio::pGPFEN =  (unsigned int*) (PERIPHERAL_BASE + 0x200058);
/** GPIO Pin High detect Enable registers 0-1 (64,68) */
volatile unsigned int* Gpio::pGPHEN =  (unsigned int*) (PERIPHERAL_BASE + 0x200064);
/** GPIO Pin Low detect Enable registers 0-1 (70,74) */
volatile unsigned int* Gpio::pGPLEN =  (unsigned int*) (PERIPHERAL_BASE + 0x200070);
/** GPIO Pin Async rising edge detect Enable registers 0-1 (7C,80) */
volatile unsigned int* Gpio::pGPAREN =  (unsigned int*) (PERIPHERAL_BASE + 0x20007C);
/** GPIO Pin Async falling edge detect Enable registers 0-1 */
volatile unsigned int* Gpio::pGPAFEN =  (unsigned int*) (PERIPHERAL_BASE + 0x200088);
/** GPIO Pin Pull-up/down Enable register */
volatile unsigned int* Gpio::pGPPUD =  (unsigned int*) (PERIPHERAL_BASE + 0x200094);
/** GPIO Pin Pull-up/down Enable clock registers 0-1 (98, 9C)*/
volatile unsigned int* Gpio::pGPPUDCLK = (unsigned int*) (PERIPHERAL_BASE + 0x200098);

/************** INTERRUPTS *****************/
/** Basic IRQs pending status register */
volatile unsigned int* pPENDING_IRQ_B = (unsigned int*) (PERIPHERAL_BASE + 0xB200);
/** IRQ 1 pending status register */
volatile unsigned int* pPENDING_IRQ_1 = (unsigned int*) (PERIPHERAL_BASE + 0xB204);
/** IRQ 2 pending status register */
volatile unsigned int* pPENDING_IRQ_2 = (unsigned int*) (PERIPHERAL_BASE + 0xB208);

/** Enable IRQ 1 register */
volatile unsigned int* pENABLE_IRQ_1 =  (unsigned int*) (PERIPHERAL_BASE + 0xB210);
/** Enable IRQ 2 register */
volatile unsigned int* pENABLE_IRQ_2 =  (unsigned int*) (PERIPHERAL_BASE + 0xB214);
/** Enable Basic IRQs register */
volatile unsigned int* pENABLE_IRQ_B =  (unsigned int*) (PERIPHERAL_BASE + 0xB218);

/** Disable IRQ 1 register */
volatile unsigned int* pDISABLE_IRQ_1 = (unsigned int*) (PERIPHERAL_BASE + 0xB21C);
/** Disable IRQ 2 register */
volatile unsigned int* pDISABLE_IRQ_2 = (unsigned int*) (PERIPHERAL_BASE + 0xB220);
/** Disable Basic IRQs register */
volatile unsigned int* pDISABLE_IRQ_B = (unsigned int*) (PERIPHERAL_BASE + 0xB224);

/************** ARM TIMER *****************/
/** Load register (W) */
volatile unsigned int* pARM_TIMER_LD =  (unsigned int*) (PERIPHERAL_BASE + 0xB400);
/** Value register (R) */
volatile unsigned int* pARM_TIMER_VAL = (unsigned int*) (PERIPHERAL_BASE + 0xB404);
/** Control (R/W) */
volatile unsigned int* pARM_TIMER_CTL = (unsigned int*) (PERIPHERAL_BASE + 0xB408);
/** IRQ Clear/Ack (W) */
volatile unsigned int* pARM_TIMER_IRQ = (unsigned int*) (PERIPHERAL_BASE + 0xB40C);
/** RAW IRQ (R)*/
volatile unsigned int* pARM_TIMER_RAW_IRQ = (unsigned int*) (PERIPHERAL_BASE + 0xB410);
/** Masked IRQ (R) */
volatile unsigned int* pARM_TIMER_MSK_IRQ = (unsigned int*) (PERIPHERAL_BASE + 0xB414);
/** Reload (W) */
volatile unsigned int* pARM_TIMER_RLD =  (unsigned int*) (PERIPHERAL_BASE + 0xB418);
/** Pre-divider (R/W) */
volatile unsigned int* pARM_TIMER_PDIV = (unsigned int*) (PERIPHERAL_BASE + 0xB41C);
/** Free running counter (R) */
volatile unsigned int* pARM_TIMER_CNT =  (unsigned int*) (PERIPHERAL_BASE + 0xB420);

/************** AUXILIARY PERIPHERAL *****************/
/** Auxiliary Interrupt status */
volatile unsigned int* pAUX_IRQ        = (unsigned int*) (PERIPHERAL_BASE + 0x215000);
/** Auxiliary enables */
volatile unsigned int* pAUX_ENABLES    = (unsigned int*) (PERIPHERAL_BASE + 0x215004);
/** Mini UART I/O Data */
volatile unsigned int* pAUX_MU_IO_REG  = (unsigned int*) (PERIPHERAL_BASE + 0x215040);
/** Mini UART Interrupt Enable */
volatile unsigned int* pAUX_MU_IER_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215044);
/** Mini UART Interrupt Identify (Interrupt status) */
volatile unsigned int* pAUX_MU_IIR_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215048);
/** Mini UART Line Control */
volatile unsigned int* pAUX_MU_LCR_REG = (unsigned int*) (PERIPHERAL_BASE + 0x21504C);
/** Mini UART Modem Control */
volatile unsigned int* pAUX_MU_MCR_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215050);
/** Mini UART Line Status */
volatile unsigned int* pAUX_MU_LSR_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215054);
/** Mini UART Modem Status */
volatile unsigned int* pAUX_MU_MSR_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215058);
/** Mini UART Scratch */
volatile unsigned int* pAUX_MU_SCRATCH = (unsigned int*) (PERIPHERAL_BASE + 0x21505C);
/** Mini UART Extra Control */
volatile unsigned int* pAUX_MU_CNTL_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215060);
/** Mini UART Extra Status */
volatile unsigned int* pAUX_MU_STAT_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215064);
/** Mini UART Baudrate */
volatile unsigned int* pAUX_MU_BAUD_REG = (unsigned int*) (PERIPHERAL_BASE + 0x215068);

/** System Timer Control/Status */
volatile unsigned int* pSYS_TIMER_CNTRL_STAT = (unsigned int*) (PERIPHERAL_BASE + 0x3000);
/** System Timer Counter Lower 32 bits */
volatile unsigned int* pSYS_TIMER_COUNT_LO =   (unsigned int*) (PERIPHERAL_BASE + 0x3004);
/** System Timer Counter Higher 32 bits */
volatile unsigned int* pSYS_TIMER_COUNT_HI =   (unsigned int*) (PERIPHERAL_BASE + 0x3008);
/** System Timer Compare 0 */
volatile unsigned int* pSYS_TIMER_CMP_0 =      (unsigned int*) (PERIPHERAL_BASE + 0x300C);
/** System Timer Compare 1 */
volatile unsigned int* pSYS_TIMER_CMP_1 =      (unsigned int*) (PERIPHERAL_BASE + 0x3010);
/** System Timer Compare 2 */
volatile unsigned int* pSYS_TIMER_CMP_2 =      (unsigned int*) (PERIPHERAL_BASE + 0x3014);
/** System Timer Compare 3 */
volatile unsigned int* pSYS_TIMER_CMP_3 =      (unsigned int*) (PERIPHERAL_BASE + 0x3018);

/** GPU addresses **/
volatile unsigned int* mailbox = (unsigned int*) (PERIPHERAL_BASE + 0xB880);

#endif /* BCM2835_HPP_ */
