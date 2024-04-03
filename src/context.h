

#ifndef CONTEXT_H
#define CONTEXT_H

#include "ast.h"

typedef struct StructMap {
  char* key;
  AST_Struct* value;
} StructMap;

typedef struct Context {
  StructMap* struct_map;
} Context;

#endif
