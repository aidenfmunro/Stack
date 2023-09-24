#ifndef STACKFUNCS_H
#define STACKFUNCS_H

#include <stdlib.h>
#include <math.h>

typedef int ErrorCode;

typedef unsigned long long canary_t;

const int MAX_STACK_SIZE = 4000 * sizeof(canary_t);

const canary_t LEFT_STRUCT_CANARY = 0xDEADAAAA;
const canary_t RIGHT_STRUCT_CANARY = 0xDEADBBBB;

const canary_t LEFT_DATA_CANARY = 0xDEADEEEE;
const canary_t RIGHT_DATA_CANARY = 0xDEADFFFF;

#define INT_T

#ifdef INT_T
    #define FORMAT "d"
    typedef int elem_t;
    const elem_t POISON = INT_MAX;
    const int CANARY_SIZE = 2;
#endif

#ifdef FLOAT_T
    #define FORMAT "f"
    typedef float elem_t;
    const elem_t POISON = NAN;
    const int CANARY_SIZE = 2;
#endif

#ifdef DOUBLE_T
    #define FORMAT "lg"
    typedef double elem_t;
    const elem_t POISON = NAN;
    const int CANARY_SIZE = 1;
#endif

#ifdef CHAR_T
    #define FORMAT "c"
    typedef char elem_t;
    const elem_t POISON = '!';
    const int CANARY_SIZE = 8;
#endif

#ifdef STRING_T
    #define FORMAT "s"
    typedef char* elem_t;
    const elem_t POISON = "POISON";
    const int CANARY_SIZE = 8;
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
    SIZE_BIGGER_CAPACITY    = 4,
    LCANARY_DATA_CHANGED    = 8,
    RCANARY_DATA_CHANGED    = 16,
    LCANARY_STRUCT_CHANGED  = 32,
    RCANARY_STRUCT_CHANGED  = 64,
    MAX_CAPACITY_OVERFLOW   = 128,
};


typedef struct Stack
{
    canary_t leftCanary;

    elem_t* data;
    size_t capacity;
    size_t size;

    canary_t rightCanary;

} stack_t;

void StackInit(stack_t* stk);

void reallocStack(stack_t* stk, const int resize);

void placeCanary(stack_t* stk, size_t place, canary_t canary);

void poisonFill(stack_t* stk);

void StackDtor(stack_t* stk);

void Push(stack_t* stk, elem_t value);

void Pop(stack_t* stk);

void PrintStack(stack_t* stk);

ErrorCode stackVerify(stack_t* stk);

void stackDump(stack_t* stk, const char* filename, const int lineNum, const char* functionName);

#endif
