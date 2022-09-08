/**
 * @file heaplessLinkedList.h
 * @author Tiago Lobao (tiago.blobao@gmail.com)
 * @brief header of the heapless Linked List module
 */

#ifndef HEAPLESSLINKEDLIST_H
#define HEAPLESSLINKEDLIST_H

#include "userDefines.h"
#include "ringBuffer.h"

#ifndef NULL_PTR
#define NULL_PTR (void*)0u
#endif

/*****************************************
 ********** Type definitions *************
 *****************************************/

typedef struct{
    tIndex previousNode;
    tListData data;
    tIndex nextNode;
} heaplessListNode;

typedef struct{
    heaplessListNode* linkedList;
    ringBuffer allocationTable;
    tIndex firstNodeIndex;
    tIndex lastNodeIndex;
    tIndex cMaxLength;
} heaplessList;

/*******************************************************
 ********** function prototypes - Direct access ********
 ** Those functions will operate directly in the list **
 *******************************************************/

/**
 * @brief init an empty heapless list,
 * 
 * @param l pointer to the list
 */
void heaplessList_init(heaplessList* l, tIndex* ringBufferArray, heaplessListNode* heapArray, tIndex maxSize);

/**
 * @brief add data to the list
 * 
 * @param l pointer to the list
 * @param data to be added
 * @return true means there was space in the list
 * @return false means the list was full
 */
bool heaplessList_append(heaplessList* l, tListData data);

/**
 * @brief removes one list of the element (first), retrieving it to the caller
 * 
 * @param l pointer to the list
 * @param output poped data
 * @return true means the output data is value
 * @return false means the output data is not valid because the list was empty
 */
bool heaplessList_pop(heaplessList* l, tListData* output);

/**
 * @brief getter for the very first element of the list
 * 
 * @param l pointer to the list
 * @param output returned data from the list
 * @return true means operation successful
 * @return false means operation not successful
 */
bool heaplessList_getFirst(heaplessList* l, tListData* output);

/**
 * @brief remove first element of the list
 * 
 * @param l pointer to the list
 * @return true means operation successful
 * @return false means operation not successful
 */
bool heaplessList_removeFirst(heaplessList* l);

/**
 * @brief remove last element of the list
 * 
 * @param l pointer to the list
 * @return true means operation successful
 * @return false means operation not successful
 */
bool heaplessList_removeLast(heaplessList* l);

/*******************************************************
 ********** function prototypes - Iterator *************
 ** Those functions will operate directly in the list **
 *******************************************************/

/**
 * @brief Creates a iterator for the heapless list starting from the begginning
 * 
 * @param l pointer to the list
 * @return node pointer
 */
heaplessListNode* heaplessList_initIt(heaplessList* l);


/**
 * @brief Creates a iterator for the heapless list starting from the end
 * 
 * @param l pointer to the list
 * @return node pointer
 */
heaplessListNode* heaplessList_initItEnd(heaplessList* l);


/**
 * @brief get next node from the heaplessList
 * 
 * @param l pointer to the list
 * @param n pointer to the node
 * @return true means operation successful (the node is not the last one)
 * @return false means operation not successful (the node is the last one)
 */
bool heaplessList_nextIt(heaplessList* l, heaplessListNode** n);

/**
 * @brief get previous node from the heaplessList
 * 
 * @param l pointer to the list
 * @param n pointer to the node
 * @return true means operation successful (the node is not the last one)
 * @return false means operation not successful (the node is the last one)
 */
bool heaplessList_previousIt(heaplessList* l, heaplessListNode** n);

/**
 * @brief returns the data from the node
 * 
 * @param n pointer to the node
 * @return tListData
 */
tListData heaplessList_getItData(heaplessListNode* n);

/**
 * @brief remove the element from the list and return de next element
 * 
 * @param l pointer to the list
 * @param n pointer to the node
 * OBS: the output node will be NULL if this is the last node
 */
bool heaplessList_removeAndNextIt(heaplessList* l, heaplessListNode** n);

#endif /* HEAPLESSLINKEDLIST_H */
