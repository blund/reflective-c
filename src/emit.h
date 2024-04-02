
#include <bl/string_builder.h>
#include "reflect.h"

void emit_decl(string_builder* b, struct_node* node);
void emit_field(string_builder* b, var_node* n);
void emit_struct(string_builder* b, struct_node* n);
void emit_print_fn(struct_node* n);
