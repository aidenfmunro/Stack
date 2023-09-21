#ifndef STACKFUNCS_H
#define STACKFUNCS_H

#include <stdlib.h>

typedef int ErrorCode;

typedef int elem_t;

struct Stack
{
    elem_t* data;
    size_t capacity,
               size;
};

ErrorCode StackInit(struct Stack* stk);
ErrorCode Push(struct Stack* stk, elem_t value);
ErrorCode Pop(struct Stack* stk);

#endif
