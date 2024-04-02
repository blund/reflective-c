
#include "reflect.h"
#include "ast.h"

typedef struct parse_state {
  char* base;
  int index;

  char* word;
  int len;
} parse_state;


void eat_whitespace(parse_state* ps);
int parse_exact(parse_state* ps, char* word);
int parse_word(parse_state* ps);
int parse_field(parse_state* ps, AST_Children* c);
int parse_struct(parse_state* ps, AST_Struct* s);
int parse_oneof(parse_state* ps, char* matches);
