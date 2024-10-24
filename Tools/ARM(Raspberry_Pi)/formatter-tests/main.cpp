/*
 * main.cpp
 *
 */
using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "../../../Porting/Ix386/pc/formatter.cpp"

int main(int argc, char * argv[]) {
  if( argc != 2 ) {
   cout << "Usage : main <string>\n";
   return -1;
  }

  char out[80];
  FormatParser fp;
// test 1.
  char c = 'A';
  fp.format(out, "Test 1: char .%c.\n", c);
  cout << out;
// test 2.
  int d  = 1024, ud = -5;
  fp.format(out, "Test 2: char .%c. int .%d. hex .%h. unsigned .%u. \n", c, d, d, ud);
  cout << out;
// test 3.
  char * str  = "string";
  fp.format(out, "Test 3: char .%-3c. int .%-5d. hex .%-7h. unsigned .%-12u. string '%-20s'\n", c, d, d, ud, str);
  cout << out;
// test 4.
  fp.format(out, "Test 4: char .%3c. int .%5d. hex .%7h. unsigned .%12u. string '%20s'\n", c, d, d, ud, str);
  cout << out;
  return 0;
}

/*
 * run:
#g++ main.cpp -o main
#./main example
 */
