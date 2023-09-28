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
    Push(&stk, 60);
    Push(&stk, 70);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Push(&stk, 50);
    Push(&stk, 60);
    Push(&stk, 70);
    Push(&stk, 50);
    Push(&stk, 60);
    Push(&stk, 70);
    Push(&stk, 100);
    Push(&stk, 900);

    // PrintStack(&stk);

    stackDump(&stk, __FILE__, __LINE__, __func__);
}
