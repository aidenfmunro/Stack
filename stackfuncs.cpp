#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stackfuncs.h"

ErrorCode StackInit(stack_t* stk)
{  
    stk->leftCanary  = LEFT_STRUCT_CANARY;
    stk->size        = 0;
    stk->capacity    = sizeof(elem_t);
    stk->data        = (elem_t*)calloc((2 * CANARY_SIZE + 1), sizeof(elem_t));
    stk->rightCanary = RIGHT_STRUCT_CANARY; 

    placeCanary(stk, 0, LEFT_DATA_CANARY);
    placeCanary(stk, 1 + CANARY_SIZE, RIGHT_DATA_CANARY);

    PrintStack(stk);
    //ASSERTHARD(stk); 

    return 0;
}

void placeCanary(stack_t* stk, size_t place, canary_t canary)
{
    canary_t* temp = (canary_t*)calloc(1, sizeof(canary_t));

    *temp = canary;

    memcpy(stk->data + place, temp, sizeof(canary_t));
    printf("%llu\n", *(stk->data + place));
    free(temp);
}

void reallocStack(stack_t* stk, const int resize)
{
    switch (resize)
      {
        case EXPAND:
            stk->data = (elem_t*)realloc(stk->data, stk->capacity * 2 + sizeof(canary_t) * 2);
            stk->capacity *= 2;

            break;

        case SHRINK:
            stk->data = (elem_t*)realloc(stk->data, stk->capacity / 2 + sizeof(canary_t) * 2);
            stk->capacity /= 2;

            break;
        
        default:
            break;
      }    
}
    

ErrorCode Push(stack_t* stk, elem_t value)
{
    // ASSERTHARD(stk);

    if (stk->capacity < sizeof(elem_t) * (stk->size + 1))
        reallocStack(stk, EXPAND);
      

    stk->data[CANARY_SIZE + stk->size++] = value;
    placeCanary(stk, (CANARY_SIZE + stk->size), RIGHT_DATA_CANARY);

    // ASSERTHARD(stk);

    return 0;
}

ErrorCode Pop(stack_t* stk)
{
    // ASSERTHARD(stk);

    if (stk->capacity > 2 * (stk->size + 1) * sizeof(elem_t))
        reallocStack(stk, SHRINK);

    // stk->data[CANARY_SIZE + stk->size--] = POISON;

    stk->size--;
    placeCanary(stk, (CANARY_SIZE + stk->size) , RIGHT_DATA_CANARY);

    // ASSERTHARD(stk);

    return 0;
}

ErrorCode StackDtor(stack_t* stk)
{
    free(stk->data);

    return 0;
}

ErrorCode PrintStack(stack_t* stk)
{
    for (size_t i = 0; i < stk->capacity / sizeof(elem_t) + CANARY_SIZE * 2; i++)
        printf("%d\n", stk->data[i]);
    printf("---------------------------------\n");
    return 0;
}

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* function)
{
    printf("Stack [%p], ERROR ..., \n", stk);
    printf("{\n");
    printf("\tleft struct canary = %llx\n", stk->leftCanary);
    printf("\tsize = %llu\n", stk->size);
    printf("\tcapacity = %llu\n", stk->capacity);
    printf("\tright struct canary = %llx\n", stk->rightCanary);
    printf("\tdata[%p]:\n", stk->data);

    PrintStack(stk);
    printf("}\n");
}

ErrorCode stackVerify(stack_t* stk)
{
    int error = 0;

    if (stk == NULL)
        error |= NULLPTR_STACK;
    if (stk->data == NULL)
        error |= NULLPTR_DATA;
    if (stk->size < 0)
        error |= NEGATIVE_SIZE;
    if (stk->capacity < 0)
        error |= NEGATIVE_CAPACITY;

    return error;
}
