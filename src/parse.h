
#include "reflect.h"

typedef struct parse_state {
  char* base;
  int index;

  char* word;
  int len;
} parse_state;


void eat_whitespace(parse_state* ps);
int parse_exact(parse_state* ps, char* word);
int parse_word(parse_state* ps);
int parse_field(parse_state* ps, struct_node* n);
int parse_struct(parse_state* ps, struct_node* n);
