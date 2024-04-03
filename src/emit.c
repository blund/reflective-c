
#include <stb/stb_ds.h>

#include <bl/string_builder.h>

#include "ast.h"
#include "context.h"

void emit_sub_decl(string_builder* b, AST_Struct* s, char* name);
void emit_sub_struct(string_builder* b, Context* ctx, AST_Struct* s, char* name);

void emit_decl(string_builder* b, Context* ctx, AST_Struct* s) {
  add_to(b, "typedef struct %s {\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node *n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_decl, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    char* format_string = string_to_format(v->type);
    add_to(b, "  %s %s;\n", v->type, v->name);
  }
  add_to(b, "} %s;\n", s->name);
}

void emit_sub_decl(string_builder* b, AST_Struct* s, char* name) {
  add_to(b, "%s %s {\n", s->name, name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node *n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_decl, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    add_to(b, "  %s %s;\n", v->type, v->name);
  }
  add_to(b, "};\n", s->name);
}

void emit_field(string_builder* b, AST_VarDecl* v, char* name) {
  add_to(b, "add_to(b, \"%s: %s, \", ", v->name, string_to_format(v->type));
  add_to(b, "obj->");
  if (name) {
    add_to(b, "%s.", name);
  }
  add_to(b, "%s);\n", v->name);
}

void emit_struct(string_builder* b, Context* ctx, AST_Struct* s) {
  add_to(b, "add_to(b, \"struct %s  { \");\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node* n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_struct, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    // v is of a c standard type
    char* f = string_to_format(v->type);
    puts("???");
    printf("%s %s\n", f, v->type);
    if (string_to_format(v->type) != NULL) {
      emit_field(b, (AST_VarDecl*)n, NULL);
    } else {
      AST_Struct* sub = shget(ctx->struct_map, v->type);
      if (!sub) {
	puts("Struct %s was not defined\n");
	return;
      }
      emit_sub_struct(b, ctx, sub, v->name);
    }
  }
  add_to(b, "add_to(b, \"};\");\n");
  add_to(b, "printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", s->name);
}

void emit_sub_struct(string_builder* b, Context* ctx, AST_Struct* s, char* name) {
  add_to(b, "add_to(b, \"%s:  { \");\n", name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    AST_Node* n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_struct, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    // v is of a c standard type
    if (string_to_format(v->type) != NULL) {
      emit_field(b, (AST_VarDecl*)n, name);
    } else {
      AST_Struct* sub = shget(ctx->struct_map, v->type);
      if (!sub) {
	puts("Struct %s was not defined\n");
	return;
      }
      emit_struct(b, ctx, sub);
    }
  }
  add_to(b, "add_to(b, \"} \");\n");
}


void emit_print_fn(AST_Struct* n, Context* ctx) {
  string_builder* b = new_builder(256);

  // First, we need to find any dependencies
  // @TODO - This has to be recursive
  AST_Struct* dependencies[32];
  int index = 0;

  AST_Children* c = &n->children;
  for (int i = 0; i < c->index; i++) {
    AST_VarDecl* v = (AST_VarDecl*)c->list[i];
    if (!string_to_format(v->type)) {
      AST_Struct* s = shget(ctx->struct_map, v->type);
      if (!s) {
	printf("Error in building dependencies for '%s', '%s' was not defined\n", n->name, s->name);
	return;
      }
      dependencies[index++] = s;
    }
  }
  // Emit dependencies
  add_to(b, "#include <bl/string_builder.h>\n");
  for (int i = 0; i < index; i++) {
    emit_decl(b, ctx, dependencies[i]);
  }


  
  emit_decl(b, ctx, n);
  add_to(b, "void print_%s(struct %s* obj) {\n", n->name, n->name);
  add_to(b, "string_builder* b = new_builder(128);\n");
  emit_struct(b, ctx, n);
  
  FILE *fptr;
  char file_name[32];
  sprintf(file_name, "gen/%s.c", n->name);
  fptr = fopen(file_name,"w");
  fprintf(fptr, "%s\n", to_string(b));
  fclose(fptr);
}

