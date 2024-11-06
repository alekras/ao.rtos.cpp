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
  volatile DWORD txhead;
  volatile DWORD txtail;
  volatile unsigned char txbuffer[TXBUFMASK+1];

  volatile DWORD rxhead;
  volatile DWORD rxtail;
  volatile unsigned char rxbuffer[RXBUFMASK+1];

  char outputString[80];
  Message *logMsg;
  FormatParser fp;
  volatile int sec;

  BYTE receivedSymbol[2];
  Message msg, *outMsg;

 protected:
  virtual void run();
  virtual DWORD processMessage(Message*);
  virtual AO_STACK * serviceInterrupt(AO_STACK *stp);
  void initUart();

 public:
          UartAO(DWORD);
};

#endif /* UARTAO_HPP_ */
