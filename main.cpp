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
    Push(&stk, 80);
    Push(&stk, 90);
    Push(&stk, 100);
    Push(&stk, 110);
    Push(&stk, 110);
    Push(&stk, 120);
    Push(&stk, 130);
    Push(&stk, 140);
    Push(&stk, 150);
    Push(&stk, 160);
    Push(&stk, 170);
    Push(&stk, 180);
    Push(&stk, 190);
    Push(&stk, 200);
    Push(&stk, 210);
    Push(&stk, 220);
    Push(&stk, 230);

    // PrintStack(&stk);

    stackDump(&stk, __FILE__, __LINE__, __func__);

}
