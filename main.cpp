#include <stdio.h>
#include "stackfuncs.h"

int main()
{
    struct Stack stk = {};

    StackInit(&stk);

    Push(&stk, 10);
    Push(&stk, 20);
    Push(&stk, 30);
    


    for (size_t i = 0; i < stk.size + 1; i++)
        printf("%d ", stk.data[i]);
}