
reflect:
	gcc -g -I../include ast.c parse.c emit.c reflect.c -o reflect

run:
	tcc -g -I../include ast.c parse.c emit.c -run reflect.c
