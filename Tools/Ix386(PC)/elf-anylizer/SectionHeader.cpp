using namespace std;
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <elf.h>

class SectionHeader
{
    char* names;
    int entryNum;
  public:
    Elf32_Shdr** hArray;
    SectionHeader( ELFHeader* e ){ entryNum = e->h->e_shnum; hArray = new Elf32_Shdr*[entryNum]; };
    SectionHeader(){ entryNum = 1; hArray = new Elf32_Shdr*[entryNum]; };
    void setNames( char* n ){ names = n; };
    ~SectionHeader(){ for(int i = 0; i < entryNum; i++) delete hArray[i]; delete hArray; };
    friend istream & operator >> ( istream & is, SectionHeader & that );
    friend ostream & operator << ( ostream & os, const SectionHeader & that );
};

istream & operator >> ( istream & is, SectionHeader & that )
{ Elf32_Shdr* tmp;
  for(int i = 0; i < that.entryNum; i++)
  { tmp = that.hArray[i] = new Elf32_Shdr;
    is.read( (char*)(tmp), sizeof(Elf32_Shdr) );
  }
  return is;
}

ostream & operator << ( ostream & os, const SectionHeader & that )
{
  Elf32_Shdr** p = that.hArray;
  os << "\n---- Section Header (Table of Sections) -------------------------------------------------\n"
     <<   " #                               flags  virtual   file                  Add.       Entr\n"
     <<   "ent          name           type I A W  address  offset    size   link  Info align size\n"
     <<   "-----------------------------------------------------------------------------------------\n";
  for(int i = 0; i < that.entryNum; i++)
  { Elf32_Word t = (*p[i]).sh_type;
    t = ( t > 100 )? 0xffff : t;
    os << setw(3) << dec << i << " ";
    os << setw(23) << &that.names[(*p[i]).sh_name] << " ";
    os << setw(4) <<  hex << t << " ";
    os << setw(1) << hex << ((0x4 & (*p[i]).sh_flags)>>2) << " "
       << setw(1) << ((0x2 & (*p[i]).sh_flags)>>1) << " "
       << setw(1) << (0x1 & (*p[i]).sh_flags) << " ";
    os << setw(8) << hex << (*p[i]).sh_addr << " ";
    os << setw(8) << hex << (*p[i]).sh_offset << " ";
    os << setw(8) << hex << (*p[i]).sh_size << " ";
    os << setw(5) << hex << (*p[i]).sh_link << " ";
    os << setw(5) << hex << (*p[i]).sh_info << " ";
    os << setw(4) << hex << (*p[i]).sh_addralign << " ";
    os << setw(5) << hex << (*p[i]).sh_entsize << endl;
  }

  return os;
}

