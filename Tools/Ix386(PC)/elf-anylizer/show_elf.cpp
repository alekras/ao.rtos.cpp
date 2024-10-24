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

class ProgramHeader
{
    int entryNum;
  public:
    Elf32_Phdr** hArray;
    ProgramHeader( ELFHeader* e ){ entryNum = e->h->e_phnum; hArray = new Elf32_Phdr*[entryNum]; };
    char* pType( Elf32_Word t ) const;
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




