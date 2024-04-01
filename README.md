# Reflective-C
`reflective-C` is an experiment in C reflective programming and metaprogramming.
The current goal of the project is to generate `print_*` functions for arbitrary `structs`.

The basic idea is to have a non-intrusive, editor-compatible way of generating functions. This way, `reflective-c` can be a library on top of C as opposed to a whole new language.

## Usage
When defining your struct, prepend the `BL_REFLECT_PRINT` macro with the name of your struct. 
This will mark that `reflect` should generate a print function signature that can be used by the rest of your program. Here is an example from `main.c`
    
    BL_REFLECT_PRINT(vec3); // Note this macro!
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


To see this in action, run the command

    # With tcc
    tcc -Iinclude -run reflect.c main.c
    # Or, if you prefer gcc
    gcc -Iinclude reflect.c && ./a.out main.c

If you now look in the `gen` folder, you can see a new file, `vec3.c`. This contains the generated `print_vec3` for the struct.

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
(Note that out of convenience I am using my string builder library both in the actual code generation and in the emitted code).

This file be linked to your normal project project

    # With tcc
    tcc -Iinclude gen/*.c -run main.c
    # Or, if you prefer gcc
    gcc -Iinclude main.c gen/*.c && ./a.out

When you compile and run `main.c`, you should see the following printed to your console

    struct vec3  { x: 1.000000 y: 2.000000 z: 3.000000 };

## Shortcomings and further work
This project is very much an experiment in its early beginnings.
So far, the C parser and code emitter only work with flat struct definitions and floats.

Further work is needed for implementing the remaining C builtin types and recursive structs.
