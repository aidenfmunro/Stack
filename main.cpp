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

    PrintStack(&stk);
    /*
    printf("%llx\n", stk.data[0]);
    printf("%d\n", stk.data[2]);
    printf("%d\n", stk.data[3]);
    printf("%d\n", stk.data[4]);
    printf("%d\n", stk.data[5]);
    printf("%llu\n", stk.data[6]);
    printf("%llx\n", stk.data[7]);
    */
}
