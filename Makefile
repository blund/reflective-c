

example: codegen
	gcc -Iinclude -Isrc examples/structs.c gen/*.c -o build/structs
	./build/structs

codegen: reflect
	@mkdir -p gen
	@rm -f gen/*
	./build/reflect examples/structs.c

.PHONY: reflect
reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build

