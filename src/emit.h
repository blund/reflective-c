
#include <bl/string_builder.h>
#include "reflect.h"
#include "ast.h"
#include "context.h"

void emit_decl(string_builder* b, Context* ctx, AST_Struct* s);
void emit_field(string_builder* b, AST_VarDecl* n);
void emit_struct(string_builder* b, Context* ctx, AST_Struct* s);
void emit_print_fn(AST_Struct* n, Context* ctx, char* out_dir);
void write_string(char* text, char* path, char* name);
