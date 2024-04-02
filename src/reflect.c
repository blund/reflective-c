
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "reflect.h"
#include "parse.h"
#include "emit.h"

int main(int argc, char **argv) {

  if (argc != 2) {
      fprintf(stderr, "Bad amount of arguments, expected 1 but got %d\n", argc);
  }

  char* file_name = argv[1];

  const int file_max_size = 1024*1024;
  char file_content[file_max_size];
  FILE *fp = fopen(file_name, "r");
  if (fp != NULL) {
    size_t len = fread(file_content, sizeof(char), file_max_size, fp);
    if (ferror(fp) != 0) {
      fprintf(stderr, "Could not read source file '%s'\n", file_name);
    } else {
      file_content[len++] = '\0'; /* Just to be safe. */
    }

    fclose(fp);
  }

  char* base = file_content;

  parse_state ps = {
    .base = file_content,
    .index = 0,
    .len = 0,
  };

  struct_node n = {.field_index = 0, .field_capacity = 4};
  parse_struct(&ps, &n);
  make_print_fn(&n);
 
  return 0;
}
