
#include <string.h>
#include <malloc.h>

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
  case ',': break;
  case ';': break;
  case '}': break;
  case '{': break;
  case '(': break;
  case ')': break;
  case '*': break;
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

int parse_token(parse_state* ps) {
  eat_whitespace(ps);
  int start = ps->index;

 next:
  switch(*(ps->base + ps->index)) {
  case ' ':
  case ',':
  case ';':
  case '}':
  case '{':
  case '(':
  case ')':
  case '*':
  case '\n': {
    // @NOTE - we do not pass on the token
    ps->index++; return 1;
  } break;
  default: {
    ps->index++; goto next;
  }
  }

  return ps->index - start;
}



int parse_field(parse_state* ps, AST_Children* c) {
  int original_index = ps->index;
  int word_len;

  char* type;
  char* name;

  // We make a special case for char* since it is annoying
  // to make an exception for the only primitive type that has a *
  // in it
  int string = parse_exact(ps, "char*");

  if (string) {
    word_len = 5;
    type = "char*";
  } else {
    word_len = parse_word(ps);
    type = ps->word;
    if (!word_len) return 0;
  }

  int ptr = parse_exact(ps, "*");

  word_len = parse_word(ps);
  name = ps->word;
  if (!word_len) return 0;

  int result = parse_exact(ps, ";");
  if (!result) {
    ps->index = original_index;
    return 0;
  }

  AST_VarDecl* decl = malloc(sizeof(AST_VarDecl));
  decl->node.kind = AST_KIND_DECL;
  decl->node.flags = 0;
  decl->name = name;
  decl->type = type;


  // Mark the node as a pointer if we found a '*' above!
  // @NOTE that char* is handled separately above
  // This is because it is easier to lump it in with the
  // other C types where we just emit a format string
  if (ptr) decl->node.flags |= AST_NODE_PTR;

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


int parse_fun(parse_state* ps, AST_Func* f) {
  parse_word(ps);
  f->ret = ps->word;
  parse_word(ps);
  f->name = ps->word;
  parse_exact(ps, "(");
  parse_word(ps);
  f->p1 = ps->word;
  parse_token(ps);
  parse_token(ps);
  parse_word(ps);
  parse_exact(ps, ",");
  parse_word(ps);
  f->p2 = ps->word;
  parse_word(ps);
  parse_exact(ps, ")");

  return 1;
}

int parse_until(parse_state* ps, char* word) {
  for(;;) {
    int result = parse_exact(ps, word);
    if (result) break;
    parse_token(ps);

    // There are no more BL_REFLCET_PRINT statements in the given file
    if (ps->index >= ps->buf_len-1) return 0;
  }
  return 1;
}
