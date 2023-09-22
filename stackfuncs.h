#ifndef STACKFUNCS_H
#define STACKFUNCS_H

#include <stdlib.h>
#include <math.h>

typedef int ErrorCode;

const int RESIZE = 2;


const unsigned long long LEFT_STRUCT_CANARY = 0xDEADAAAA;
const unsigned long long RIGHT_STRUCT_CANARY = 0xDEADBBBB;

const unsigned long long LEFT_DATA_CANARY = 0xDEADEEEE;
const unsigned long long RIGHT_DATA_CANARY = 0xDEADFFFF;

#define INT_T

#ifdef INT_T
    typedef int elem_t;
    const elem_t POISON = INT_MAX;
#endif

#ifdef FLOAT_T
    typedef float elem_t;
    const elem_t POISON = NAN;
#endif

#ifdef DOUBLE_T
    typedef double elem_t;
    const elem_t POISON = NAN;
#endif

#ifdef CHAR_T
    typedef char elem_t;
    const elem_t POISON = '!';
#endif

#ifdef STRING_T
    typedef char* elem_t;
    const elem_t = "POISON";
#endif

#define ASSERT_HARD(stk)                          \
    {                                             \
        if (stackVerify(&(stk)) == STACK_WORKING) \
            Dump(&(stk));                         \       
    }

#define DUMP(stk) \
    {                                                    \
    stackDump(&(stk), __FILE__, __LINE__, __func__);     \                                       
    }

enum STACK_STATUS
{
    STACK_BROKEN  = 0,
    STACK_WORKING = 1
};

enum ERRORS
{
    NULLPTR_STACK           = 1,
    NULLPTR_DATA            = 2,
    NEGATIVE_SIZE           = 4,
    NEGATIVE_CAPACITY       = 8,
    LCANARY_DATA_CHANGED    = 16,
    RCANARY_DATA_CHANGED    = 32,
    LCANARY_STRUCT_CHANGED  = 64,
    RCANARY_STRUCT_CHANGED  = 128,
    MAX_CAPACITY_OVERFLOW   = 264,
};


typedef struct Stack
{
    elem_t* data;
    size_t capacity,
               size;
} stack_t;

ErrorCode StackInit(stack_t* stk);

ErrorCode StackDtor(stack_t* stk);

ErrorCode Push(stack_t* stk, elem_t value);

ErrorCode Pop(stack_t* stk);

ErrorCode PrintStack(stack_t* stk);

void Dump(stack_t* stk);

ErrorCode stackVerify(stack_t* stk);

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* function);

#endif
