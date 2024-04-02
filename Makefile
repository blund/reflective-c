

example: codegen
	gcc -Iinclude -Isrc examples/vec3.c gen/*.c -o build/vec3
	gcc -Iinclude -Isrc examples/person.c gen/*.c -o build/person
	./build/vec3
	./build/person

codegen: reflect
	@mkdir -p gen
	./build/reflect examples/vec3.c
	./build/reflect examples/person.c

.PHONY: reflect
reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build

