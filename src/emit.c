
#include <stb/stb_ds.h>

#include <bl/string_builder.h>

#include "ast.h"
#include "context.h"


typedef struct refs {
  AST_VarDecl* chain[32];
  int count;
} refs;

void emit_sub_decl(string_builder* b, AST_Struct* s, char* name);
void emit_sub_struct(string_builder* b, Context* ctx, AST_Struct* s, refs ref);

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
    if (v->node.flags & AST_NODE_PTR) {
      add_to(b, "  %s* %s;\n", v->type, v->name);
    } else {
      add_to(b, "  %s %s;\n", v->type, v->name);
    }
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
    if (v->node.flags & AST_NODE_PTR) {
      add_to(b, "  %s* %s;\n", v->type, v->name);
    } else {
      add_to(b, "  %s %s;\n", v->type, v->name);
    }
  }
  add_to(b, "};\n", s->name);
}

void emit_field(string_builder* b, AST_VarDecl* v, refs ref) {
  add_to(b, "add_to(b, \"%s: %s, \", ", v->name, string_to_format(v->type));
  add_to(b, "obj->");

  for (int i = 0; i < ref.count; i++) {
    AST_VarDecl* v2 = ref.chain[i];

    if (v2->node.flags & AST_NODE_PTR) {
      add_to(b, "%s->", v2->name);
    } else {
      add_to(b, "%s.", v2->name);
    }
  }
  add_to(b, "%s);\n", v->name);
}

void emit_struct(string_builder* b, Context* ctx, AST_Struct* s) {

  // Refs are used to keep track of names in nested accesses in structs
  refs ref = {{},0};

  add_to(b, "add_to(b, \"struct %s  { \");\n", s->name);

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    ref.count = 0;
    AST_Node* n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_struct, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    // v is of a c standard type
    char* f = string_to_format(v->type);
    if (string_to_format(v->type) != NULL) {
      emit_field(b, (AST_VarDecl*)n, ref);
    } else {
      AST_Struct* sub = shget(ctx->struct_map, v->type);
      sub->node.flags = v->node.flags; // Inherit flags (to know if it is a ptr)
      if (!sub) {
	puts("Struct %s was not defined\n");
	return;
      }
      ref.chain[ref.count] = v;
      ref.count++;
      emit_sub_struct(b, ctx, sub, ref);
    }
  }
  add_to(b, "add_to(b, \"};\");\n");
  add_to(b, "printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", s->name);
}

void emit_sub_struct(string_builder* b, Context* ctx, AST_Struct* s, refs ref) {
  add_to(b, "add_to(b, \"%s: { \");\n", ref.chain[ref.count-1]->name); // I don't know why this is -1

  int local_ref_count = ref.count;

  AST_Children* c = &s->children;
  for (int i = 0; i < c->index; i++) {
    ref.count = local_ref_count;
    AST_Node* n = c->list[i];

    if (n->kind != AST_KIND_DECL) {
      printf("In emit_struct, got bad node %d\n", n->kind);
    }

    AST_VarDecl* v = (AST_VarDecl*)n;
    // v is of a c standard type
    if (string_to_format(v->type) != NULL) {
      emit_field(b, v, ref);
    } else {
      AST_Struct* sub = shget(ctx->struct_map, v->type);
      if (!sub) {
	puts("Struct %s was not defined\n");
	return;
      }
      ref.chain[ref.count] = v;
      ref.count++;
      emit_sub_struct(b, ctx, sub, ref);
    }
  }
  add_to(b, "add_to(b, \"}, \");\n");
}


void build_dependency_line(Context* ctx, AST_Struct** dependencies, AST_Struct* n, int* index) {
  AST_Children* c = &n->children;
  for (int i = 0; i < c->index; i++) {
    AST_VarDecl* v = (AST_VarDecl*)c->list[i];
    if (!string_to_format(v->type)) {
      AST_Struct* s = shget(ctx->struct_map, v->type);
      if (!s) {
	printf("Error in building dependencies for '%s', '%s' was not defined\n", n->name, s->name);
	return;
      }
      build_dependency_line(ctx, dependencies, s, index);
      dependencies[*index] = s;
      *index += 1;
    }
  }
  return;
}

void write_string(char* text, char* path, char* name) {
  FILE *fptr;
  char file_name[32];
  sprintf(file_name, "%s/%s.c", path, name);
  fptr = fopen(file_name,"w");
  fprintf(fptr, "%s\n", text);
  fclose(fptr);
}


void emit_print_fn(AST_Struct* n, Context* ctx, char* out_dir) {
  string_builder* b = new_builder(256);

  // First, we need to find any dependencies
  // @TODO - This has to be recursive
  AST_Struct* dependencies[32] = {};
  int dep_count = 0;

  build_dependency_line(ctx, dependencies, n, &dep_count);

  // Emit dependencies
  add_to(b, "#include <bl/string_builder.h>\n");
  for (int i = 0; i < dep_count; i++) {
    if (i == dep_count) break; // Did I go insane? I thought the loop was supposed to do this...
    emit_decl(b, ctx, dependencies[i]);
  }
  
  emit_decl(b, ctx, n);
  add_to(b, "void print_%s(struct %s* obj) {\n", n->name, n->name);
  add_to(b, "string_builder* b = new_builder(128);\n");
  emit_struct(b, ctx, n);

  write_string(to_string(b), out_dir, n->name);
 }


