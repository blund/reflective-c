#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>


#define BL_ARR__IMPLEMENTATION
#include <bl/arr.h>
#include "reflect.h"

#include "../gen/generics/gen.c"


// Define a generic function, using the internals from <bl/arr.>
BL_GENERIC(
    void arr_add($T** arr, $T a) {
    if (*arr == 0) {
        arr_init((void**)arr, sizeof($T));
    }

    struct arr_info* info = get_info(*arr);
    if (info->index >= info->capacity) {
        arr_expand((void**)arr, sizeof($T));
        info = get_info(*arr); // @NOTE - avoid aliasing if we get a realloc in expand
    }

    (*arr)[info->index] = a;
    info->index += 1;

    }
BL_GENERIC_END
);

BL_SPECIALIZE(
    void arr_add(int** arr, int value)
);

int main() {

  int* arr = NULL;

  for (int i = 0; i < 4; i++) {
    arr_add(&arr, i);
    arr_info* info = get_info(arr);
    printf("%d\n", info->index);
  }

  return 0;
}
