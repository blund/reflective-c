
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
  char* buffer;
  int buf_len = read_entire_file(file_name, &buffer);

  parse_state ps = {
    .base = buffer,
    .buf_len = buf_len,
    .index = 0,
    .len = 0,
  };

  for (;;) {
    AST_Struct s;
    init_children(&s.children);
    for(;;) {
      int result = parse_exact(&ps, "BL_REFLECT_PRINT");
      if (result) break;
      parse_token(&ps);

      // There are no more BL_REFLCET_PRINT statements in the given file
      if (ps.index >= ps.buf_len) goto end_parse;
    }

    parse_exact(&ps, "(");
    parse_word(&ps);
    parse_exact(&ps, ")");
    parse_exact(&ps, ";");

    int success = parse_struct(&ps, &s);
    if (!success) {
      return 0;
    }
    emit_print_fn(&s);
  }
 end_parse:

  return 0;
}
