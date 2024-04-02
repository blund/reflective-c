
#include <bl/string_builder.h>
#include "reflect.h"
#include "ast.h"

void emit_decl(string_builder* b, AST_Struct* n);
void emit_field(string_builder* b, AST_VarDecl* n);
void emit_struct(string_builder* b, AST_Struct* n);
void emit_print_fn(AST_Struct* n);
