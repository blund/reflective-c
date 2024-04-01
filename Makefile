
codegen:
	tcc -I../bl -run reflect.c

run: codegen
	tcc -I../bl gen/*.c -run main.c
	@rm gen/*

compile:
	gcc -I../bl reflect.c && ./a.out
	gcc -I../bl main.c gen/*.c && ./a.out
	@rm a.out
