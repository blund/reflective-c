
#include <bl/string_builder.h>
#include "reflect.h"
#include "ast.h"

void emit_decl(string_builder* b, AST_Struct* s) {
  add_to(b, "struct %s {\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node *n = c->list[i];
    switch(n->kind) {
    case(AST_KIND_STRUCT): {
      emit_decl(b, (AST_Struct*)n);
    } break;
    case(AST_KIND_DECL): {
      AST_VarDecl* v = (AST_VarDecl*)n;
      add_to(b, "  %s %s;\n", v->type, v->name);
    } break;
    }
  }
  add_to(b, "};\n", s->name);
}

void emit_field(string_builder* b, AST_VarDecl* v) {
    add_to(b, "add_to(b, \"%s: %s \", obj->%s);\n", v->name, "%f", v->name);
}

void emit_struct(string_builder* b, AST_Struct* s) {
  add_to(b, "add_to(b, \"struct %s  { \");\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node* child = c->list[i];
    switch(child->kind) {
    case(AST_KIND_DECL):   emit_field(b, (AST_VarDecl*)child); break;
    case(AST_KIND_STRUCT): emit_struct(b, (AST_Struct*)child); break;
    default:
      puts("Failed building print function!");
    }
  }
  add_to(b, "add_to(b, \"};\");\n");
  add_to(b, "printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", s->name);
}

void emit_print_fn(AST_Struct* n) {
  string_builder* b = new_builder(256);

  add_to(b, "#include <bl/string_builder.h>\n");
  emit_decl(b, n);
  add_to(b, "void print_%s(struct %s* obj) {\n", n->name, n->name);
  add_to(b, "string_builder* b = new_builder(128);\n");
  emit_struct(b, n);
  
  FILE *fptr;
  char file_name[32];
  sprintf(file_name, "gen/%s.c", n->name);
  fptr = fopen(file_name,"w");
  fprintf(fptr, "%s\n", to_string(b));
  fclose(fptr);
}

