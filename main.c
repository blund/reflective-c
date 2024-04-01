
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>

struct vec3 {
  float x;
  float y;
  float z;
};

void print_vec3(struct vec3* obj);

int main() {
  struct vec3 obj = {1,2,3};
  print_vec3(&obj);

  return 0;
}
