
#include <string.h>
#include <malloc.h>

#include "reflect.h"
#include "parse.h"

void eat_whitespace(parse_state* ps) {
 next:
  switch(*(ps->base + ps->index)) {
  case ' ': goto iterate;
  case ';': goto iterate;
  case '\n': goto iterate;
  case '\t': goto iterate;
  default: return;
  }
 iterate:
  ps->index++; goto next;
}

int parse_exact(parse_state* ps, char* word) {
  eat_whitespace(ps);
  int len = strlen(word);
  if (!strncmp(ps->base+ps->index, word, len)) {
    ps->index += len;
    return 1;
  }
  return 0;
}

int parse_word(parse_state* ps) {
  eat_whitespace(ps);
  int start = ps->index;

 next:
  switch(*(ps->base + ps->index)) {
  case ' ': break;
  case ';': break;
  case '\n': break;
  default: {
    ps->index++; goto next;
  }
  }

  int len = ps->index - start;
  ps->word = (char*)malloc(len);

  strncpy(ps->word, ps->base + start, len);
  ps->word[len] = 0;

  return len+1;
}

int parse_field(parse_state* ps, struct_node* n) {
  int word_len = 0;
  char* word;

  if (n->field_index >= n->field_capacity) {
    n->fields = realloc(n->fields, sizeof(var_node)*n->field_capacity*2);
  }

  var_node* fn = malloc(sizeof(var_node));

  n->fields[n->field_index] = (node_t*)fn;
  word_len = parse_word(ps);
  fn->type = ps->word;

  word_len = parse_word(ps);
  fn->name = ps->word;
  fn->kind = var_k;

  n->field_index++;
  return 1;
}

int parse_struct(parse_state* ps, struct_node* n) {
  for(;;) {
    int result = parse_exact(ps, "BL_REFLECT_PRINT");
    if (result) break;
    parse_word(ps);
  }
  parse_exact(ps, "(");
  parse_word(ps); // Closing paren is implicit, might be bad if we make except for parens


  // Allocate array for struct fields
  n->fields = malloc(sizeof(var_node*) * n->field_capacity);

  // Begin building struct
  if (!parse_exact(ps, "struct")) return 0;
  parse_word(ps);
  n->name = ps->word;

  if (!parse_exact(ps, "{")) return 0;

  for (;;) {
    parse_field(ps, n);
    if(parse_exact(ps, "};")) break;
  }

  return 1;
}
