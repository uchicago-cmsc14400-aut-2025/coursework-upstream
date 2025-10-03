#include <stdio.h>
#include <stdlib.h>
#include "png-tools.h"

void usage()
{
  fprintf(stderr,"usage: png-map [FILENAME]\n");
  exit(1);
}

void show_chunk_list(chunk_list *chunks)
{
  if (chunks) {
    printf("[%u,%s,%u]->\n",chunks->start_byte,chunks->chunk_type,chunks->length);
    show_chunk_list(chunks->next);
  } else
    printf("end.\n");
}

void free_chunk_list(chunk_list *c)
{
  if (c) {
    free_chunk_list(c->next);
    free(c->chunk_type);
    free(c);
  }
}

int main(int argc, char *argv[])
{
  if (argc!=2)
    usage();
  char *filename = argv[1];
  FILE *f = fopen(filename,"r");
  if (!f) {
    fprintf(stderr,"\"%s\" file not found\n",filename);
    exit(1);
  }
  chunk_list *map = png_map(f);
  show_chunk_list(map);
  free_chunk_list(map);
  fclose(f);
  return 0;
}
