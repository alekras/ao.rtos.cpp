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
#ifndef MYAO_HPP_
#define MYAO_HPP_

#include "Timer.hpp"
#include "AOScheduler.hpp"
#include "../../../Library/Display/Include/formatter.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"
#include "gpio.hpp"

class MyAO : public AObject
{
 private:
  String *outputString, *outputString1;
  char receivedLine[80];
  Message *logMsg, *commandMsg;
  int second, counter, lineIdx;
  Gpio *gpio10, *gpio22, *gpio47;
  FormatParser fp;

 protected:
  virtual DWORD processMessage( Message * );
  virtual void run();

 public:
         MyAO( DWORD, DWORD );
  virtual void log(BYTE level, char* text);
};

#endif /*MYAO_HPP_*/
