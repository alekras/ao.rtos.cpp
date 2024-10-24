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
};

#endif /* GPIO_HPP_ */
