#include <stdio.h>
#include "stackfuncs.h"

ErrorCode main()
{
    stack_t stackwow = {};

    StackInit(stackwow);

    Push(&stackwow, 10);
    Push(&stackwow, 20);
    Push(&stackwow, 30);
    Push(&stackwow, 40);
    Push(&stackwow, 50);
    Pop(&stackwow);

    /* 
    stack.size = 0; // hash change
    */

    stackDump(&stackwow, __FILE__, __LINE__, __func__);
    PrintStack(&stackwow);
    StackDtor(&stackwow);
}