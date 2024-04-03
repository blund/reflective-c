
#include <string.h>
#include <malloc.h>

#include "ast.h"
#include "parse.h"

#define KV(string, type, format) {#string, format},
kv_b to_format[4] = {
  TYPES
};
#undef KV

char* string_to_format(char* string) {
  for (int i = 0; i < 4; i++) {
    int r = !strncmp(to_format[i].key, string, 8);
    if (r) return to_format[i].format;
  }
  return NULL;
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
