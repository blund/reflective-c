
#ifndef REFLECT_H
#define REFLECT_H
#define BL_REFLECT_PRINT(name) typedef struct name name; void print_##name ( struct name* obj )
#endif

