/*
   Copyright (C) 2007-2024 by krasnop@bellsouth.net (Alexei Krasnopolski)

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
#include "../../../Porting/ARM(Raspberry_Pi)/Include/bcm_registers.hpp"

extern Gpio *gpio47;

class MyAO : public AObject
{
 private:
  char outputString[80], outputString1[80];
  char receivedLine[80];
  Message *logMsg, *logMsg1;
  int second, counter, lineIdx;
  FormatParser fp;

 protected:
  virtual DWORD processMessage( Message * );

 public:
         MyAO( DWORD, DWORD );
  char * getString(){ return outputString; };
  virtual void log(BYTE level, char* text);
};

#endif /*MYAO_HPP_*/
