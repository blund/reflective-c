#ifndef REFLECT_H
#define REFLECT_H
#define REFLECT_DERIVE_PRINT(name) typedef struct name name; void print_##name ( struct name* obj )

#define REFLECT_GENERIC(a) asm("nop");
#define REFLECT_GENERIC_END

#define REFLECT_SPECIALIZE(a) asm("nop")
#endif

