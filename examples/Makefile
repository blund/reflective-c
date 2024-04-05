

generics: codegen
	@mkdir -p build
	gcc -I../include -I../src generics.c gen/*.c -o ../build/generics

codegen:
	@mkdir -p gen
	@rm -rf gen/*
	../build/reflect -o ./gen generics.c
