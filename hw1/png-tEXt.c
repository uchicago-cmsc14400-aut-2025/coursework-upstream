#include <stdio.h>
#include <stdlib.h>
#include "png-tools.h"

void usage()
{
  fprintf(stderr,"usage: png-tEXt [FILENAME] [INDEX]\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc!=3)
    usage();
  char *filename = argv[1];
  uint32_t i = atoi(argv[2]);
  FILE *f = fopen(filename,"r");
  if (!f) {
    fprintf(stderr,"\"%s\" file not found\n",filename);
    exit(1);
  }
  char *t = extract_tEXt(f,i);
  if (t) {
    printf("%s\n",t);
    free(t);
  }
  fclose(f);
  return 0;
}
