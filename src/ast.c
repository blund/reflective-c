
#include <malloc.h>

#include "ast.h"
#include "parse.h"

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
