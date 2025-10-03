#ifndef __PNG_TOOLS_H__
#define __PNG_TOOLS_H__

#include <stdio.h>
#include <stdint.h>

struct dims {
  uint32_t width;
  uint32_t height;
};

struct chunk_list {
  uint32_t start_byte;
  uint32_t length;
  char *chunk_type;
  struct chunk_list *next;
};
typedef struct chunk_list chunk_list;

// do not call fclose in any of the following functions

// verify the first 8 bytes of a file are as expected for PNG
// precondition: the file is open to byte 0
int png_signature(FILE *f);

// read 4 bytes in sequence as big-endian uint32
// read the bytes starting from where f is open 
// if the file doesn't have 4 more bytes in it, GIGO
uint32_t read_int(FILE *f);

// read 4 bytes in sequence as PNG chunk tag
// read the bytes starting from where f is open
// return a new heap-allocated string
// if the file doesn't have 4 more bytes in it, GIGO
char *read_chunk_tag(FILE *f);

// read the width and height from the IHDR chunk
// return a new heap-allocated struct
// precondition: the file is open to byte 0
// if the file is not a PNG, return NULL
struct dims *png_dimensions(FILE *f);

// verify the signature of the PNG,
// then build a chunk list describing the chunks
// the list nodes should appear in file order: IHDR first, etc.
// precondition: the file is open to byte 0
// note: the file signature (its first 8 bytes) is not a chunk
// if the file is not a PNG, return NULL
chunk_list *png_map(FILE *f);

// extract the text (the data) from the ith tEXt chunk in the file
// the index is 0-based: the first tEXt chunk is 0, the second is 1, etc.
// if there is no such tEXt chunk, return NULL
// otherwise return a new heap-allocated string properly terminated
// precondition: the file is open to byte 0
// if the file is not a PNG, return NULL
char *extract_tEXt(FILE *f, uint32_t i);

#endif
