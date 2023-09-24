#ifndef STACKFUNCS_H
#define STACKFUNCS_H

#include <stdlib.h>
#include <math.h>

typedef int ErrorCode;

typedef unsigned long long canary_t;

// const int TWO = 2;

const canary_t LEFT_STRUCT_CANARY = 0xDEADAAAA;
const canary_t RIGHT_STRUCT_CANARY = 0xDEADBBBB;

const canary_t LEFT_DATA_CANARY = 0xDEADEEEE;
const canary_t RIGHT_DATA_CANARY = 0xDEADFFFF;

#define INT_T

#ifdef INT_T
    typedef int elem_t;
    const elem_t POISON = INT_MAX;
    const int CANARY_SIZE = 2;
#endif

#ifdef FLOAT_T
    typedef float elem_t;
    const elem_t POISON = NAN;
    const int CANARY_SIZE = 2;
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

#define ASSERTHARD(stk)                                 \
    {                                                   \
        if (stackVerify((stk)))                         \
            DUMP((stk));                                \
            exit(0);                                    \
    }

#define DUMP(stk)                                       \
    {                                                   \
    stackDump((stk), __FILE__, __LINE__, __func__);     \
    }

#define COPY(a, b, size)						        \ 
    do                                                  \
      {                                                 \
        char *_a = (char*)a, *_b = (char*)b;	        \
        for (size_t i = 0; i < size; i++)	            \
	        {							                \
	          _a[i]= _b[i];			                    \
	        } 					                        \ 
      } while (0)

enum RESIZE
{
    SHRINK = 0,
    EXPAND = 1
};    

enum STATUS
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
    canary_t leftCanary;

    elem_t* data;
    size_t capacity;
    size_t size;

    canary_t rightCanary;

} stack_t;

ErrorCode StackInit(stack_t* stk);

void reallocStack(stack_t* stk, const int resize);

void placeCanary(stack_t* stk, size_t place, canary_t canary);

ErrorCode StackDtor(stack_t* stk);

ErrorCode Push(stack_t* stk, elem_t value);

ErrorCode Pop(stack_t* stk);

ErrorCode PrintStack(stack_t* stk);

ErrorCode stackVerify(stack_t* stk);

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* function);

#endif
