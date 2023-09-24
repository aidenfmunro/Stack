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

    //ASSERTHARD(stk); 

    return 0;
}

void placeCanary(stack_t* stk, size_t place, canary_t canary)
{
    canary_t* temp = (canary_t*)calloc(1, sizeof(canary_t));

    *temp = canary;

    memcpy(stk->data + place, temp, sizeof(canary_t));
    
    free(temp);
}

void reallocStack(stack_t* stk, const int resize)
{
    switch (resize)
      {
        case EXPAND:
            stk->data = (elem_t*)realloc(stk->data, stk->capacity * 2 + sizeof(canary_t) * 2);
            stk->capacity *= 2;
            placeCanary(stk, CANARY_SIZE + stk->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
            break;

        case SHRINK:
            stk->data = (elem_t*)realloc(stk->data, stk->capacity / 2 + sizeof(canary_t) * 2);
            stk->capacity /= 2;
            placeCanary(stk, CANARY_SIZE + stk->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
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

    poisonFill(stk);

    // ASSERTHARD(stk);

    return 0;
}

ErrorCode Pop(stack_t* stk)
{
    // ASSERTHARD(stk);

    if (stk->capacity > 2 * (stk->size + 1) * sizeof(elem_t))
        reallocStack(stk, SHRINK);

    stk->data[CANARY_SIZE + stk->size--] = POISON;

    poisonFill(stk);

    // ASSERTHARD(stk);

    return 0;
}

void poisonFill(stack_t* stk)
{
    for(size_t i = CANARY_SIZE + stk->size; i < CANARY_SIZE + stk->capacity / sizeof(elem_t); i++)
        stk->data[i] = POISON;
}

ErrorCode StackDtor(stack_t* stk)
{
    free(stk->data);

    return 0;
}

ErrorCode PrintStack(stack_t* stk)
{
    printf("\tleft data canary = %llx\n", stk->data[0]);

    for (size_t i = CANARY_SIZE; i < stk->capacity / sizeof(elem_t) + CANARY_SIZE; i++)
        printf("%d\n", stk->data[i]);
    
    printf("\tright data canary = %llx\n", stk->data[CANARY_SIZE + stk->capacity / sizeof(elem_t)]);
    return 0;
}

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* functionName)
{
    printf("Stack [%p], ERROR #%d, in file %s, line %s, \
           function: %s \n", stk, stackVerify(stk), filename, lineNum, functionName);
    printf("{\n");
    printf("\tleft struct canary = %llx\n", stk->leftCanary);
    printf("\tsize = %llu\n", stk->size);
    printf("\tcapacity = %llu\n", stk->capacity);
    printf("\tright struct canary = %llx\n", stk->rightCanary);
    printf("\tdata[%p]:\n", stk->data);


    printf("}\n");
}

ErrorCode stackVerify(stack_t* stk)
{
    int error = 0;

    if (stk == NULL)
        error |= NULLPTR_STACK;
    if (stk->data == NULL)
        error |= NULLPTR_DATA;

    return error;
}
