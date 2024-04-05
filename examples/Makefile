

generics:
	@mkdir -p gen/generics
	@rm -rf gen/generics/*
	../build/reflect -o ./gen/generics generics.c
	@mkdir -p ../build
	gcc -I../include -I../src generics.c gen/*.c -o ../build/generics

structs:
	@mkdir -p gen/structs
	@rm -rf gen/structs/*
	../build/reflect -o ./gen/structs structs.c
	@mkdir -p ../build
	gcc -I../include -I../src structs.c gen/*.c -o ../build/structs
