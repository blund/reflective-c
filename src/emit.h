
#include <bl/string_builder.h>
#include "reflect.h"

void build_decl(string_builder* b, struct_node* node);
void build_field(string_builder* b, var_node* n);
void build_struct(string_builder* b, struct_node* n);
void make_print_fn(struct_node* n);
