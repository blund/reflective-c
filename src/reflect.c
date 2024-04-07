
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

  // Handle defining generics
  for (;;) {
    if(!parse_until(&ps, "REFLECT_GENERIC(")) goto end_reflect_generic;
    {
      int start = ps.index;
    
      AST_Func* f = malloc(sizeof(AST_Func));
      init_children(&f->params);
      parse_fun(&ps, f);
      Generic* g = malloc(sizeof(Generic));
      g->sig = f;
      g->code = "";

      parse_until(&ps, "REFLECT_GENERIC_END");
      int len = ps.index - start - strlen("REFLECT_GENERIC_END");

      g->code = malloc(len);
      strncpy(g->code, ps.base + start, len);
      g->code[len] = 0;

      shput(ctx.generics_map, f->name, g);
    }
  }
 end_reflect_generic:

  ps.index = 0;

  string_builder* specialized = new_builder(128);
  for (;;) {
    // Handle specializing functions
    if (!parse_until(&ps, "REFLECT_SPECIALIZE(")) goto end_reflect_specialize;
    {
      AST_Func* f2 = malloc(sizeof(AST_Func));
      init_children(&f2->params);
      parse_fun(&ps, f2);
      Generic* g = shget(ctx.generics_map, f2->name);

      char* code = g->code;


      // rename gen_param and spec_param
      AST_VarDecl** param_gen = (AST_VarDecl**)g->sig->params.list;
      AST_VarDecl** param_spec = (AST_VarDecl**)f2->params.list;

      //char* name = "arr_add_int";
      string_builder* name_b = new_builder(32);
      add_to(name_b, g->sig->name);

      string_builder* base_type_b = new_builder(32);
      add_to(base_type_b, param_spec[0]->type);


      for (int i = 0; i < g->sig->params.index; i++) {
	string_builder* b = new_builder(8);
	add_to(b, param_spec[i]->type);
	add_to(name_b, "_");
	add_to(name_b, param_spec[i]->type);
	for (int j = 0; j < param_spec[i]->ptr_level; j++) {
	  if (i == 0) {
	    add_to(name_b, "p");
	    add_to(base_type_b, "*");
	  }
	  add_to(b, "*");
	}

	code = str_replace(code, param_gen[i]->type, to_string(b));
	free_builder(b);
	if (!code) break;
      }

      char* specialized_name = to_string(name_b);
      code = str_replace(code, g->sig->name, specialized_name);

      string_builder* file_b = new_builder(32);
      add_to(file_b, "#include <malloc.h>\n");
      add_to(file_b, "#include <bl/arr.h>\n");

 
      add_to(file_b, "#define %s(a, b) _Generic((a),", g->sig->name);
      add_to(file_b, "%s: %s", to_string(base_type_b), specialized_name);
      add_to(file_b,  ")((a),(b))\n");

      add_to(file_b, "void %s(", specialized_name);
      for (int i = 0; i < f2->params.index; i++) {
	if (i != 0) add_to(file_b, ",");
	string_builder *b = new_builder(4);
	add_to(b, param_spec[i]->type);
	for (int j = 0; j < param_spec[i]->ptr_level; j++) {
	  add_to(b, "*");
	}
	add_to(file_b, "%s arg%d", to_string(b), i);
	free_builder(b);
      }
      add_to(file_b, ");\n");


      add_to(file_b, "#ifndef GEN_SPECIALIZE_IMPORT");
      add_to(file_b, code);
      add_to(file_b, "#endif");


      char specialized_dir[128];
      sprintf(specialized_dir, "%s/specialized", out_dir);
      
      write_string(to_string(file_b), specialized_dir, g->sig->name);

      add_to(specialized, "\n#include \"specialized/%s.c\"", g->sig->name);
    }
  }
 end_reflect_specialize:
  write_string(to_string(specialized), out_dir, "gen");

  // Handle printing for structs
  ps.index = 0;
  for (;;) {
    AST_Struct* s = malloc(sizeof(AST_Struct));
    init_children(&s->children);

    if (!parse_until(&ps, "REFLECT_DERIVE_PRINT")) goto end_parse;

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
