/*
 * main.cpp
 *
 */
using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "Hex2BinEFSMachine.cpp"

int main(int argc, char * argv[]) {
  if( argc != 2 ) {
   cout << "Usage : main <hex file>\n";
   return -1;
  }

  char * fileName = argv[1];
  ifstream inf( fileName, ios::in | ios::binary );

  if( !inf.is_open() ) {
   cout << "\nCan't find or open file " << fileName << ".\n";
   return -1;
  }
  cout << "\nFile: " << fileName << " is open.\n";

  char c;
  Hex2BinEFSMachine fsm;
  do {
    inf.read(&c, 1);
//    cout << "input:" << hex << (int)c << "\n";
//    if (inf.eof()) break;
    if (fsm.dispatch(&c) > 0 ) break;
//    cout << c;
  } while (1);
  cout << "The end\n";
  return 0;
}

