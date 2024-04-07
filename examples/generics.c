#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>


#define BL_ARR_IMPLEMENTATION
#include <bl/arr.h>
#include "reflect.h"

#define GEN_SPECIALIZE_IMPORT
#include "gen/generics/gen.c"

// Define a generic function, using the internals from <bl/arr.>
REFLECT_GENERIC(
    void arr_add($T1 arr, $T2 a) {
    if (*arr == 0) {
        arr_init((void**)arr, sizeof($T2));
    }

    struct arr_info* info = get_info(*arr);
    if (info->index >= info->capacity) {
        arr_expand((void**)arr, sizeof($T2));
        info = get_info(*arr); // @NOTE - avoid aliasing if we get a realloc in expand
    }

    (*arr)[info->index] = a;
    info->index += 1;

    }
REFLECT_GENERIC_END
);

REFLECT_GENERIC(
    void arr_del($T1 arr, $T2 index) {
      arr_info* info = get_info(arr);
      _arr__delete((void**)arr, sizeof($T2), index);
    }
REFLECT_GENERIC_END
);



REFLECT_SPECIALIZE(
    void arr_add(int** arr, int value)
);

REFLECT_SPECIALIZE(
    void arr_del(int** arr, int index)
);



REFLECT_DERIVE_PRINT(vec3);
struct vec3 {
  float x;
  float y;
  float z;
};

int main() {
  int* arr = NULL;

  for (int i = 0; i < 3; i++) {
    arr_add(&arr, i);
  }

  arr_del(&arr, 0);

  vec3 v = {1,2,3};
  print_vec3(&v);

  return 0;
}
