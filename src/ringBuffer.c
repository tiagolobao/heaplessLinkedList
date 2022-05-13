/**
 * @file ringBuffer.c
 * @author Tiago Lobao (you@domain.com)
 * @brief ring buffer (FIFO) implementation. But this ring buffer should not overwrite anything. 
 * if you want a proper ring buffer go to something like https://github.com/dhess/c-ringbuf
 */

#include "ringBuffer.h"

/*****************************************
 ********* private functions *************
 *****************************************/

/**
 * @brief increment the index of the ring buffer accordingly, \
 * going back to zero when overlaps the max list size
 * 
 * @param i [ pointer to the index ]
 */
inline static void __incrementIndex__(tIndex* i)
{
    (*i)++; if( *i == HEAPLESS_LIST_MAX_SIZE ) *i = 0;
}

/*****************************************
 ********* public functions **************
 *****************************************/

// --------------------------------------------------------------------
void ringBuffer_init(ringBuffer* rb)
{
    rb->headIndex = 0u;
    rb->tailIndex = 0u;
    rb->lenght = RING_BUFFER_EMPTY;
}

// --------------------------------------------------------------------
bool ringBuffer_addData(ringBuffer* rb, tIndex data)
{
    bool wasOperationSuccessful = false;

    if( rb->lenght < RING_BUFFER_FULL ){
        rb->buffer[rb->headIndex] = data;
        __incrementIndex__( &(rb->headIndex) );
        rb->lenght++;
        wasOperationSuccessful = true;
    }

    return wasOperationSuccessful;
}

// --------------------------------------------------------------------
tIndex ringBuffer_popData(ringBuffer* rb)
{
    tIndex popedData = HLL_NULL;

    if( RING_BUFFER_EMPTY != rb->lenght ){
        popedData = rb->buffer[rb->tailIndex];
        __incrementIndex__( &(rb->tailIndex) );
        rb->lenght--;
    }
    
    return popedData;
}