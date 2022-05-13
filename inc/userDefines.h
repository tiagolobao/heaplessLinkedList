/**
 * @file userDefines.h
 * @author Tiago Lobao (tiago.blobao@gmail.com)
 * @brief This was a way to create a different types of linked lists without 
 * templates that only C++ provides
 */

#ifndef HLL_USERDEFINES_H
#define HLL_USERDEFINES_H

#include <inttypes.h>
#include <stdbool.h>

/*****************************************
 ********* user changable defines ********
 *****************************************/

// This is the type of data stored inside the linked list
// It does not support strucs, but maybe you can store a pointer
#ifndef HEAPLESS_LIST_TYPE 
#define HEAPLESS_LIST_TYPE uint16_t
#endif

// This is the maximum number of elements the list can have
#ifndef HEAPLESS_LIST_MAX_SIZE 
#define HEAPLESS_LIST_MAX_SIZE 300
#endif

/*****************************************
 ********* processing user defines *******
 *****************************************/

typedef HEAPLESS_LIST_TYPE tListData;

// choosing the type of the index accordingly
#if HEAPLESS_LIST_MAX_SIZE < 0xFF
    typedef uint8_t tIndex;
    #define HLL_NULL 0xFF
#elif HEAPLESS_LIST_MAX_SIZE > 0xFF && HEAPLESS_LIST_MAX_SIZE < 0xFFFF
    typedef uint16_t tIndex;
    #define HLL_NULL 0xFFFF
#elif HEAPLESS_LIST_MAX_SIZE > 0xFFFF
    typedef uint32_t tIndex;
    #define HLL_NULL 0xFFFFFFFF
#else
    #error Index 0xFFFF | 0xFF is used for empty list. Please use another LIST_MAX_SIZE
#endif

#endif /* HLL_USERDEFINES_H */