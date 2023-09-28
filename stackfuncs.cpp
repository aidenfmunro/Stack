#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "stackfuncs.h"

void StackInit(stack_t* stk)
{  

    stk->leftCanary  = LEFT_STRUCT_CANARY;
    stk->size        = 0;
    stk->capacity    = sizeof(canary_t);
    stk->data        = (elem_t*)calloc(3, sizeof(canary_t));
    stk->rightCanary = RIGHT_STRUCT_CANARY; 
    
    placeCanary(stk, 0, LEFT_DATA_CANARY);
    placeCanary(stk, 4, RIGHT_DATA_CANARY);

    stk->data = (elem_t*)((void*)stk->data + sizeof(canary_t));

    ASSERTHARD(stk); 
}

void placeCanary(stack_t* stack, size_t place, canary_t canary)
{
    ASSERTHARD(stack);

    memcpy(stack->data + place, &canary, sizeof(canary_t));
    
    ASSERTHARD(stack);
}

void reallocStack(stack_t* stack, const int resize)
{
    ASSERTHARD(stack);

    switch (resize)
      {
        case EXPAND:

            // elem_t* temp = (elem_t*)realloc(stk->data, (stk->capacity * 2 + sizeof(canary_t) * 2));

            // ASSERTSOFT(temp, NULLPTR);

            stack->data = (elem_t*)realloc((elem_t*)((void*)stack->data - sizeof(canary_t)), (stack->capacity * 2 + sizeof(canary_t) * 2));
            stack->data = (elem_t*)((void*)stack->data + sizeof(canary_t));
            stack->capacity *= 2;
            placeCanary(stack, stack->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
            break;

        case SHRINK:

            // elem_t* temp = (elem_t*)realloc(stk->data, (stk->capacity / 2 + sizeof(canary_t) * 2));

            // ASSERTSOFT(temp, NULLPTR);

            stack->data = (elem_t*)realloc((elem_t*)((void*)stack->data - sizeof(canary_t)), (stack->capacity / 2 + sizeof(canary_t) * 2)); 
            stack->data = (elem_t*)((void*)stack->data + sizeof(canary_t));
            stack->capacity /= 2;
            placeCanary(stack, stack->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);
            break;
        
        default:

            break;
      } 

    ASSERTHARD(stack);   
}
    

void Push(stack_t* stk, elem_t value)
{
    ASSERTHARD(stk);

    if (stk->capacity < sizeof(elem_t) * (stk->size + 1))
        reallocStack(stk, EXPAND);
      

    stk->data[stk->size++] = value;

    poisonFill(stk);

    ASSERTHARD(stk);
}

void Pop(stack_t* stk)
{
    ASSERTHARD(stk);

    if (stk->capacity > 2 * (stk->size + 1) * sizeof(elem_t) && stk->capacity > sizeof(canary_t))
        reallocStack(stk, SHRINK);

    stk->data[stk->size--] = POISON;

    poisonFill(stk);

    ASSERTHARD(stk);
}

void poisonFill(stack_t* stk)
{
    ASSERTHARD(stk);

    for(size_t i = stk->size; i < stk->capacity / sizeof(elem_t); i++)
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

    for (size_t i = 0; i < stk->capacity / sizeof(elem_t); i++)
    if (stk->data[i] == POISON)
        printf("\t\t ! [%d] %" FORMAT " (POISON!)\n", i, stk->data[i]);
    else
        printf("\t\t   [%d] %" FORMAT "\n", i, stk->data[i]);
    
    printf("\tright data canary = %llx\n", stk->data[stk->capacity / sizeof(elem_t)]);
}

const char* StackStrErr (int code)
    {
    #define CODE_(code)  case code: return #code;

    switch (code)
        {
        CODE_ (NULLPTR_STACK)
        CODE_ (NULLPTR_DATA)
        CODE_ (SIZE_BIGGER_CAPACITY)
        CODE_ (LCANARY_DATA_CHANGED)
        CODE_ (RCANARY_DATA_CHANGED)
        CODE_ (LCANARY_STRUCT_CHANGED)
        CODE_ (RCANARY_STRUCT_CHANGED)
        CODE_ (MAX_CAPACITY_OVERFLOW)
        CODE_ (CANARY_SIZE_CHANGED)
        }

    return "**UNKNOWN**";

    #undef CODE_    
    }

void stackDump(const stack_t* stk, const char* filename, const int lineNum, const char* functionName)
{
    time_t t;
    time(&t);

    FILE* fp = fopen("logbad.txt", "w+");
    fprintf(fp, "This log file was made at: %s\n", ctime(&t));

    int error = stackVerify(stk);
    fprintf(fp, "Stack [%p], ERROR #%u (%s), in file %s, line %d, function: %s\n", stk, error, StackStrErr(error), filename, lineNum, functionName);
    fprintf(fp, "{\n");
    fprintf(fp, "\tleft struct canary = 0x%llx\n", stk->leftCanary);
    fprintf(fp, "\tsize = %llu\n", stk->size);
    fprintf(fp, "\tcapacity = %llu\n", stk->capacity);
    fprintf(fp, "\tright struct canary = 0x%llx\n", stk->rightCanary);
    fprintf(fp, "\n");
    fprintf(fp, "\tdata[%p]:\n", stk->data);
    fprintf(fp, "\n"); 
    fprintf(fp, "\tleft data canary = 0x%llx\n", *(elem_t*)((void*)stk->data - sizeof(canary_t)));

    for (size_t i = 0; i < stk->capacity / sizeof(elem_t); i++)
    if (stk->data[i] == POISON)
        fprintf(fp, "\t\t ! [%d] %" FORMAT " (POISON!)\n", i, stk->data[i]);
    else
        fprintf(fp, "\t\t   [%d] %" FORMAT "\n", i, stk->data[i]);
    
    fprintf(fp, "\tright data canary = 0x%llx\n", stk->data[stk->capacity / sizeof(elem_t)]);
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
    if (*(elem_t*)((void*)stk->data - sizeof(canary_t)) != LEFT_DATA_CANARY)
        error |= LCANARY_DATA_CHANGED;
    if (*(elem_t*)((void*)stk->data + stk->capacity) != RCANARY_DATA_CHANGED)
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

/*
uint32_t hashAdler32(const stack_t* stk)
{
    uint32_t a = 1, b = 0;
    size_t index;
    
    // Process each byte of the data in order

    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}
*/