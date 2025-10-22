#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "png-tools.h"

unsigned int chunk_list_length(chunk_list *cl)
{
  unsigned int len = 0;
  for (;cl;cl=cl->next)
    ++len;
  return len;
}

void chunk_list_free(chunk_list *cl)
{
  if (cl) {
    chunk_list_free(cl->next);
    free(cl->chunk_type);
    free(cl);
  }
}

// ======== tests

char *c_file = "./test_project1_phase2.c";

char *pngs[] = {
  "./img/compile-her-mug_100x67.png",
  "./img/darkgreen_40x180.png",
  "./img/red_90x30.png",
  "./img/dodgerblue_80x60.png"
};

Test(project1_png_signature, png_signature00)
{
  FILE *f = fopen(c_file,"r");
  cr_assert(!png_signature(f));
  fclose(f);
}

Test(project1_png_signature, png_signature01)
{
  FILE *f = fopen(pngs[0],"r");
  cr_assert(png_signature(f));
  fclose(f);
}

Test(project1_png_signature, png_signature02)
{
  FILE *f = fopen(pngs[1],"r");
  cr_assert(png_signature(f));
  fclose(f);
}

Test(project1_png_signature, png_signature03)
{
  FILE *f = fopen(pngs[2],"r");
  cr_assert(png_signature(f));
  fclose(f);
}

Test(project1_png_signature, png_signature04)
{
  FILE *f = fopen(pngs[3],"r");
  cr_assert(png_signature(f));
  fclose(f);
}

Test(project1_read_int, read_int00)
{
  FILE *f = fopen(c_file,"r");
  cr_assert(read_int(f)==594112099);
  fclose(f);
}

// reminder: PNG signature is  (hex) 89  50  4E  47  0D  0A  1A  0A

Test(project1_read_int, read_int01)
{
  FILE *f = fopen(pngs[1],"r");
  cr_assert(read_int(f)==0X89504E47);
  fclose(f);
}

Test(project1_read_int, read_int02)
{
  FILE *f = fopen(pngs[1],"r");
  fgetc(f); //advance one byte
  cr_assert(read_int(f)==0X504E470D);
  fclose(f);
}

Test(project1_read_int, read_int03)
{
  FILE *f = fopen(pngs[1],"r");
  fgetc(f);
  fgetc(f); //advance two bytes
  cr_assert(read_int(f)==0X4E470D0A);
  fclose(f);
}

Test(project1_read_int, read_int04)
{
  FILE *f = fopen(pngs[1],"r");
  fgetc(f);
  fgetc(f);
  fgetc(f); //advance three bytes
  cr_assert(read_int(f)==0X470D0A1A);
  fclose(f);
}

Test(project1_read_chunk_tag, read_chunk_tag00)
{
  FILE *f = fopen(c_file,"r");
  char *t = read_chunk_tag(f);
  cr_assert(!strcmp(t,"#inc"));
  free(t);
  fclose(f);
}

Test(project1_read_chunk_tag, read_chunk_tag01)
{
  FILE *f = fopen(pngs[0],"r");
  fseek(f, 12, SEEK_CUR); // adv. past signature and length of chunk 0
  char *t = read_chunk_tag(f);
  cr_assert(!strcmp(t,"IHDR"));
  free(t);
  fclose(f);
}

Test(project1_read_chunk_tag, read_chunk_tag02)
{
  FILE *f = fopen(pngs[0],"r");
  fseek(f, -8, SEEK_END); // move to 8 before the end
  char *t = read_chunk_tag(f);
  cr_assert(!strcmp(t,"IEND"));
  free(t);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions00)
{
  FILE *f = fopen(c_file,"r");
  cr_assert(png_dimensions(f)==NULL);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions01)
{
  FILE *f = fopen(pngs[0],"r");
  struct dims *d = png_dimensions(f);
  cr_assert(d->width==100 && d->height==67);
  free(d);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions02)
{
  FILE *f = fopen(pngs[1],"r");
  struct dims *d = png_dimensions(f);
  cr_assert(d->width==40 && d->height==180);
  free(d);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions03)
{
  FILE *f = fopen(pngs[2],"r");
  struct dims *d = png_dimensions(f);
  cr_assert(d->width==90 && d->height==30);
  free(d);
  fclose(f);
}

Test(project1_png_dimensions, png_dimensions04)
{
  FILE *f = fopen(pngs[3],"r");
  struct dims *d = png_dimensions(f);
  cr_assert(d->width==80 && d->height==60);
  free(d);
  fclose(f);
}

Test(project1_png_map, png_map00)
{
  FILE *f = fopen(c_file,"r");
  cr_assert(png_map(f)==NULL);
  fclose(f);
}

Test(project1_png_map, png_map01)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  chunk_list *map = png_map(f);
  cr_assert(map);
  chunk_list_free(map);
  fclose(f);
}

