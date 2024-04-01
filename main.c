
#define BL_STRING_BUILDER_IMPLEMENTATION
#include <bl/string_builder.h>
#include "reflect.h"

BL_REFLECT_PRINT(epic);
struct epic {
  float e;
  float p;
  float i;
  float c;
};

int main() {
  epic obj = {1,2};
  print_epic(&obj);

  return 0;
}
