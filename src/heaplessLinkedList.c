/**
 * @file heaplessLinkedList.c
 * @author Tiago Lobao (tiago.blobao@gmail.com)
 * @brief linked list without heap implementation
 * @version 0.1
 */

#include "heaplessLinkedList.h"

/*****************************************
 ********* private functions *************
 *****************************************/
inline static tIndex pointerToIndex(heaplessList* l, heaplessListNode* n)
{
    return (tIndex) ( (n - l->linkedList) / sizeof(HEAPLESS_LIST_TYPE) );
}

/*****************************************
 ********* public functions **************
 *****************************************/

// --------------------------------------------------------------------
void heaplessList_init(heaplessList* l)
{
    tIndex i;

    l->firstNodeIndex = HLL_NULL;
    l->lastNodeIndex = HLL_NULL;
    ringBuffer_init( &(l->allocationTable) );

    // add the list of free spaces in the ringBuffer
    for( i = 0; i < HEAPLESS_LIST_MAX_SIZE; i++ ){
        ringBuffer_addData( &(l->allocationTable), i );
    }
}

// --------------------------------------------------------------------
bool heaplessList_append(heaplessList* l, tListData data)
{
    tIndex i;
    bool wasOperationOk = false;

    i = ringBuffer_popData( &(l->allocationTable) ); // malloc
    if( HLL_NULL != i ){
        // very first element case, first = last
        if( HLL_NULL == l->firstNodeIndex){
            l->firstNodeIndex = i;
            l->lastNodeIndex = i;
            l->linkedList[i].nextNode = HLL_NULL;
            l->linkedList[i].previousNode = HLL_NULL;
            l->linkedList[i].data = data;
        }
        // other elements case
        else{
            l->linkedList[i].previousNode = l->lastNodeIndex;
            l->linkedList[i].nextNode = HLL_NULL;
            l->linkedList[l->lastNodeIndex].nextNode = i;
            l->lastNodeIndex = i;
            l->linkedList[i].data = data;
        }
        wasOperationOk = true;
    }
    return wasOperationOk;
}

// --------------------------------------------------------------------
bool heaplessList_pop(heaplessList* l, tListData* output)
{
    bool isListNotEmpty = false;

    isListNotEmpty = heaplessList_getFirst(l, output); // get
    isListNotEmpty |= heaplessList_removeFirst(l);
    return isListNotEmpty;
}

// --------------------------------------------------------------------
bool heaplessList_getFirst(heaplessList* l, tListData* output)
{
    bool isListNotEmpty = false;

    if( HLL_NULL != l->firstNodeIndex ){
        *output = l->linkedList[l->firstNodeIndex].data;
        isListNotEmpty = true;
    }
    return isListNotEmpty;
}

// --------------------------------------------------------------------
bool heaplessList_removeFirst(heaplessList* l)
{
    bool isOperationOk = false;
    tIndex nodeIndex;

    if( HLL_NULL != l->firstNodeIndex ){
        isOperationOk = ringBuffer_addData( &(l->allocationTable), l->firstNodeIndex ); // free
        if( isOperationOk ){
            nodeIndex = l->linkedList[l->firstNodeIndex].nextNode;
            l->linkedList[nodeIndex].previousNode = HLL_NULL;
            l->firstNodeIndex = nodeIndex;
        }
    }
    return isOperationOk;
}

// --------------------------------------------------------------------
bool heaplessList_removeLast(heaplessList* l)
{
    bool isOperationOk = false;
    tIndex nodeIndex;

    if( HLL_NULL != l->lastNodeIndex ){
        isOperationOk = ringBuffer_addData( &(l->allocationTable), l->lastNodeIndex ); // free
        if( isOperationOk ){
            nodeIndex = l->linkedList[l->lastNodeIndex].previousNode;
            l->linkedList[nodeIndex].nextNode = HLL_NULL;
            l->lastNodeIndex = nodeIndex;
        }
    }
    return isOperationOk;
}

// --------------------------------------------------------------------
heaplessListNode* heaplessList_initIt(heaplessList* l)
{
    return &( l->linkedList[l->firstNodeIndex] );
}

// --------------------------------------------------------------------
heaplessListNode* heaplessList_initItEnd(heaplessList* l)
{
    return &( l->linkedList[l->lastNodeIndex] );
}

// --------------------------------------------------------------------
bool heaplessList_nextIt(heaplessList* l, heaplessListNode* n)
{
    bool isLastElement;
    if(HLL_NULL == n->nextNode){
        isLastElement = true;
    }
    else{
        n = &( l->linkedList[n->nextNode] );
        isLastElement = false;
    }
    return isLastElement;
}

// --------------------------------------------------------------------
bool heaplessList_previousIt(heaplessList* l, heaplessListNode* n)
{
    bool isFirstElement;
    if(HLL_NULL == n->previousNode){
        isFirstElement = true;
    }
    else{
        n = &( l->linkedList[n->previousNode] );
        isFirstElement = false;
    }
    return isFirstElement;
}

// --------------------------------------------------------------------
tListData heaplessList_getItData(heaplessListNode* n)
{
    return n->data;
}

// --------------------------------------------------------------------
bool heaplessList_removeAndNextIt(heaplessList* l, heaplessListNode* n)
{
    bool isOperationOk = true;
    if(HLL_NULL == n->previousNode){ // isFirstElement
        isOperationOk = heaplessList_removeFirst(l);
        n = heaplessList_initIt(l);
    }
    else if(HLL_NULL == n->nextNode){ // isLastElement
        isOperationOk = heaplessList_removeLast(l);
        n = NULL_PTR;
    }
    else{
        // find the index to the array that stores the list, in order to free the memory
        tIndex nodeIndexinList = pointerToIndex(l, n);
        isOperationOk = ringBuffer_addData( &(l->allocationTable), nodeIndexinList ); // free
        if( isOperationOk ){
            l->linkedList[n->previousNode].nextNode = n->nextNode;
            l->linkedList[n->nextNode].previousNode = n->previousNode;
            n = &( l->linkedList[n->nextNode] );
        }
    }
    return isOperationOk;
}