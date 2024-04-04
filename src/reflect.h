#ifndef REFLECT_H
#define REFLECT_H
#define BL_REFLECT_PRINT(name) typedef struct name name; void print_##name ( struct name* obj )

#define BL_GENERIC(a) asm("nop");
#define BL_GENERIC_END

#define BL_SPECIALIZE(a) asm("nop")
#endif

