
run:
	tcc -I../bl -run gen.c
	tcc -I../bl main.c -run gen/vec3.c

compile:
	gcc -I../bl gen.c && ./a.out
	gcc -I../bl main.c gen/vec3.c && ./a.out
