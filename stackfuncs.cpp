#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "stackfuncs.h"

void StackInit(stack_t* stack)
{  

    stack->leftCanary  = LEFT_STRUCT_CANARY;
    stack->size        = 0;
    stack->capacity    = sizeof(canary_t);
    stack->data        = (elem_t*)calloc(3, sizeof(canary_t));
    stack->hash        = hashAdler32(stack);
    stack->rightCanary = RIGHT_STRUCT_CANARY; 
    
    placeCanary(stack, 0, LEFT_DATA_CANARY);
    placeCanary(stack, 4, RIGHT_DATA_CANARY);

    stack->data = (elem_t*)((void*)stack->data + sizeof(canary_t));

    stack->hash = hashAdler32(stack);

    ASSERTHARD(stack); 
}

void placeCanary(stack_t* stack, size_t place, canary_t canary)
{
    ASSERTHARD(stack);

    memcpy(stack->data + place, &canary, sizeof(canary_t));

    stack->hash = hashAdler32(stack);
    
    ASSERTHARD(stack);
}

void reallocStack(stack_t* stack)
{
    ASSERTHARD(stack);

    if (stack->capacity < sizeof(elem_t) * (stack->size + 1))
      {
        stack->data = (elem_t*)realloc((elem_t*)((void*)stack->data - sizeof(canary_t)), (stack->capacity * 2 + sizeof(canary_t) * 2));
        stack->data = (elem_t*)((void*)stack->data + sizeof(canary_t));
        stack->capacity *= 2;
      }
    else if (stack->capacity > 2 * (stack->size + 1) * sizeof(elem_t) && stack->capacity > sizeof(canary_t))
      {
        stack->data = (elem_t*)realloc((elem_t*)((void*)stack->data - sizeof(canary_t)), (stack->capacity / 2 + sizeof(canary_t) * 2)); 
        stack->data = (elem_t*)((void*)stack->data + sizeof(canary_t));
        stack->capacity /= 2;
      }

    stack->hash = hashAdler32(stack);

    placeCanary(stack, stack->capacity / sizeof(elem_t), RIGHT_DATA_CANARY);  
}

void Push(stack_t* stack, elem_t value)
{
    ASSERTHARD(stack);

    reallocStack(stack);
      
    stack->data[stack->size++] = value;

    stack->hash = hashAdler32(stack);

    poisonFill(stack);
}

void Pop(stack_t* stack)
{
    ASSERTHARD(stack);

    if (stack->capacity > 2 * (stack->size + 1) * sizeof(elem_t) && stack->capacity > sizeof(canary_t))
        reallocStack(stack);

    stack->data[stack->size--] = POISON;

    stack->hash = hashAdler32(stack);

    poisonFill(stack);
}

void poisonFill(stack_t* stack)
{
    ASSERTHARD(stack);

    for(size_t i = stack->size; i < stack->capacity / sizeof(elem_t); i++)
        stack->data[i] = POISON;

    stack->hash = hashAdler32(stack);

    ASSERTHARD(stack);
}

void StackDtor(stack_t* stack)
{
    free(stack->data);
}

void PrintStack(const stack_t* stack)
{
    printf("\tleft data canary = %llx\n", stack->data[0]);

    for (size_t i = 0; i < stack->capacity / sizeof(elem_t); i++)
    if (stack->data[i] == POISON)
        printf("\t\t ! [%d] %" FORMAT " (POISON!)\n", i, stack->data[i]);
    else
        printf("\t\t   [%d] %" FORMAT "\n", i, stack->data[i]);
    
    printf("\tright data canary = %llx\n", stack->data[stack->capacity / sizeof(elem_t)]);
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
        CODE_ (HASH_CHANGED)
        }

    return "UNKNOWN";

    #undef CODE_    
}

void stackDump(const stack_t* stack, const char* filename, const int lineNum, const char* functionName)
{
    time_t t;
    time(&t);

    FILE* fp = fopen("logbad.txt", "w+");
    fprintf(fp, "This log file was made at: %s\n", ctime(&t));

    int error = stackVerify(stack);
    fprintf(fp, "Stack [%p], ERROR #%u (%s), in file %s, line %d, function: %s\n", stack, error, StackStrErr(error), filename, lineNum, functionName);
    fprintf(fp, "{\n");
    fprintf(fp, "\tleft struct canary = 0x%llx\n", stack->leftCanary);
    fprintf(fp, "\tsize = %llu\n", stack->size);
    fprintf(fp, "\tcapacity = %llu\n", stack->capacity);
    fprintf(fp, "\tright struct canary = 0x%llx\n", stack->rightCanary);
    fprintf(fp, "\n");
    fprintf(fp, "\tdata[%p]:\n", stack->data);
    fprintf(fp, "\n"); 
    fprintf(fp, "\tleft data canary = 0x%llx\n", *(elem_t*)((void*)stack->data - sizeof(canary_t)));

    for (size_t i = 0; i < stack->capacity / sizeof(elem_t); i++)
    if (stack->data[i] == POISON)
        fprintf(fp, "\t\t ! [%d] %" FORMAT " (POISON!)\n", i, stack->data[i]);
    else
        fprintf(fp, "\t\t   [%d] %" FORMAT "\n", i, stack->data[i]);
    
    fprintf(fp, "\tright data canary = 0x%llx\n", stack->data[stack->capacity / sizeof(elem_t)]);
    fprintf(fp, "}\n");

    fclose(fp);

}

ErrorCode stackVerify(const stack_t* stack)
{
    int error = 0;

    if (stack == NULL)
        error |= NULLPTR_STACK;
    if (stack->data == NULL)
        error |= NULLPTR_DATA;
    if (stack->size > stack->capacity / sizeof(elem_t))
        error |= SIZE_BIGGER_CAPACITY;
    /*
    if (*(elem_t*)((void*)stack->data - sizeof(canary_t)) != LEFT_DATA_CANARY)
        error |= LCANARY_DATA_CHANGED;
    if (*(elem_t*)((void*)stack->data + stack->capacity) != RCANARY_DATA_CHANGED)
        error |= RCANARY_DATA_CHANGED;
    */
    if (stack->leftCanary != LEFT_STRUCT_CANARY)
        error |= LCANARY_STRUCT_CHANGED;
    if (stack->rightCanary != RIGHT_STRUCT_CANARY)
        error |= RCANARY_STRUCT_CHANGED;
    if (stack->capacity > MAX_STACK_SIZE)
        error |= MAX_CAPACITY_OVERFLOW;
    if (stack->hash != hashAdler32(stack))
        error |= HASH_CHANGED;

    return error;
}


unsigned int hashAdler32(const stack_t* stack)
{
    unsigned int a = 1, b = 0;
    
    for (size_t i = 0; i < stack->capacity; i++)
    {
        a = (a + *(char*)((void*)stack->data + i)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}
