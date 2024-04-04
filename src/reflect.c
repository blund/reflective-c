
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/
#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#define BL_IO_IMPLEMENTATION
#include <bl/io.h>

#include "str_replace.h"

#include "ast.h"
#include "parse.h"
#include "emit.h"
#include "context.h"


int main(int argc, char **argv) {

  Context ctx = {
    .struct_map = NULL,
    .generics_map = NULL,
  };

  ctx.struct_map = NULL;
  assert(!hmget(ctx.struct_map, ""));

  char* out_dir = "gen";
  char* file_name = argv[1]; // Assume first arg is file to process if no output is given
  if (!strcmp(argv[1], "-o")) {
    out_dir = argv[2];
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

  parse_until(&ps, "BL_GENERIC(");
  {
    int start = ps.index;
    
    AST_Func* f = malloc(sizeof(AST_Func));
    parse_fun(&ps, f);
    Generic* g = malloc(sizeof(Generic));
    g->sig = f;
    g->code = "";

    parse_until(&ps, "BL_GENERIC_END");
    int len = ps.index - start - strlen("BL_GENERIC_END");

    g->code = malloc(len);
    strncpy(g->code, ps.base + start, len);
    g->code[len] = 0;

    shput(ctx.generics_map, f->name, g);
  }

  ps.index = 0;

  parse_until(&ps, "BL_SPECIALIZE(");
  {
    AST_Func* f = malloc(sizeof(AST_Func));
    parse_fun(&ps, f);
    Generic* g = shget(ctx.generics_map, f->name);

    char* code = g->code;

    char* name = "arr_add_int";

    char* c1 = str_replace(code, g->sig->p1, f->p1);
    char* c2 = str_replace(c1, g->sig->name, name);

    string_builder* b = new_builder(256);
    add_to(b, "#include <malloc.h>\n");
    add_to(b, "#define BL_ARR_IMPLEMENTATION\n");
    add_to(b, "#include <bl/arr.h>\n");

    add_to(b, c2);

    write_string(to_string(b), out_dir, g->sig->name);

    reset(b);
    add_to(b, "void arr_add_int(int** arr, int a);\n");
    add_to(b, "#define arr_add(a, b) _Generic((a),");
    add_to(b, "int**: arr_add_int");
    add_to(b,  ")((a),(b))");
    write_string(to_string(b), out_dir, "gen");
  }

  ps.index = 0;
  for (;;) {
    AST_Struct* s = malloc(sizeof(AST_Struct));
    init_children(&s->children);

    if (!parse_until(&ps, "BL_REFLECT_PRINT")) goto end_parse;

    parse_exact(&ps, "(");
    parse_word(&ps);
    parse_exact(&ps, ")");
    parse_exact(&ps, ";");

    int success = parse_struct(&ps, s);
    if (!success) {
      return 0;
    }

    shput(ctx.struct_map, s->name, s);

    emit_print_fn(s, &ctx, out_dir);
  }

 end_parse:

  return 0;
}
