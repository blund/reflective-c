

example: reflect
	./build/reflect examples/vec3.c
	gcc -Iinclude -Isrc examples/vec3.c gen/*.c -o build/example
	./build/example

.PHONY: reflect
reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build

