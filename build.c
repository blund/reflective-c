
#include "reflect.h"
#include <bl/string_builder.h>

void build_decl(string_builder* b, struct_node* node) {
  add_to(b, "struct %s {\n", node->name);
  for (int i = 0; i < node->field_index; i++) {
    node_t* n = node->fields[i];
    switch(n->kind) {
    case(struct_k): {
      build_decl(b, (struct_node*)n);
    } break;
    case(var_k): {
      var_node* var = (var_node*)n;
      add_to(b, "  %s %s;\n", var->type, var->name);
    } break;
    }
  }
  add_to(b, "};\n", node->name);
}

void build_field(string_builder* b, var_node* n) {
    add_to(b, "add_to(b, \"%s: %s \", obj->%s);\n", n->name, "%f", n->name);
}

void build_struct(string_builder* b, struct_node* n) {
  add_to(b, "add_to(b, \"struct %s  { \");\n", n->name);
  for (int i = 0; i < n->field_index; i++) {
    node_t* child = n->fields[i];
    switch(child->kind) {
    case(var_k): build_field(b, (var_node*)child); break;
    case(struct_k): build_struct(b, (struct_node*)child); break;
    default:
      puts("Failed building print function!");
    }
  }
  add_to(b, "add_to(b, \"};\");\n");
  add_to(b, "printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", n->name);
}

void make_print_fn(struct_node* n) {
  string_builder* b = new_builder(256);

  add_to(b, "#include <bl/string_builder.h>\n");
  build_decl(b, n);
  add_to(b, "void print_%s(struct %s* obj) {\n", n->name, n->name);
  add_to(b, "string_builder* b = new_builder(128);\n");
  build_struct(b, n);
  
  FILE *fptr;
  char file_name[32];
  sprintf(file_name, "gen/%s.c", n->name);
  fptr = fopen(file_name,"w");
  fprintf(fptr, "%s\n", to_string(b));
  fclose(fptr);
}

