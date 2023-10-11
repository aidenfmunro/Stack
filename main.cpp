#include "stackfuncs.h"

ErrorCode main()
{
    Stack stackwow = {};

    CreateStack(stackwow);
    

    Push(&stackwow, 10);
    
    Push(&stackwow, 20);
    Push(&stackwow, 30);
    Push(&stackwow, 40);
    Push(&stackwow, 50);
    Pop(&stackwow);

    
    Push(&stackwow, 80);
    
    stackDump(&stackwow, __FILE__, __LINE__, __func__);
    // PrintStack(&stackwow);
    
    DestroyStack(&stackwow);
}