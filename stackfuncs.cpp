#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stackfuncs.h"

void StackInit(stack_t* stk)
{  

    stk->leftCanary  = LEFT_STRUCT_CANARY;
    stk->size        = 0;
    stk->capacity    = sizeof(canary_t);
    stk->data        = (elem_t*)calloc((2 * CANARY_SIZE + 1), sizeof(elem_t));
    stk->rightCanary = RIGHT_STRUCT_CANARY; 

    placeCanary(stk, 0, LEFT_DATA_CANARY);
    placeCanary(stk, 1 + CANARY_SIZE, RIGHT_DATA_CANARY);

    ASSERTHARD(stk); 
}

void placeCanary(stack_t* stk, size_t place, canary_t canary)
{
    ASSERTHARD(stk);

    canary_t* temp = (canary_t*)calloc(1, sizeof(canary_t));

    *temp = canary;

    ASSERTSOFT(temp, NULLPTR);

    memcpy(stk->data + place, temp, sizeof(canary_t));
    
    free(temp);

    ASSERTHARD(stk);
}

void reallocStack(stack_t* stk, const int resize)
{
    ASSERTHARD(stk);

    switch (resize)
      {
        case EXPAND:

            // elem_t* temp = (elem_t*)realloc(stk->data, (stk->capacity * 2 + sizeof(canary_t) * 2));

            // ASSERTSOFT(temp, NULLPTR);

            stk->data = (elem_t*)realloc(stk->data, (stk->capacity * 2 + sizeof(canary_t) * 2)); //temp;
            stk->capacity *= 2;
            placeCanary(stk, CANARY_SIZE + stk->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
            break;

        case SHRINK:

            // elem_t* temp = (elem_t*)realloc(stk->data, (stk->capacity / 2 + sizeof(canary_t) * 2));

            // ASSERTSOFT(temp, NULLPTR);

            stk->data = (elem_t*)realloc(stk->data, (stk->capacity * 2 + sizeof(canary_t) * 2)); //temp;
            stk->capacity /= 2;
            placeCanary(stk, CANARY_SIZE + stk->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
            break;
        
        default:

            break;
      } 

    ASSERTHARD(stk);   
}
    

void Push(stack_t* stk, elem_t value)
{
    ASSERTHARD(stk);

    if (stk->capacity < sizeof(elem_t) * (stk->size + 1))
        reallocStack(stk, EXPAND);
      

    stk->data[CANARY_SIZE + stk->size++] = value;

    poisonFill(stk);

    ASSERTHARD(stk);
}

void Pop(stack_t* stk)
{
    ASSERTHARD(stk);

    if (stk->capacity > 2 * (stk->size + 1) * sizeof(elem_t))
        reallocStack(stk, SHRINK);

    stk->data[CANARY_SIZE + stk->size--] = POISON;

    poisonFill(stk);

    ASSERTHARD(stk);
}

void poisonFill(stack_t* stk)
{
    ASSERTHARD(stk);

    for(size_t i = CANARY_SIZE + stk->size; i < CANARY_SIZE + stk->capacity / sizeof(elem_t); i++)
        stk->data[i] = POISON;

    ASSERTHARD(stk);
}

void StackDtor(stack_t* stk)
{
    free(stk->data);

}

void PrintStack(const stack_t* stk)
{
    printf("\tleft data canary = %llx\n", stk->data[0]);

    for (size_t i = CANARY_SIZE; i < stk->capacity / sizeof(elem_t) + CANARY_SIZE; i++)
    if (stk->data[i] == POISON)
        printf("\t\t ! [%d] %" FORMAT " (POISON!)\n",i - CANARY_SIZE, stk->data[i]);
    else
        printf("\t\t   [%d] %" FORMAT "\n",i - CANARY_SIZE, stk->data[i]);
    
    printf("\tright data canary = %llx\n", stk->data[CANARY_SIZE + stk->capacity / sizeof(elem_t)]);
}

void stackDump(const stack_t* stk, const char* filename, const int lineNum, const char* functionName)
{
    FILE* fp = fopen("log.txt", "w+");

    fprintf(fp, "Stack [%p], ERROR #%u, in file %s, line %d, function: %s\n", stk, stackVerify(stk), filename, lineNum, functionName);
    fprintf(fp, "{\n");
    fprintf(fp, "\tleft struct canary = %llx\n", stk->leftCanary);
    fprintf(fp, "\tsize = %llu\n", stk->size);
    fprintf(fp, "\tcapacity = %llu\n", stk->capacity);
    fprintf(fp, "\tright struct canary = %llx\n", stk->rightCanary);
    fprintf(fp, "\n");
    fprintf(fp, "\tdata[%p]:\n", stk->data);
    fprintf(fp, "\n"); 

    fprintf(fp, "\tleft data canary = %llx\n", stk->data[0]);

    for (size_t i = CANARY_SIZE; i < stk->capacity / sizeof(elem_t) + CANARY_SIZE; i++)
    if (stk->data[i] == POISON)
        fprintf(fp, "\t\t ! [%d] %" FORMAT " (POISON!)\n",i - CANARY_SIZE, stk->data[i]);
    else
        fprintf(fp, "\t\t   [%d] %" FORMAT "\n",i - CANARY_SIZE, stk->data[i]);
    
    fprintf(fp, "\tright data canary = %llx\n", stk->data[CANARY_SIZE + stk->capacity / sizeof(elem_t)]);
    fprintf(fp, "}\n");

    fclose(fp);

}

ErrorCode stackVerify(const stack_t* stk)
{
    int error = 0;

    if (stk == NULL)
        error |= NULLPTR_STACK;
    if (stk->data == NULL)
        error |= NULLPTR_DATA;
    if (stk->size > stk->capacity / sizeof(elem_t))
        error |= SIZE_BIGGER_CAPACITY;
    /*
    if (stk->data[0] != LEFT_DATA_CANARY)
        error |= LCANARY_DATA_CHANGED;
    if (stk->data[CANARY_SIZE + stk->capacity / sizeof(elem_t)] != RCANARY_DATA_CHANGED)
        error |= RCANARY_DATA_CHANGED;
    */    
    if (stk->leftCanary != LEFT_STRUCT_CANARY)
        error |= LCANARY_STRUCT_CHANGED;
    if (stk->rightCanary != RIGHT_STRUCT_CANARY)
        error |= RCANARY_STRUCT_CHANGED;
    if (stk->capacity > MAX_STACK_SIZE)
        error |= MAX_CAPACITY_OVERFLOW;

    return error;
}

