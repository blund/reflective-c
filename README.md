# Reflective-C
`reflective-C` is an experiment in C reflective programming and metaprogramming. This means inspecting the source code and programmatically generating new code from it.
The current goal of the project is to generate print functions for arbitrary C structs, akin to `deriving Show` in Haskell programs.

The basic idea is to have a non-intrusive, editor-compatible way of doing metaprogramming in C. This is why `reflective-c` is a library on top of C as opposed to a whole new language.

## Usage
When defining your struct, prepend the `BL_REFLECT_PRINT` macro with the name of your struct. 
This will mark that `reflect` should generate a print function signature that can be used by the rest of your program. Here is an example from `examples/structs.`
    
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

The macro has two purposes:
1. It generates a function header for the `print_vec3` function, so that your compiler and LSP can know of it before it is generated 
2. It is used as a trigger for the `reflect` program to generate a function based on the struct following it

This move is inspired by Rich Harris' musings of hijacking the `$:` syntax in JavaScript for Svelte. We can add a step to our build system and expand on our C compiler's capabilities, but without confusing our compiler or LSP with new syntax.
Goto definition should even work as usual once the file is generated.


To see this in action, run the command

    $ make codegen

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

When you compile and run `examples/vec3.c`, you should see the following printed to your console.

    $ make example
    struct vec3  { x: 1.000000 y: 2.000000 z: 3.000000 };

Here, the final compiled file is using the print function generated by `reflect`.
When compiling the file `examples/vec3.c`, all the files in the `gen` folder are linked:

    gcc -Iinclude -Isrc examples/vec3.c gen/*.c -o build/example

## Shortcomings and further work
This project is very much an experiment in its early beginnings!
So far, there are a few limitations:
* Only structs can be read by the parser
* Only basic C types are supported (int, float, char, char*). Other types will simply lead to errors
* Every struct tagged with `BL_REFLECT_PRINT` requires its children to be either the C primitives above or also tagged with `BL_REFLECT_PRINT`.

Expanding the parser to comply with more of C's syntax will allow more use cases. 
Some interesting ideas to explore further are:
* Generic functions (lists, hash maps etc...)
* General serialization generators (json, csv)
* Creating initialization functions for structs
* Deriving equality checks for structs
* Deriving hashes from structs
* AST analysis (custom errors)
* Creating a utility for safe pattern matching
