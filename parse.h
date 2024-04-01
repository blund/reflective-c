
typedef struct parse_state {
  char* base;
  int index;
  int last_len;
  char* last_word;
} parse_state;


void eat_whitespace(char* base, int* index);
int parse_exact(char* base, int* index, char* word);
int parse_word(char* base, int* index, char** result);
int parse_field(char* base, int* index, struct_node* n);
