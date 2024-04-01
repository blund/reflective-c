
/*
https://blog.robertelder.org/building-broken-c-parsers/
*/

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct field_node {
  char name[32];
  char type[32];
} field_node;

typedef struct struct_node {
  char name[32];
  field_node fields[32];
  int field_index;
} struct_node;

void print_struct(string_builder* b, struct_node* node) {
  add_to(b, "struct %s {\n", node->name);
  for (int i = 0; i < node->field_index; i++) {
    add_to(b, "  %s %s;\n", node->fields[i].type, node->fields[i].name);
  }
  add_to(b, "};\n", node->name);
}

void make_print_fn(struct_node* n) {
  string_builder* b = new_builder(256);

  add_to(b, "#include <bl/string_builder.h>\n");
  
  print_struct(b, n);
  
  add_to(b, "void print_%s(struct %s* obj) {\n", n->name, n->name);
  add_to(b, "  string_builder* b = new_builder(128);\n");
  add_to(b, "  add_to(b, \"struct %s  { \");\n", n->name);
  for (int i = 0; i < n->field_index; i++) {
    add_to(b, "  ");
    add_to(b, "add_to(b, \"%s: %s \", obj->%s);\n", n->fields[i].name, "%f", n->fields[i].name);
  }
  add_to(b, "  add_to(b, \"};\");\n");
  add_to(b, "  printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", n->name);

  FILE *fptr;
  fptr = fopen("gen/vec3.c","w");
  fprintf(fptr, "%s\n", to_string(b));
  fclose(fptr);

}

void eat_whitespace(char* base, int* index) {
 next:
  switch(*(base + *index)) {
  case ' ': goto iterate;
  case ';': goto iterate;
  case '\n': goto iterate;
  case '\t': goto iterate;
  default: return;
  }
 iterate:
  (*index)++; goto next;
}


int parse_exact(char* base, int* index, char* word) {
  eat_whitespace(base, index);
  int len = strlen(word);
  if (!strncmp(base+*index, word, len)) {
    *index += len;
    return 1;
  }
  return 0;
}

int parse_word(char* base, int* index, char** result) {
  eat_whitespace(base, index);

  int start = *index;

 next:
  switch(*(base + *index)) {
  case ' ': break;
  case ';': break;
  default: {
    (*index)++; goto next;
  }
  }

  int len = *index - start;
  *result = malloc(len);

  strncpy(*result, base + start, len);
  (*result)[len] = 0;

  return len+1;
}

int parse_field(char* base, int* index, struct_node* n) {
  int word_len = 0;
  char* word;

  word_len = parse_word(base, index, &word);
  strncpy(n->fields[n->field_index].type, word, word_len);
  //printf("type: '%s'\n", n->fields[n->field_index].type);

  word_len = parse_word(base, index, &word);
  strncpy(n->fields[n->field_index].name, word, word_len);
  //printf("name: '%s'\n", n->fields[n->field_index].name);

  n->field_index++;
  return 1;
}

int main() {
  char* base = "struct vec3 {\n float x; \nfloat y; \nfloat z; \n};";

  int index = 0;
  int word_len = 0;
  char* word;

  struct_node n = {.field_index = 0};

  // Begin building struct
  if (!parse_exact(base, &index, "struct")) return 0;
  word_len = parse_word(base, &index, &word);
  strncpy(n.name, word, word_len);
  if (!parse_exact(base, &index, "{")) return 0;

  for (;;) {
    parse_field(base, &index, &n);
    if(parse_exact(base, &index, "};")) break;
  }
  make_print_fn(&n);
 
  return 0;
}
