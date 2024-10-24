using namespace std;
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <elf.h>

#include "ELFHeader.cpp"

#include "ProgramHeader.cpp"

#include "SectionHeader.cpp"

int readFile( char * fileName )
{ ifstream inf( fileName, ios::in | ios::binary );

  if( inf.bad() )
  {
	 cout << "\nCan't find or open file " << fileName << ".\n";
	 return 1;
  }

  ELFHeader hd;

  inf >> hd;
  if( hd.test() == 1 )
  {  cout << "This is not ELF file.\n";
     return -1;
  }
  cout << hd;

  ProgramHeader ph( &hd );

  inf.seekg(0);
  inf.seekg( hd.h->e_phoff );
  if( sizeof( *(*ph.hArray) ) != hd.h->e_phentsize )
  { cout << "Can not process this format...\n";
    return -1;
  }
  inf >> ph;
  cout << ph;

// Reads names of sections
  SectionHeader sh;

  inf.seekg(0);
  inf.seekg( hd.h->e_shoff + hd.h->e_shentsize * hd.h->e_shstrndx );  // goes to position of a header for the name section
  inf >> sh;
  int sizeSec = sh.hArray[0]->sh_size;  // size of a name section

  char *nameBuffer = new char[sizeSec];

  inf.seekg(0);
  inf.seekg( sh.hArray[0]->sh_offset );  // goes to position of name section
  inf.read( nameBuffer, sizeSec );

  SectionHeader shh( &hd );
  shh.setNames( nameBuffer );
  inf.seekg(0);
  inf.seekg( hd.h->e_shoff );
  if( sizeof( *(*sh.hArray) ) != hd.h->e_shentsize )
  { cout << "Can not process this format...\n";
    return -1;
  }
  inf >> shh;
  cout << shh;

  return 0;
}

int main(int argc, char * argv[])
{ int result1, result2;
  char kb[7];
  if( argc != 2 )
  {
	 cout << "Usage : show_elf <elffile>\n";
	 return -1;
  }

  if ( readFile( argv[1] ) < 0 )
    return -1;

  cout << "The end\n";
  return 0;
}
