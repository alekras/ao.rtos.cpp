using namespace std;
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <elf.h>

class ProgramHeader
{
    int entryNum;
  public:
    Elf32_Phdr** hArray;
    ProgramHeader( ELFHeader* e ){ entryNum = e->h->e_phnum; hArray = new Elf32_Phdr*[entryNum]; };
    char* pType( Elf32_Word t ) const;
    int getEntryCount() { return entryNum; };
    ~ProgramHeader(){ for(int i = 0; i < entryNum; i++) delete hArray[i]; delete hArray; };
    friend istream & operator >> ( istream & is, ProgramHeader & that );
    friend ostream & operator << ( ostream & os, const ProgramHeader & that );
};

char* ProgramHeader::pType( Elf32_Word t ) const
{
  switch( t )
  {
     case 0: return "unus";
     case 1: return "load";
     case 2: return "dyna";
     case 3: return "inte";
     case 4: return "note";
     case 5: return "shlb";
     case 6: return "phdr";
     case 7: return "ThLS";
     case 8: return "numb";
     default: return "proc";
  }
}

istream & operator >> ( istream & is, ProgramHeader & that )
{ Elf32_Phdr* tmp;
  for(int i = 0; i < that.entryNum; i++)
  { tmp = that.hArray[i] = new Elf32_Phdr;
    is.read( (char*)(tmp), sizeof(Elf32_Phdr) );
  }
  return is;
}

ostream & operator << ( ostream & os, const ProgramHeader & that )
{
  Elf32_Phdr** p = that.hArray;
  os << "\n---- Program Header (Table of Segments) -----------------------------------------------\n"
     <<   "  #           file   virtual physical                                       flags      \n"
     <<   "entry type   offset  address  address     size in file       size in mem.   E W R align\n"
     <<   "---------------------------------------------------------------------------------------\n";
  for(int i = 0; i < that.entryNum; i++)
  {
    os << setw(5) << dec << i << " ";
    os << setw(4) << (that.pType( (*p[i]).p_type )) << " ";
    os << setw(8) << hex << (*p[i]).p_offset << " ";
    os << setw(8) << hex << (*p[i]).p_vaddr << " ";
    os << setw(8) << hex << (*p[i]).p_paddr << " ";
    os << setw(8) << hex << (*p[i]).p_filesz << " ";
    os << setw(8) << dec << (*p[i]).p_filesz << ". ";
    os << setw(8) << hex << (*p[i]).p_memsz << " ";
    os << setw(8) << dec << (*p[i]).p_memsz << ". ";
    os << setw(1) << hex << ((0x4 & (*p[i]).p_flags)>>2) << " "
       << setw(1) << ((0x2 & (*p[i]).p_flags)>>1) << " "
       << setw(1) << (0x1 & (*p[i]).p_flags) << " ";
    os << setw(4) << hex << (*p[i]).p_align << endl;
  }

  return os;
}

