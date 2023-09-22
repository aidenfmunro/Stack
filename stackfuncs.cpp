#include <stdio.h>
#include <math.h>
#include "stackfuncs.h"


ErrorCode StackInit(stack_t* stk)
{
    stk->size = 0;
    stk->capacity = sizeof(elem_t);
    stk->data = (elem_t*)calloc(1, sizeof(elem_t));

    return 0; 
}

ErrorCode Push(stack_t* stk, elem_t value)
{
    if (stk->capacity < sizeof(elem_t) * (stk->size + 1)) 
        stk->data = (elem_t*)realloc(stk->data, stk->capacity * RESIZE);

    stk->data[stk->size++] = value;

    return 0;
}

ErrorCode Pop(stack_t* stk)
{
    
    stk->data[stk->size] = NULL;
    stk->size--;

    return 0;
}

ErrorCode StackDtor(stack_t* stk)
{
    free(stk->data);

    return 0;
}

ErrorCode PrintStack(stack_t* stk)
{
    for (size_t i = 0; i < stk->size; i++)
        printf("%d ", stk->data[i]);
    
    return 0;
}

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* function)
{

}

ErrorCode stackVerify(stack_t* stk)
{

}