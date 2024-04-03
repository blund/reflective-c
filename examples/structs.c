
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>
#include <reflect.h>

BL_REFLECT_PRINT(vec3);
struct vec3 {
  float x;
  float y;
  float z;
};

BL_REFLECT_PRINT(person);
struct person {
  char* name;
  int age;
  vec3 speed;
};


int main() {
  vec3 obj = {1.0, 2.0, 3.0};
  print_vec3(&obj);

  person person = {"Børge", 27, {1.0, 2.0, 3.0}};
  print_person(&person);
  return 0;
}
