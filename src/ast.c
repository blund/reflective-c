
#include <string.h>
#include <malloc.h>

#include "ast.h"
#include "parse.h"

#define KV(string, type, format) {#string, AST_TYPE_##type},
kv_a from_string[4] = {
  TYPES
};
#undef KV

AST_TYPE string_to_type(char* s) {
  for (int i = 0; i < 4; i++) {
    int r = !strncmp(from_string[i].key, s, 8);
    if (r) return from_string[i].type;
  }
  return AST_TYPE_NONE;
}

#define KV(string, type, format) {AST_TYPE_##type, #string, format},
kv_b from_type[4] = {
  TYPES
};
#undef KV

char* type_to_format(AST_TYPE t) {
  for (int i = 0; i < 4; i++) {
    int r = from_type[i].key == t;
    if (r) return from_type[i].format;
  }
  return "";
}
char* type_to_string(AST_TYPE t) {
  for (int i = 0; i < 4; i++) {
    int r = from_type[i].key == t;
    if (r) return from_type[i].string;
  }
  return "";
}


void add_child(AST_Children* c, AST_Node* n) {
  // Grow capacity if needed
  if (c->index > c->capacity) {
    c->capacity *= 2;
    c->list = (AST_Node**)realloc(c->list, sizeof(AST_Node*)*c->capacity);
  }

  c->list[c->index] = n;
  c->index += 1;
}

void init_children(AST_Children* c) {
  *c = (AST_Children){NULL, 4, 0};
  c->list = malloc(sizeof(AST_Node*)*c->capacity);
}
