


example: reflect
	make -C examples generics
	./build/generics

reflect:
	@make -C src
	@mkdir -p build
	@mv src/reflect build
