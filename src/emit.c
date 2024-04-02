
#include <bl/string_builder.h>
#include "ast.h"

void emit_decl(string_builder* b, AST_Struct* s) {
  add_to(b, "struct %s {\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node *n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_decl, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    add_to(b, "  %s %s;\n", type_to_string(v->type), v->name);
  }
  add_to(b, "};\n", s->name);
}

void emit_field(string_builder* b, AST_VarDecl* v) {
  add_to(b, "add_to(b, \"%s: %s, \", obj->%s);\n", v->name, type_to_format(v->type), v->name);
}

void emit_struct(string_builder* b, AST_Struct* s) {
  add_to(b, "add_to(b, \"struct %s  { \");\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node* n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_struct, got bad node %d\n", n->kind);
    }

    emit_field(b, (AST_VarDecl*)n);
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

