#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "p4machine.h"

void TODO(char *thing)
{
  fprintf(stderr, "TODO: %s\n", thing);
  exit(1);
}

p4heap *p4heap_create()
{
  TODO("p4heap_create");
}

uint8_t p4heap_num_allocs(p4heap *h)
{
  TODO("p4heap_num_allocs");
}

void *p4malloc(p4heap *h, uint16_t num_bytes, uint8_t num_pointers)
{
  TODO("p4malloc");
}

void *p4calloc(p4heap *h, uint16_t count, uint16_t size, uint8_t num_pointers)
{
  TODO("p4calloc");
}

free_list *infer_free_list(p4heap *h)
{
  TODO("infer_free_list");
}

void p4free(p4heap *h, void *addr)
{
  TODO("p4free");
}

uint8_t p4gc(p4heap *h, address_list *live_roots)
{
  TODO("p4gc");
}

void allocation_list_show(p4heap *h)
{
  TODO("allocation_list_show");
}
