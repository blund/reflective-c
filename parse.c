
#include <string.h>
#include <malloc.h>

#include "reflect.h"
#include "parse.h"

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
    n->fields = realloc(n->fields, sizeof(var_node)*n->field_capacity*2);
  }

  var_node* fn = malloc(sizeof(var_node));

  n->fields[n->field_index] = (node_t*)fn;
  word_len = parse_word(base, index, &word);
  fn->type = word;

  word_len = parse_word(base, index, &word);
  fn->name = word;
  fn->kind = var_k;

  n->field_index++;
  return 1;
}
