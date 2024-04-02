
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>
#include <reflect.h>

BL_REFLECT_PRINT(person);
struct person {
  char* name;
  int age;
};

int main() {
  person person = {"Børge", 27};
  print_person(&person);

  return 0;
}
