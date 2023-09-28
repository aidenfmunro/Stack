#include <stdio.h>
#include "stackfuncs.h"

ErrorCode main()
{
    stack_t stack = {};

    StackInit(&stack);

    Push(&stack, 10);
    Push(&stack, 20);
    Push(&stack, 30);
    Pop(&stack);
    Push(&stack, 50);
    Push(&stack, 60);
    Push(&stack, 70);
    Pop(&stack);
    Pop(&stack);
    Pop(&stack);
    Pop(&stack);
    Push(&stack, 50);
    Push(&stack, 60);
    Push(&stack, 70);
    Push(&stack, 50);
    Push(&stack, 60);
    Push(&stack, 70);
    Push(&stack, 100);
    Push(&stack, 900);

    // PrintStack(&stk);

    stackDump(&stack, __FILE__, __LINE__, __func__);
}
