using namespace std;
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <elf.h>

class ELFHeader
{
  public:
    Elf32_Ehdr * h;
    ELFHeader(){ h = new Elf32_Ehdr; };
    int test();
    ~ELFHeader(){ delete h; };
    friend istream & operator >> ( istream & is, ELFHeader & that );
    friend ostream & operator << ( ostream & os, const ELFHeader & that );
};

istream & operator >> ( istream & is, ELFHeader & that )
{ is.read( (char*)(that.h), sizeof(Elf32_Ehdr) );
  return is;
}

int ELFHeader::test()
{
  if( h->e_ident[0] == 0x7F && h->e_ident[1] == 'E' && h->e_ident[2] == 'L' && h->e_ident[3] == 'F' )
    return 0;
  return 1;
}

ostream & operator << ( ostream & os, const ELFHeader & that )
{
  Elf32_Ehdr * p = that.h;
  os << "\n---- ELF file Header --------------------------------------------------------------------\n"
     <<   " Identification:              | file        file   Entry    Pr hdr   Sec hdr       header \n"
     <<   " 0  1  2  3  4  5  6  7  8  9 | type  Arch  vers  Address   offset   offset  flags  size  \n"
     <<   "-----------------------------------------------------------------------------------------\n";
  os << hex << setw(2) << (int)(*p).e_ident[0] << " "
            << setw(2) << (char)(*p).e_ident[1] << " "
            << setw(2) << (char)(*p).e_ident[2] << " "
            << setw(2) << (char)(*p).e_ident[3] << " ";
  os << dec
     << setw(2) << (int)(*p).e_ident[4] << " "
     << setw(2) << (int)(*p).e_ident[5] << " "
     << setw(2) << (int)(*p).e_ident[6] << " "
     << setw(2) << (int)(*p).e_ident[7] << " "
     << setw(2) << (int)(*p).e_ident[8] << " "
     << setw(2) << (int)(*p).e_ident[9] << "  ";
  os << setw(5) << (*p).e_type << " ";
  os << setw(5) << (*p).e_machine << " ";
  os << setw(5) << (*p).e_version << " ";
  os << hex << setw(8) << (*p).e_entry << " ";
  os << hex << setw(8) << (*p).e_phoff << " ";
  os << hex << setw(8) << (*p).e_shoff << " ";
  os << dec << setw(6) << (*p).e_flags << " ";
  os << dec << setw(5) << (*p).e_ehsize << "\n\n";
  os << "---- ELF file Header (continue) --------------------------------------------------------\n"
     << " Pr hdr entry  Pr hdr entry  Sec hdr entry  Sec hdr entry  String table                 \n"
     << "     size         count           size          count         index                     \n"
     << "----------------------------------------------------------------------------------------\n";
  os << dec << setw(12) << (*p).e_phentsize << "  ";
  os << dec << setw(12) << (*p).e_phnum << "   ";
  os << dec << setw(12) << (*p).e_shentsize << "   ";
  os << dec << setw(12) << (*p).e_shnum << "  ";
  os << dec << setw(12) << (*p).e_shstrndx << "\n";

  return os;
}

