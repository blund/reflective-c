#ifndef AST_H
#define AST_H

typedef enum {
  AST_KIND_DECL,
  AST_KIND_STRUCT,
} AST_NODE_KIND;

#define TYPES					\
  KV(float, FLOAT, "%f")			\
    KV(int,   INT, "%d")			\
    KV(char*, STRING, "%s")			\
    KV(char,  CHAR, "%c")

#define KV(string, type, format) AST_TYPE_##type,
typedef enum {
  TYPES
  AST_TYPE_NONE = -1,
} AST_TYPE;
#undef KV

AST_TYPE string_to_type(char* s);
char* type_to_string(AST_TYPE t);
char* type_to_format(AST_TYPE t);

typedef struct kv_a {
  char key[8];
  AST_TYPE type;
} kv_a;

typedef struct kv_b {
  AST_TYPE key;
  char string[8];
  char format[8];
} kv_b;

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
  AST_TYPE type;
} AST_VarDecl;

typedef struct AST_Struct {
  AST_Node node;
  AST_Children children;
  char* name;
} AST_Struct;

void init_children(AST_Children* c);
void add_child(AST_Children* c, AST_Node* n);
#endif
