/*
   Copyright (C) 2007-2015 by krasnop@bellsouth.net (Alexei Krasnopolski)

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


#ifndef GPIO_HPP_
#define GPIO_HPP_

class Gpio {
  public:
    static volatile unsigned int* pGPFSEL;
    static volatile unsigned int* pGPSET;
    static volatile unsigned int* pGPCLR;
    static volatile unsigned int* pGPLEV;

    static volatile unsigned int* pGPEDS;
    static volatile unsigned int* pGPREN;
    static volatile unsigned int* pGPFEN;
    static volatile unsigned int* pGPHEN;
    static volatile unsigned int* pGPLEN;
    static volatile unsigned int* pGPAREN;
    static volatile unsigned int* pGPAFEN;

    static volatile unsigned int* pGPPUD;
    static volatile unsigned int* pGPPUDCLK;

  private:
    int shift1;
    int offset1;
    int offset2;
    int mask;

  public:
    Gpio(int n) {
      offset1 = n / 10;
      offset2 = n / 32;
      shift1 = (n % 10) * 3;
      mask = 1 << (n % 32);
    }

    inline void setFunction(int fn) {
      int a = *(pGPFSEL + offset1);
      a &= ~ (7 << shift1);
      *(pGPFSEL + offset1) = a | ((7 & fn) << shift1);
    };

    inline void setPullUpDown(int pullUpDown) {

      *pGPPUD = pullUpDown;
      for (int i = 0; i < 150; i++) {};
      *(pGPPUDCLK + offset2) = mask;
      for (int i = 0; i < 150; i++) {};
      *pGPPUD = 0;
      *(pGPPUDCLK + offset2) = 0;
    }

    inline void setLevel(void) {
      *(pGPSET + offset2) = mask;
    };

    inline void clearLevel(void) {
      *(pGPCLR + offset2) = mask;
    };

    inline int readLevel(void) {
      return ((*(pGPLEV + offset2) & mask) == 0)? 0 : 1;
    };

    inline int readEventDetectStatus(void) {
      return ((*(pGPEDS + offset2) & mask) == 0)? 0 : 1;
    }

    inline void clearEventDetectStatus(void) {
      *(pGPEDS + offset2) = mask;
    }

    inline void enableRisingEdgeDetect() {
      *(pGPREN + offset2) |= mask;
    }

    inline void disableRisingEdgeDetect() {
      *(pGPREN + offset2) &= !mask;
    }

    inline void enableFallingEdgeDetect() {
      *(pGPFEN + offset2) |= mask;
    }

    inline void disableFallingEdgeDetect() {
      *(pGPFEN + offset2) &= !mask;
    }

    inline void enableHighDetect() {
      *(pGPHEN + offset2) |= mask;
    }

    inline void disableHighDetect() {
      *(pGPHEN + offset2) &= !mask;
    }

    inline void enableLowDetect() {
      *(pGPLEN + offset2) |= mask;
    }

    inline void disableLowDetect() {
      *(pGPLEN + offset2) &= !mask;
    }

    inline void enableAsyncRisingEdgeDetect() {
      *(pGPAREN + offset2) |= mask;
    }

    inline void disableAsyncRisingEdgeDetect() {
      *(pGPAREN + offset2) &= !mask;
    }
    inline void enableAsyncFallingEdgeDetect() {
      *(pGPAFEN + offset2) |= mask;
    }

    inline void disableAsyncFallingEdgeDetect() {
      *(pGPAFEN + offset2) &= !mask;
    }
};

#endif /* GPIO_HPP_ */
