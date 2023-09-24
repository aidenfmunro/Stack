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
    Push(&stk, 90);
 

    PrintStack(&stk);

}
