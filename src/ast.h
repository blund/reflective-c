#ifndef AST_H
#define AST_H

typedef enum {
  AST_KIND_DECL,
  AST_KIND_STRUCT,
} AST_NODE_KIND;

typedef enum {
  AST_TYPE_FLOAT,
  AST_TYPE_INT,
  AST_TYPE_CHAR,
  AST_TYPE_STRING,
} AST_TYPE;

typedef struct AST_Node {
  AST_NODE_KIND kind;
} AST_Node;

typedef struct AST_Children {
  AST_Node **list;
  int capacity;
  int index;
} AST_Children;

typedef struct AST_VarDecl {
  AST_Node node;
  char* name;
  char* type;
} AST_VarDecl;

typedef struct AST_Struct {
  AST_Node node;
  AST_Children children;
  char* name;
} AST_Struct;

void init_children(AST_Children* c);
void add_child(AST_Children* c, AST_Node* n);
#endif
