
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
  char* name;
  char* type;
} field_node;

typedef struct struct_node {
  char* name;
  field_node** fields;
  int field_index;
  int field_capacity;
} struct_node;

void print_struct(string_builder* b, struct_node* node) {
  add_to(b, "struct %s {\n", node->name);
  for (int i = 0; i < node->field_index; i++) {
    add_to(b, "  %s %s;\n", node->fields[i]->type, node->fields[i]->name);
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
    add_to(b, "add_to(b, \"%s: %s \", obj->%s);\n", n->fields[i]->name, "%f", n->fields[i]->name);
  }
  add_to(b, "  add_to(b, \"};\");\n");
  add_to(b, "  printf(\"%s\\n\", to_string(b));\n", "%s");
  add_to(b, "};\n", n->name);

  FILE *fptr;
  char file_name[32];
  sprintf(file_name, "gen/%s.c", n->name);
  fptr = fopen(file_name,"w");
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
  case '\n': break;
  default: {
    (*index)++; goto next;
  }
  }


  int len = *index - start;
  if (result == NULL) return len+1;

  *result = malloc(len);

  strncpy(*result, base + start, len);
  (*result)[len] = 0;

  return len+1;
}

int parse_field(char* base, int* index, struct_node* n) {
  int word_len = 0;
  char* word;

  if (n->field_index >= n->field_capacity) {
    n->fields = realloc(n->fields, sizeof(field_node)*n->field_capacity*2);
  }

  field_node* fn = malloc(sizeof(field_node));
  n->fields[n->field_index] = fn;
  word_len = parse_word(base, index, &word);
  n->fields[n->field_index]->type = word;
  //printf("type: '%s'\n", n->fields[n->field_index]->type);

  word_len = parse_word(base, index, &word);
  n->fields[n->field_index]->name = word;
  //printf("name: '%s'\n", n->fields[n->field_index]->name);

  n->field_index++;
  return 1;
}

int main() {

#define MAXBUFLEN 1000000

  char source[MAXBUFLEN + 1];
  FILE *fp = fopen("main.c", "r");
  if (fp != NULL) {
    size_t newLen = fread(source, sizeof(char), MAXBUFLEN, fp);
    if ( ferror( fp ) != 0 ) {
      fputs("Error reading file", stderr);
    } else {
      source[newLen++] = '\0'; /* Just to be safe. */
    }

    fclose(fp);
  }

  char* base = source; //"BL_REFLECT_PRINT(sexy) struct sexy {\nfloat x; \nfloat y; \nfloat z; \n};";

  int index = 0;
  int word_len = 0;
  char* word;

  for(;;) {
    int result = parse_exact(base, &index, "BL_REFLECT_PRINT");
    if (result) break;
    parse_word(base, &index, NULL);
  }
  parse_exact(base, &index, "(");
  parse_word(base, &index, NULL); // Closing paren is implicit, might be bad if we make except for parens

  struct_node n = {.field_index = 0, .field_capacity = 4};

  // Allocate array for struct fields
  n.fields = malloc(sizeof(field_node*) * n.field_capacity);

  // Begin building struct
  if (!parse_exact(base, &index, "struct")) return 0;
  word_len = parse_word(base, &index, &word);
  n.name = word;

  if (!parse_exact(base, &index, "{")) return 0;

  for (;;) {
    parse_field(base, &index, &n);
    if(parse_exact(base, &index, "};")) break;
  }
  make_print_fn(&n);
 
  return 0;
}
