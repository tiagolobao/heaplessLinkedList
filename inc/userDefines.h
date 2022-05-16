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
typedef uint16_t tListData;
#define HLL_NULL 0xFFFF

// Depends on the maximum number of elements in the list/array
typedef uint16_t tIndex;


#endif /* HLL_USERDEFINES_H */