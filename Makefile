
codegen:
	tcc -Iinclude -run reflect.c main.c

run: codegen
	tcc -Iinclude gen/*.c -run main.c

compile:
	gcc -Iinclude reflect.c -o reflect && ./reflect main.c
	gcc -Iinclude main.c gen/*.c && ./a.out
