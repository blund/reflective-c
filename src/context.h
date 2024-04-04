

#ifndef CONTEXT_H
#define CONTEXT_H

#include "ast.h"

typedef struct StructMap {
  char* key;
  AST_Struct* value;
} StructMap;

typedef struct Generic {
  AST_Func* sig;
  char* code;
} Generic;

typedef struct GenericsMap {
  char* key;
  Generic *value;
} GenericsMap;


typedef struct Context {
  StructMap* struct_map;
  GenericsMap* generics_map;
} Context;

#endif
