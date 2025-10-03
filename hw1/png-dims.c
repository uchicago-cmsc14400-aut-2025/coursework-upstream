#include <stdio.h>
#include <stdlib.h>
#include "png-tools.h"

void usage()
{
  fprintf(stderr,"usage: png-dims [FILENAME]\n");
  exit(1);
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
  struct dims *d = png_dimensions(f);
  if (d) {
    printf("width %u\nheight %u\n",d->width,d->height);
    free(d);
  }
  fclose(f);
  return 0;
}
