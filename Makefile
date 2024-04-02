

example: codegen
	gcc -Iinclude -Isrc examples/vec3.c gen/*.c -o build/example
	./build/example

codegen: reflect
	@mkdir -p gen
	./build/reflect examples/vec3.c

.PHONY: reflect
reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build

