
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#define BL_IO_IMPLEMENTATION
#include <bl/io.h>

#include "reflect.h"
#include "parse.h"
#include "emit.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Bad amount of arguments, expected 1 but got %d\n", argc);
  }

  char* file_name = argv[1];

  char* buffer = read_entire_file(file_name);

  parse_state ps = {
    .base = buffer,
    .index = 0,
    .len = 0,
  };

  struct_node n = {.field_index = 0, .field_capacity = 4};
  parse_struct(&ps, &n);
  make_print_fn(&n);

  return 0;
}
