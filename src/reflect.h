
#ifndef REFLECT_H
#define REFLECT_H
#define BL_REFLECT_PRINT(name) typedef struct name name; void print_##name ( struct name* obj )


typedef enum node_kind {
  struct_k,
  var_k
} node_kind;

typedef struct node {
  node_kind kind;
} node_t;

typedef struct var_node {
  node_kind kind;
  char* name;
  char* type;
} var_node;

typedef struct struct_node {
  node_kind kind;
  char* name;
  node_t** fields;
  int field_index;
  int field_capacity;
} struct_node;
#endif

