

generics:
	@rm -rf gen/*
	@mkdir -p gen/generics/specialized
	../build/reflect -o gen/generics generics.c
	@mkdir -p ../build
	gcc -I../include -I../src generics.c gen/generics/*.c -o ../build/generics

structs:
	@mkdir -p gen/structs
	@rm -rf gen/structs/*
	../build/reflect -o ./gen/structs structs.c
	@mkdir -p ../build
	gcc -I../include -I../src structs.c gen/structs/*.c -o ../build/structs
