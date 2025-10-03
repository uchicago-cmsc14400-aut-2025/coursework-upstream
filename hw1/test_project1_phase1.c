#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "png-tools.h"

// ======== tests

char *path_to_file = "./test_project1_phase1.c";

Test(project1_png_signature, png_signature00)
{
  FILE *f = fopen(path_to_file,"r");
  cr_assert(png_signature(f)==0);
  fclose(f);
}

Test(project1_read_int, read_int00)
{
  FILE *f = fopen(path_to_file,"r");
  cr_assert(read_int(f)==594112099);
  fclose(f);
}

Test(project1_read_chunk_tag, read_chunk_tag00)
{
  FILE *f = fopen(path_to_file,"r");
  char *t = read_chunk_tag(f);
  cr_assert(!strcmp(t,"#inc"));
  free(t);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions00)
{
  FILE *f = fopen(path_to_file,"r");
  cr_assert(png_dimensions(f)==NULL);
  fclose(f);
}

Test(project1_png_map, png_map00)
{
  FILE *f = fopen(path_to_file,"r");
  cr_assert(png_map(f)==NULL);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt00)
{
  FILE *f = fopen(path_to_file,"r");
  cr_assert(extract_tEXt(f,0)==NULL);
  fclose(f);
}
