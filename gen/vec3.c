#include <bl/string_builder.h>
struct vec3 {
  float x;
  float y;
  float z;
};
void print_vec3(struct vec3* obj) {
  string_builder* b = new_builder(128);
  add_to(b, "struct vec3  { ");
  add_to(b, "x: %f ", obj->x);
  add_to(b, "y: %f ", obj->y);
  add_to(b, "z: %f ", obj->z);
  add_to(b, "};");
  printf("%s\n", to_string(b));
};

