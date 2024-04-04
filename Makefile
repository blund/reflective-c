
generics: codegen-generics
	gcc -Iinclude -Isrc examples/generics.c gen/generics/*.c -o build/generics
	./build/generics


structs: codegen-structs
	gcc -Iinclude -Isrc examples/structs.c gen/structs/*.c -o build/structs
	./build/structs

codegen-generics: reflect clear_gen
	@mkdir -p gen/generics
	./build/reflect -o ./gen/generics examples/generics.c

codegen-structs: reflect clear_gen
	@mkdir -p gen/structs
	./build/reflect -o ./gen/structs examples/structs.c

clear_gen:
	@mkdir -p gen
	@rm -rf gen/*


reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build
