
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>
#include "reflect.h"

BL_REFLECT_PRINT(vec3);
struct vec3 {
  float x;
  float y;
  float z;
};

BL_REFLECT_PRINT(stats);
struct stats {
  int atk;
  int def;
};

BL_REFLECT_PRINT(components);
struct components {
  stats* stats;
  vec3 pos;
};

BL_REFLECT_PRINT(person);
struct person {
  char* name;
  int   age;
  components components;
};

int main() {
  vec3 pos = {1.0, 2.0, 3.0};
  print_vec3(&pos);

  stats stats = {.atk = 8, .def = 32};
  print_stats(&stats);

  person person = {"Børge", 27, {&stats, pos}};
  print_person(&person);
  return 0;
}
