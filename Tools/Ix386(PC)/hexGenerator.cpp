/* ------------------ E L F   O F F S E T S -------------------------------------
; offsets for variables in ELF header :
entry_address        EQU     0x18     ; 4
prog_hdr_offset         EQU     0x1C     ; 4
sect_hdr_offset         EQU     0x20     ; 4
header_size          EQU     0x28     ; 2
prog_hdr_entry_size  EQU     0x2A     ; 2
prog_hdr_entry_count EQU     0x2C     ; 2
sect_hdr_entry_size  EQU     0x2E     ; 2
sect_hdr_entry_count EQU     0x30     ; 2
sect_name_entr_index EQU     0x32     ; 2

; offsets for variables in a program segment headers
segment_type         EQU     0x0      ; 4
offset_in_file       EQU     0x4      ; 4
phys_address         EQU     0xC      ; 4
segm_size_in_file    EQU     0x10     ; 4

; offsets for variables in a section headers
section_name         EQU     0x0      ; 4
section_type         EQU     0x4      ; 4
section_flags        EQU     0x8      ; 4
section_address      EQU     0xC      ; 4
section_file_offset  EQU     0x10     ; 4
section_size         EQU     0x14     ; 4

temp_buffer          EQU  0x70000  ; keep application file in ELF befor we go to protected mode

; RELOCATION file.elf from temp buffer to original (physical) address space
; Keep in mind that temp buffer and physical addresses are both absolute addresses in flat memory (32bit PM)
               mov     ebx, [temp_buffer + prog_hdr_off]            ; program segment header pointer (offset value)
               add     ebx, temp_buffer                             ; absolute value
               movzx   ecx, word [temp_buffer + prog_hdr_entry_count] ; number of entries in program segment header
*/
  ifstream elfFile( fileName, ios::in | ios::binary );

  if( elfFile.bad() )
  {
	 cout << "\nCan't find or open file " << fileName << ".\n";
	 return 1;
  }

  ELFHeader hd;

  elfFile >> hd;
  if( hd.test() == 1 )
  {
  	cout << "This is not ELF file.\n";
     return -1;
  }

  ProgramHeader ph( &hd );

  elfFile.seekg(0);
  elfFile.seekg( hd.h->e_phoff );
  if( sizeof( *(*ph.hArray) ) != hd.h->e_phentsize )
  {
  	cout << "Can not process this format...\n";
    return -1;
  }
  elfFile >> ph;
  Elf32_Phdr** p = ph.hArray;
  for(int i = 0; i < ph.entryNum; i++)
  {
		if( (*p[i]).p_type == 1 )
		{
		  elfFile.seekg(0);
  		elfFile.seekg( (*p[i]).p_offset );
  		(*p[i]).p_paddr;
  		// convert address to HEX string and write to output stream;
  		for(int j = 0; j < (*p[i]).p_filesz; j++)
  		{
  			char s;
  			elfFile.read( &s, sizeof(char) );
  			// convert s to HEX string and write to output stream;
  		}
		}
  }

int * buffer = 0x70000;
int * pointer = buffer + *(buffer + prog_hdr_offset);
int nEntries = *(buffer + prog_hdr_entry_count);
for(int i = 0; i < nEntries; i++ )
{
	if(*pointer == 1)
	{
		int segment_size = *(pointer + segm_size_in_file);
		int * destination = *(pointer + phys_address);
		int * source = buffer + *(pointer + offset_in_file);
		for(int j = 0; j < segment_size; j++)
		{
			*(destination + j) = *(source + j);
		}
	}
	pointer += *(buffer + prog_hdr_entry_size);
}
/*
.lookup_in_segm:
               cmp     dword [ebx], 1                           ; is type of program segment "load" ?
               jne     .next                                    ; no - go to .next
; relocating of program segment from temp buffer to executable address space
               push    ecx
               cld
               mov     esi, [ebx + offset_in_file]       ; offset of code (program) segment in file
               add     esi, temp_buffer                  ; absolute value
               mov     edi, [ebx + phys_address]         ; physical address of the code segment in memory
               mov     ecx, [ebx + segm_size_in_file]    ; size of code segment
          rep  movsb
               pop     ecx
.next:
               movzx   eax, word [temp_buffer + prog_hdr_entry_size]  ; size of current entry in program header
               add     ebx, eax                                       ; point to the next entry
               loop    .lookup_in_segm                                ; go to process next entry
*/
int * pointer = buffer + *(buffer + sect_hdr_offset);
int entry_index = *(buffer + sect_name_entr_index);
int entry_size = *(buffer + sect_hdr_entry_size);
int * pointerToName = pointer + entry_index * entry_size;



/*
sections:
; point ebx to a section header
               mov     ebx, [temp_buffer + sect_hdr_off]
               add     ebx, temp_buffer
; point edx to the header entry of the name section
               mov     cx,  word [temp_buffer + sect_name_entr_index]
               movzx   eax, word [temp_buffer + sect_hdr_entry_size]
               imul    ax,  cx
               add     eax, ebx                         ; points to a name section header
; point edx to the name section body
               mov     edx, [eax + section_file_offset]
               add     edx, temp_buffer                 ; points to a body of name section

               movzx   ecx, word [temp_buffer + sect_hdr_entry_count] ; number of entries in section header
.lookup_in_sect:
               cmp     dword [ebx + section_type], 1                  ; is a type of the section "PROGBITS" ?
               jne     .next
; find a section with name = ".init"
               mov     esi, [ebx + section_name]											; index in name section body
               add     esi, edx
               mov     edi, initSect + A_7C00
               push    ecx

               mov     ecx, 5
         repe  cmpsb
               pop     ecx
               je      .find
.next:
               movzx   eax, word [temp_buffer + sect_hdr_entry_size]  ; size of current entry in section header
               add     ebx, eax                                       ; point to the next entry
               loop    .lookup_in_sect                                ; go to process next entry
               jmp     $																							; error: can not find .init section
.find:
; prepares to call .init (embedds a 'ret' instruction after end of the section)
               mov     edi, [ebx + section_address]
               add     edi, [ebx + section_size]             ; points to the section's end
               mov     eax, [edi]                            ; save a first dword of next section
               mov     [save + A_7C00], eax
               mov     eax, [retInst + A_7C00]
               mov     [edi], eax

               push    edi
               mov     esi, [ebx + section_address]
               call    esi									 ; call .init section ELF (on host it does underlying OS)									; call .init module
               pop     edi

               mov     eax,   [save + A_7C00]                 ; restore a first dword of next section
               mov     [edi], eax

;               mov     esi, [temp_buffer + entry_address]
;               mov     cx, 8
;               call    displayHex

               mov     esi, [temp_buffer + entry_address]     ; entry (start point of the application) address in elf header
               jmp     esi                                    ; GO TO AO-RTOS application !!!

initSect:      db      ".init"
save:          dd      0
retInst:       ret
*/
