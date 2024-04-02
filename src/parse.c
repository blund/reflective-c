
#include <string.h>
#include <malloc.h>

#include "reflect.h"
#include "ast.h"
#include "parse.h"

void eat_whitespace(parse_state* ps) {
 next:
  switch(*(ps->base + ps->index)) {
  case ' ': goto iterate;
  case '\n': goto iterate;
  case '\t': goto iterate;
  default: return;
  }
 iterate:
  ps->index++; goto next;
}

int parse_oneof(parse_state* ps, char* matches) {
  eat_whitespace(ps);

  int len = strlen(matches);
  char* c = ps->base + ps->index;
  for (int i = 0; i < len; i++) {
    if (*c == matches[i]) {
      ps->index++;
    }
  }
  return 0;
}

int parse_maybe(parse_state* ps, char* word) {
  eat_whitespace(ps);
  int len = strlen(word);
  if (!strncmp(ps->base+ps->index, word, len)) {
    ps->index += len;
    return 1;
  }
  return 1;
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
  case '}': break;
  case '{': break;
  case '\n': break;
  default: {
    ps->index++; goto next;
  }
  }

  int len = ps->index - start;
  ps->word = (char*)malloc(len);

  strncpy(ps->word, ps->base + start, len);
  ps->word[len] = 0;

  return len;
}

int parse_field(parse_state* ps, AST_Children* c) {
  int original_index = ps->index;
  int word_len;

  word_len = parse_word(ps);
  char* type = ps->word;
  if (!word_len) return 0;

  word_len = parse_word(ps);
  char* name = ps->word;
  if (!word_len) return 0;

  int result = parse_exact(ps, ";");
  if (!result) {
    ps->index = original_index;
    return 0;
  }

  AST_VarDecl* decl = malloc(sizeof(AST_VarDecl));
  decl->node.kind = AST_KIND_DECL;
  decl->name = name;
  decl->type = type;
  add_child(c, (AST_Node*)decl);
  return 1;
}

int parse_struct(parse_state* ps, AST_Struct* s) {
  int start_index = ps->index;
  int r = 1;

  // Optionally parse typedef
  int td = parse_exact(ps, "typedef");
  r &= parse_exact(ps, "struct");

  parse_word(ps);
  s->name = ps->word;
  r &= parse_exact(ps, "{");

  for (;;) {
    if(!parse_field(ps, &s->children)) break;
  }

  r &= parse_exact(ps, "}");
  int td_name = parse_word(ps);
  r &= parse_exact(ps, ";");

  if (td_name && !td ||!td_name && td) r = 0;
  if (!r) ps->index = start_index;

  s->node.kind = AST_KIND_STRUCT;
  return r;
}

