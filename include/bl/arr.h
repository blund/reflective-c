
#ifndef BL_ARR_H
#define BL_ARR_H
#include <malloc.h>

typedef struct arr_info {
  int elem_size;
  int capacity;
  int index;
} arr_info;

arr_info* get_info(void* arr);
void arr_init(void** arr, int size);
void arr_expand(void** arr, int size);

#ifdef BL_ARR_IMPLEMENTATION

arr_info* get_info(void* arr) {
  if (arr == 0) return 0;
  return (arr_info*)((char*)arr - sizeof(arr_info));
}

void arr_init(void** arr, int size) {
  if (*arr == 0) {
    arr_info* info = malloc(sizeof(arr_info) + 16*size);
    info->elem_size = size;
    info->capacity = 4;
    info->index = 0;
    *arr = (void*)((char*)info + sizeof(arr_info));
  }
}

void arr_expand(void** arr, int size) {
  arr_info* info = get_info(*arr);
  info->capacity *= 2;
  arr_info* ptr = realloc(info, info->capacity*size + sizeof(arr_info));
  if (info != ptr) {
    *arr = (void*)((char*)ptr + sizeof(arr_info));
  }
}

int arr_len(void* arr) {
  return get_info(arr)->index;
}

#endif
#endif
