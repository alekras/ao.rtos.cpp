/*
 * uartAO.hpp
 *
 *  Created on: Oct 11, 2024
 *      Author: alexei
 */

#ifndef UARTAO_HPP_
#define UARTAO_HPP_

#include "../../../Porting/ARM(Raspberry_Pi)/Include/os_cpu.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm2835.hpp"
#include "ISAObject.hpp"
#include "application.hpp"

#define TXBUFMASK 0xFF
#define RXBUFMASK 0xFF

class UartAO : public ISAObject {
 private:
  volatile bool flag;
  RingBuffer<BYTE> *txRingBuffer;
  RingBuffer<BYTE> *rxRingBuffer;

  Message *logMsg, *comMsg;
  FormatParser fp;
  volatile int sec;

  Message *inMsg, *outMsg;

 protected:
  virtual DWORD processMessage(Message*);
  virtual AO_STACK * serviceInterrupt(AO_STACK *stp);
  void initUart();

 public:
          UartAO(DWORD);
};

#endif /* UARTAO_HPP_ */
