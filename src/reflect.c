
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/
#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#define BL_IO_IMPLEMENTATION
#include <bl/io.h>

#include "ast.h"
#include "parse.h"
#include "emit.h"
#include "context.h"

int main(int argc, char **argv) {

  Context ctx;
  ctx.struct_map = NULL;
  assert(!hmget(ctx.struct_map, ""));

  char* out_file = "gen";
  char* file_name = argv[1]; // Assume first arg is file to process if no output is given
  if (!strcmp(argv[1], "-o")) {
    out_file = argv[2];
    file_name = argv[3];
  }

  char* buffer;
  int buf_len = read_entire_file(file_name, &buffer);

  parse_state ps = {
    .base = buffer,
    .buf_len = buf_len,
    .index = 0,
    .len = 0,
  };

  for (;;) {
    AST_Struct* s = malloc(sizeof(AST_Struct));
    init_children(&s->children);
    for(;;) {
      int result = parse_exact(&ps, "BL_REFLECT_PRINT");
      if (result) break;
      parse_token(&ps);

      // There are no more BL_REFLCET_PRINT statements in the given file
      if (ps.index >= ps.buf_len-1) goto end_parse;
    }

    parse_exact(&ps, "(");
    parse_word(&ps);
    parse_exact(&ps, ")");
    parse_exact(&ps, ";");

    int success = parse_struct(&ps, s);
    if (!success) {
      return 0;
    }

    shput(ctx.struct_map, s->name, s);

    char* out_dir = out_file;

    emit_print_fn(s, &ctx, out_dir);
  }
 end_parse:

  return 0;
}
