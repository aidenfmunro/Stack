#include <stdio.h>
#include "stackfuncs.h"

ErrorCode main()
{
    stack_t stack = {};

    StackInit(&stack);

    Push(&stack, 10);
    Push(&stack, 20);
    Push(&stack, 30);
    Push(&stack, 40);
    Pop(&stack);

    /* 
    stack.size = 0; // hash change
    */

    Push(&stack, 10);

    stackDump(&stack, __FILE__, __LINE__, __func__);
    PrintStack(&stack);
    StackDtor(&stack);
}