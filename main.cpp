#include <stdio.h>
#include <stdlib.h>
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
    Push(&stackwow, 70);
    Push(&stackwow, 80);

    //stackDump(&stackwow, __FILE__, __LINE__, __func__);
    PrintStack(&stackwow);
    StackDtor(&stackwow);
}