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
    return (tIndex) (n - l->linkedList) ;
}

/*****************************************
 ********* public functions **************
 *****************************************/

// --------------------------------------------------------------------
void heaplessList_init(heaplessList* l, tIndex* ringBufferArray, heaplessListNode* heapArray, tIndex maxSize)
{
    tIndex i;

    //check inputs
    if( NULL_PTR == l || NULL_PTR == ringBufferArray ||
        NULL_PTR == heapArray || 0u == maxSize ){
        HLL_ASSERT(false,1);
        return;
    }

    l->linkedList = heapArray; 
    l->cMaxLength = maxSize;
    l->firstNodeIndex = HLL_NULL;
    l->lastNodeIndex = HLL_NULL;
    ringBuffer_init( &(l->allocationTable), ringBufferArray, maxSize );

    // add the list of free spaces in the ringBuffer
    for( i = 0; i < maxSize; i++ ){
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
        if( isOperationOk ){ // means the list was not empty
            if( l->lastNodeIndex == l->firstNodeIndex ){ // means it the last element to be removed
                l->lastNodeIndex = HLL_NULL;
                l->firstNodeIndex = HLL_NULL;
            }
            else{
                nodeIndex = l->linkedList[l->firstNodeIndex].nextNode;
                l->linkedList[nodeIndex].previousNode = HLL_NULL;
                l->firstNodeIndex = nodeIndex;
            }
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
        if( isOperationOk ){ // means the list was not empty
            if( l->lastNodeIndex == l->firstNodeIndex ){ // means is the last element to be removed
                l->lastNodeIndex = HLL_NULL;
                l->firstNodeIndex = HLL_NULL;
            }
            else{
                nodeIndex = l->linkedList[l->lastNodeIndex].previousNode;
                l->linkedList[nodeIndex].nextNode = HLL_NULL;
                l->lastNodeIndex = nodeIndex;
            }
        }
    }
    return isOperationOk;
}

// --------------------------------------------------------------------
heaplessListNode* heaplessList_initIt(heaplessList* l)
{
    heaplessListNode* it;

    if( HLL_NULL == l->firstNodeIndex )
        it = NULL_PTR;
    else
        it = &( l->linkedList[l->firstNodeIndex] );
    return it;
}

// --------------------------------------------------------------------
heaplessListNode* heaplessList_initItEnd(heaplessList* l)
{
    heaplessListNode* it;

    if( HLL_NULL == l->lastNodeIndex )
        it = NULL_PTR;
    else
        it = &( l->linkedList[l->lastNodeIndex] );
    return it;
}

// --------------------------------------------------------------------
bool heaplessList_nextIt(heaplessList* l, heaplessListNode** n)
{
    bool isNotLastElement;

    if( NULL_PTR == *n ){
        isNotLastElement = false;
    }
    if(HLL_NULL == (*n)->nextNode){
        isNotLastElement = false;
    }
    else{
        *n = &( l->linkedList[(*n)->nextNode] );
        isNotLastElement = true;
    }
    return isNotLastElement;
}

// --------------------------------------------------------------------
bool heaplessList_previousIt(heaplessList* l, heaplessListNode** n)
{
    bool isNotFirstElement;

    if( NULL_PTR == *n ){
        isNotFirstElement = false;
    }
    if(HLL_NULL == (*n)->previousNode){
        isNotFirstElement = false;
    }
    else{
        *n = &( l->linkedList[(*n)->previousNode] );
        isNotFirstElement = true;
    }
    return isNotFirstElement;
}

// --------------------------------------------------------------------
tListData heaplessList_getItData(heaplessListNode* n)
{
    return NULL_PTR == n ? ~0 : n->data;
}

// --------------------------------------------------------------------
bool heaplessList_removeAndNextIt(heaplessList* l, heaplessListNode** n)
{
    bool isOperationOk = true;

    if( NULL_PTR == *n ){ //No elements in the list or pointer not initialized
        isOperationOk = false;
    }
    else if(HLL_NULL == (*n)->previousNode){ // isFirstElement
        isOperationOk = heaplessList_removeFirst(l);
        *n = heaplessList_initIt(l);
    }
    else if(HLL_NULL == (*n)->nextNode){ // isLastElement
        isOperationOk = heaplessList_removeLast(l);
        *n = NULL_PTR;
    }
    else{
        // find the index to the array that stores the list, in order to free the memory
        tIndex nodeIndexinList = pointerToIndex(l, *n);
        isOperationOk = ringBuffer_addData( &(l->allocationTable), nodeIndexinList ); // free
        if( isOperationOk ){
            l->linkedList[(*n)->previousNode].nextNode = (*n)->nextNode;
            l->linkedList[(*n)->nextNode].previousNode = (*n)->previousNode;
            *n = &( l->linkedList[(*n)->nextNode] );
        }
    }
    return isOperationOk;
}