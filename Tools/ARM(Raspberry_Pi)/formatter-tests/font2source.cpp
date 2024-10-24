/*
 * font2source.cpp
 *
 *  Created on: Sep 22, 2014
 *      Author: alexeikrasnopolski
 */
using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char * argv[]) {
  if (argc != 3) {
    cout << "Usage : main <font bin file> <new .hpp file>\n";
    return -1;
  }

  char * fileName = argv[1];
  ifstream inf(fileName, ios::in | ios::binary);
  ofstream out(argv[2], ios::out);

  if (!inf.is_open()) {
    cout << "\nCan't find or open file " << fileName << ".\n";
    return -1;
  }
  cout << "\nFile: " << fileName << " is open.\n";

  unsigned char c;
  cout << "static unsigned char fontSet[][16] = {\n";
  out  << "static unsigned char fontSet[][16] = {\n";
  int j = 0;
  do {
    cout << "{";
    out  << "{";
    char *dlm = "";
    for (int i = 0; i < 16; i++) {
      inf.read((char *)&c, 1);
      cout << dlm << "0x" << setfill('0') << hex << setw(2) << (unsigned int)c;
      out  << dlm << "0x" << setfill('0') << hex << setw(2) << (unsigned int)c;
      dlm = ", ";
    }
    cout << "}" << ((j == 128) ? " " : ",") << "  // '" << (char)((j >= 32) ? j : 0) << "' ASCII code = " << dec << j << " (0x" << hex << j << ")\n";
    out  << "}" << ((j == 128) ? " " : ",") << "  // '" << (char)((j >= 32) ? j : 0) << "' ASCII code = " << dec << j << " (0x" << hex << j << ")\n";
    j++;
    if (inf.eof()) break;
  } while (1);
  cout << "};\n";
  out  << "};\n";
  cout << "The end\n";
  return 0;
}
  /**
   * vi :
   * :%!xxd
   * :%!xxd -r
   */
