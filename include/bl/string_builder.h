#ifndef BL_STRING_BUILDER_H
#define BL_STRING_BUILDER_H

#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct string_builder {
  char* buffer;
  int   capacity;
  int   index;
} string_builder;

// Forward declare functions
string_builder* new_builder(int capacity);
void add_to(string_builder* b, char* format, ...);
void reset(string_builder* b);
void free_builder(string_builder* b);
char* to_string(string_builder* b);

// Declare functions if ..._IMPLEMENTATION is defined in source file
#ifdef BL_STRING_BUILDER_IMPLEMENTATION
string_builder* new_builder(int capacity) {
  string_builder* b = (string_builder*)malloc(sizeof(string_builder));
  b->buffer   = (char*)malloc(capacity); b->capacity = capacity;
  b->index    = 0;

  return b;
}

void free_builder(string_builder* b) {
  free(b->buffer);
  free(b);
}

void add_to(string_builder* b, char* format, ...) {
  // Try writing to the buffer
  // If expected written characters exceed the buffer capacity, grow it and try again
  for (;;) {
    char* position = b->buffer + b->index;
    int   remaining_capacity = b->capacity - b->index;

    va_list args;
    va_start(args, format);
    int written = vsnprintf(position, remaining_capacity, format, args);
    va_end(args);

    if (written >= remaining_capacity) {
      b->capacity *= 2;
      b->buffer = (char*)realloc(b->buffer, b->capacity);
    } else {
      b->index += written;
      break;
    }
  }
}

void reset(string_builder* b) {
  b->index = 0;
}

char* to_string(string_builder* b) {
  b->buffer[b->index] = 0;
  return b->buffer;
}
#endif
#endif
