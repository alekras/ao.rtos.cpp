#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <stddef.h>
#include "commonDef.hpp"
//#include "display.hpp"
#include "os_cpu.hpp"

class MemoryManager {
  private:
    BYTE *start,     // pointer to beginning of dynamic memory area.
         *new_alloc, // pointer to memory ready for allocation.
         *end;       // pointer to end of available memory.
    static BYTE memory[HEAP_MEMORY_SIZE];
  public:
           MemoryManager() { start = new_alloc = memory; end = start + HEAP_MEMORY_SIZE; };
    void * malloc(size_t sz);
    void   free(void*);
    struct MemoryControlBlock {
      WORD is_allocated;
      WORD size;
    };
    struct MemoryStatistics {
      BYTE *start;
      BYTE *end;
      BYTE *available;
      WORD blocks;
      WORD allocatedBlocks;
    };
    void getStatistics(MemoryStatistics *);
};

#endif /* _MEMORY_HPP_ */
