#include <stdio.h>
#include "stackfuncs.h"

ErrorCode main()
{
    stack_t stk = {};

    StackInit(&stk);

    Push(&stk, 10);

    Push(&stk, 20);

    Push(&stk, 30);

    Pop(&stk);

    Push(&stk, 50);

    PrintStack(&stk);
}
