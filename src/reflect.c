
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#define BL_IO_IMPLEMENTATION
#include <bl/io.h>

#include "ast.h"
#include "parse.h"
#include "emit.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Bad amount of arguments, expected 1 but got %d\n", argc);
    return 0;
  }

  char* file_name = argv[1];
  char* buffer = read_entire_file(file_name);

  parse_state ps = {
    .base = buffer,
    .index = 0,
    .len = 0,
  };

  AST_Struct s;
  init_children(&s.children);
  for(;;) {
    int result = parse_exact(&ps, "BL_REFLECT_PRINT");
    if (result) break;
    parse_word(&ps);
  }

  parse_exact(&ps, "(");
  parse_word(&ps);
  parse_exact(&ps, ")");
  parse_exact(&ps, ";");

  int success = parse_struct(&ps, &s);
  if (!success) {
    puts("Failed to parse struct");
    return 0;
  }

  emit_print_fn(&s);

  return 0;
}
