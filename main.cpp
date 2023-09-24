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
    printf("%llu\n", stk.data[0]);
    printf("%d\n", stk.data[1]);
    printf("%d\n", stk.data[2]);
    printf("%p\n", stk.data[3]);
    printf("%p\n", stk.data[4]);
}
