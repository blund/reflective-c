
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>
#include "reflect.h"

BL_REFLECT_PRINT(vec3);
struct vec3 {
  float x;
  float y;
  float z;
};

int main() {
  vec3 obj = {1.0, 2.0, 3.0};
  print_vec3(&obj);

  return 0;
}
