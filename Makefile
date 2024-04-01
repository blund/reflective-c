
codegen:
	tcc -Iinclude -run reflect.c main.c

run: codegen
	tcc -Iinclude gen/*.c -run main.c
	@rm gen/*

compile:
	gcc -Iinclude reflect.c && ./a.out main.c
	gcc -Iinclude main.c gen/*.c && ./a.out
	@rm a.out
