#include "stackfuncs.h"


ErrorCode StackInit(struct Stack* stk)
{
    stk->size = 0;
    stk->capacity = sizeof(elem_t);
    stk->data = (elem_t*)calloc(1, sizeof(elem_t));

    return 0; 
}

ErrorCode Push(struct Stack* stk, elem_t value)
{
    if (stk->capacity < sizeof(elem_t) * (stk->size + 1)) 
        stk->data = (elem_t*)realloc(stk->data, stk->capacity * 2);

    stk->data[stk->size++] = value;

    return 0;
}

ErrorCode Pop(struct Stack* stk)
{
    return 0;
}