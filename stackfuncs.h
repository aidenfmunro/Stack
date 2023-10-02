#ifndef STACKFUNCS_H
#define STACKFUNCS_H

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include "userconfig.h"

typedef int ErrorCode;

typedef unsigned long long canary_t;

const uint32_t MOD_ADLER = 64;

const int MAX_STACK_SIZE = 4000 * 8;

const int DEFAULT_CAPACITY = 8;

const int NULLPTR = 0;

const canary_t LEFT_STRUCT_CANARY  = 0xDEADAAAA;
const canary_t RIGHT_STRUCT_CANARY = 0xDEADBBBB;

const canary_t LEFT_DATA_CANARY    = 0xDEADEEEE;
const canary_t RIGHT_DATA_CANARY   = 0xDEADFFFF;

#ifdef DEBUG
    #define ON_DEBUG(...) \
        __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

#ifdef INT_T
    #define FORMAT "d"
    typedef int elem_t;
    const elem_t POISON = INT_MAX;
#endif

#ifdef FLOAT_T
    #define FORMAT "f"
    typedef float elem_t;
    const elem_t POISON = NAN;
#endif

#ifdef DOUBLE_T
    #define FORMAT "lg"
    typedef double elem_t;
    const elem_t POISON = NAN;
#endif

#ifdef CHAR_T
    #define FORMAT "c"
    typedef char elem_t;
    const elem_t POISON = '!';
#endif

#ifdef STRING_T
    #define FORMAT "s"
    typedef char* elem_t;
    const elem_t POISON = "POISON";
#endif

#define ASSERTSOFT(EXPRESSION, ERROR_CODE)              \
    if (!(EXPRESSION))                                  \
      {                                                 \
        fprintf (stderr,"%s, failed at %s:%d\n",        \
         #ERROR_CODE, __FILE__, __LINE__);              \
        exit(ERROR_CODE);                               \
      }

#define ASSERTHARD(stk)                                 \
    do                                                  \
      {                                                 \
        int error = stackVerify(stk);                   \
        if (error)                                      \
          {                                             \
            DUMP(stk);                                  \
            exit(error);                                \
          }                                             \
      } while(0);
                                                       

#define DUMP(stk)                                       \
    do                                                  \
      {                                                 \
        stackDump(stk, __FILE__, __LINE__, __func__);   \
      } while(0); 

#define StackInit(stk)  \
    do                  \
      {                 \
        stackInit(&stk, #stk);       \
      } while(0);       \


enum ERRORS
{
    NULLPTR_STACK           = 1, 
    NULLPTR_DATA            = 2,
    SIZE_BIGGER_CAPACITY    = 4,
    LCANARY_DATA_CHANGED    = 8,
    RCANARY_DATA_CHANGED    = 16,
    LCANARY_STRUCT_CHANGED  = 32,
    RCANARY_STRUCT_CHANGED  = 64,
    CANARY_SIZE_CHANGED     = 128,
    MAX_CAPACITY_OVERFLOW   = 256,
    CAPACITY_LESS_DEFAULT   = 512,
    HASH_CHANGED            = 1024
};

typedef struct Stack
{
    ON_DEBUG(canary_t leftCanary);

    char* name;
    elem_t* data;
    size_t capacity;
    size_t size;
    ON_DEBUG(unsigned int hash);

    ON_DEBUG(canary_t rightCanary);

} stack_t;

void stackInit(stack_t* stk, char* name);

void reallocStack(stack_t* stk);

void placeCanary(stack_t* stk, size_t place, canary_t canary);

void poisonFill(stack_t* stk);

void StackDtor(stack_t* stk);

void Push(stack_t* stk, elem_t value);

void Pop(stack_t* stk);

elem_t Peek(const stack_t* stack);

void PrintStack(const stack_t* stk);

const char* stackStrError (const int code);

ErrorCode stackVerify(const stack_t* stk);

void stackDump(const stack_t* stk, const char* filename, const int lineNum, const char* functionName);

unsigned int hashAdler32(const stack_t* stack);

#endif
