/*
   Copyright (C) 2007-2024 by krasnop@bellsouth.net (Alexei Krasnopolski)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "memory.hpp"
extern "C" void* new_req(int);
extern "C" void free_req(void *);

BYTE MemoryManager::memory[HEAP_MEMORY_SIZE];
MemoryManager* mm;

void* operator new(size_t sz) {
  return new_req(sz);
}

void operator delete(void* m) {
  free_req(m);
}

void* operator new[](size_t sz) {
  return new_req(sz);
}

void operator delete[](void* m) {
  free_req(m);
}

void *
MemoryManager::malloc(size_t sz) {
  WORD nbytes = (WORD) (sz + sizeof(MemoryControlBlock));
  MemoryControlBlock* mcb_location;
  BYTE *alloc = 0;
  BYTE *mem_location = start;
  while (mem_location < new_alloc) {
    mcb_location = (MemoryControlBlock*) mem_location;
    if (mcb_location->is_allocated == 0 && mcb_location->size >= nbytes) {
      mcb_location->is_allocated = 1;
      alloc = mem_location;
      break;
    }
    mem_location += mcb_location->size;
  }

  if (alloc == 0) {
    if (new_alloc + nbytes <= end) {
      alloc = new_alloc;
      new_alloc += nbytes;
      mcb_location = (MemoryControlBlock*) alloc;
      mcb_location->is_allocated = 1;
      mcb_location->size = nbytes;
      alloc += sizeof(MemoryControlBlock);
    }
  } else {
    alloc += sizeof(MemoryControlBlock);
  }

  return alloc;
};

void
MemoryManager::free(void *ptr) {
  if (ptr > start && ptr < end) {
    MemoryControlBlock *msb = (MemoryControlBlock*) ((BYTE*) ptr - sizeof(MemoryControlBlock));
    msb->is_allocated = 0;
  }
};
