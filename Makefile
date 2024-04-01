
codegen:
	mkdir -p gen
	tcc -Iinclude parse.c build.c -run reflect.c main.c

run: codegen
	tcc -Iinclude gen/*.c -run main.c
	@rm gen/* # For ensuring everything works when iterating!

compile:
	mkdir -p gen
	gcc -g reflect.c parse.c build.c -Iinclude -o reflect && ./reflect main.c
	gcc -Iinclude main.c gen/*.c && ./a.out
