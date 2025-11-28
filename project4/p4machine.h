#ifndef __P4MACHINE_H__
#define __P4MACHINE_H__
#include <stdint.h>

// ------ constants (do not edit) ------

#define P4HEAP_TOTAL_SIZE    65536
#define P4HEAP_HIGH_ADDR     65535
#define P4HEAP_SCRATCH_START   128
#define P4HEAP_ALLOC_START    1024
#define P4HEAP_DYNAMIC_START  2048

// ------ data definitions (do not edit) ------

struct offset_list {
  uint16_t offset;
  struct offset_list *next;
};

struct address_list {
  void *addr;
  struct address_list *next;
};

struct free_list {
  uint16_t start;
  uint16_t size;
  struct free_list *next;
};
  
struct p4heap {
  unsigned char memory[P4HEAP_TOTAL_SIZE];
};

// ------ synonyms (do not edit) ------

typedef struct offset_list offset_list;
typedef struct address_list address_list;
typedef struct free_list free_list;
typedef struct p4heap p4heap;

// ------ custom definitions? ------

// If you want to add any data definitions, constants, etc., please go
// ahead and do so, but do change or delete any of the given
// definitions.

// ------ operations (do not edit) ------

// create a new heap and zero it out
p4heap *p4heap_create();

// report the number of allocations in the p4heap
uint8_t p4heap_num_allocs(p4heap *h);

// allocate object of given size if possible
// allocate it in the lowest available location in the dynamic heap
// pass the number of pointers on the honors system
// all pointers must be at the _front_ of the allocated object
// return NULL if allocation is not possible
// note this function returns a void* (useable at large) and not an offset
void *p4malloc(p4heap *h, uint16_t num_bytes, uint8_t num_pointers);

// allocate object, like malloc, and set all bytes to 0
// return NULL if allocation is not possible
void *p4calloc(p4heap *h, uint16_t count, uint16_t size, uint8_t num_pointers);

// infer free list from the allocation list
free_list *infer_free_list(p4heap *h);

// free the object at the given address
// take no action on NULL
// if there is no object at the address, GIGO
void p4free(p4heap *h, void *addr);

// starting from the given live root set, infer garbage objects and free them
// return the number of freed objects
uint8_t p4gc(p4heap *h, address_list *live_roots);

// the following is for debugging and will not be scored by us
// (but you will need it)
void allocation_list_show(p4heap *h);

// ------ custom operations? ------

// add headers for more operations here if you choose

#endif