Test(project1_png_map, png_map02)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  chunk_list *map = png_map(f);
  cr_assert(chunk_list_length(map)==10);
  chunk_list_free(map);
  fclose(f);
}

Test(project1_png_map, png_map03)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  chunk_list *map = png_map(f);
  chunk_list *head_ptr = map;
  char *tags[] = {"IHDR","cHRM","PLTE","bKGD","tIME","IDAT","tEXt","tEXt","tEXt","IEND"};
  int i = 0;
  while (map) {
    cr_assert(!strcmp(tags[i++],map->chunk_type));
    map=map->next;
  }		   
  chunk_list_free(head_ptr);
  fclose(f);
}

Test(project1_png_map, png_map04)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  chunk_list *map = png_map(f);
  chunk_list *head_ptr = map;
  uint32_t start_bytes[] = {0X08,0X21,0X4D,0X5F,0X6C,0X7F,0X9D,0XCE,0XFF,0X133};
  int i = 0;
  while (map) {
    cr_assert(map->start_byte==start_bytes[i]);
    ++i;
    map=map->next;
  }		   
  chunk_list_free(head_ptr);
  fclose(f);
}

Test(project1_png_map, png_map05)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  chunk_list *map = png_map(f);
  chunk_list *head_ptr = map;
  uint32_t lengths[] = {13,32,6,1,7,18,37,37,40,0};
  int i = 0;
  while (map) {
    cr_assert(map->length==lengths[i]);
    ++i;
    map=map->next;
  }		   
  chunk_list_free(head_ptr);
  fclose(f);
}

Test(project1_png_map, png_map06)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  chunk_list *map = png_map(f);
  cr_assert(chunk_list_length(map)==74);
  chunk_list_free(map);
  fclose(f);
}

Test(project1_png_map, png_map07)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  chunk_list *map = png_map(f);
  chunk_list *head_ptr = map;
  char *tags[] = {"IHDR","cHRM","bKGD","pHYs","tIME","zTXt","zTXt","orNT","IDAT","eXIf"};
  // the rest of the tags are all tEXt, except for IEND
  int i = 0;
  while (map) {
    if (i==73)
      cr_assert(!strcmp("IEND",map->chunk_type));
    else if (i>9)
      cr_assert(!strcmp("tEXt",map->chunk_type));
    else
      cr_assert(!strcmp(tags[i],map->chunk_type));
    ++i;
    map=map->next;
  }		   
  chunk_list_free(head_ptr);
  fclose(f);
}

Test(project1_png_map, png_map08)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  chunk_list *map = png_map(f);
  chunk_list *head_ptr = map;
  // spot check some of the lengths (not comprehensive)
  uint32_t lengths[] = {13,32,6,9,7,110,440,1,10704,29308};
  for (int i=0; i<10; ++i) {
    cr_assert(map->length==lengths[i]);
    map = map->next;
  }
  chunk_list_free(head_ptr);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt00)
{
  FILE *f = fopen(c_file,"r");
  cr_assert(extract_tEXt(f,0)==NULL);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt01)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  char *kv = extract_tEXt(f,0);
  int n = strlen("date:create");
  cr_assert(!strcmp(kv,"date:create") && !strcmp(kv+n+1,"2025-10-20T13:47:50+00:00"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt02)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  char *kv = extract_tEXt(f,1);
  int n = strlen("date:modify");
  cr_assert(!strcmp(kv,"date:modify") && !strcmp(kv+n+1,"2025-10-20T13:47:50+00:00"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt03)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  char *kv = extract_tEXt(f,2);
  int n = strlen("date:timestamp");
  cr_assert(!strcmp(kv,"date:timestamp") && !strcmp(kv+n+1,"2025-10-20T13:47:50+00:00"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt04)
{
  FILE *f = fopen(pngs[1],"r"); // darkgreen_40x180.png
  cr_assert(!extract_tEXt(f,3));
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt05)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  char *kv = extract_tEXt(f,0);
  int n = strlen("date:create");
  cr_assert(!strcmp(kv,"date:create") && !strcmp(kv+n+1,"2025-10-07T13:54:47+00:00"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt06)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  char *kv = extract_tEXt(f,15);
  int n = strlen("exif:ExifVersion");
  cr_assert(!strcmp(kv,"exif:ExifVersion") &&
	    !strcmp(kv+n+1,"0231"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt07)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  char *kv = extract_tEXt(f,19);
  int n = strlen("exif:ExposureTime");
  cr_assert(!strcmp(kv,"exif:ExposureTime") &&
	    !strcmp(kv+n+1,"1/100"));
  free(kv);
  fclose(f);
}

Test(project1_extract_tEXt, extract_tEXt08)
{
  FILE *f = fopen(pngs[0],"r"); // compile-her-mug_100x67.png
  cr_assert(!extract_tEXt(f,999));
  fclose(f);
}

