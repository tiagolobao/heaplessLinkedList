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
inline static void __incrementIndex__(tIndex* i, tIndex maxSize)
{
    (*i)++; if( *i == maxSize ) *i = 0;
}

/*****************************************
 ********* public functions **************
 *****************************************/

// --------------------------------------------------------------------
void ringBuffer_init(ringBuffer* rb, tIndex* buffer, tIndex maxSize)
{   
    rb->buffer = buffer;
    rb->cMaxlenght = maxSize;
    rb->headIndex = 0u;
    rb->tailIndex = 0u;
    rb->lenght = RING_BUFFER_EMPTY;
}

// --------------------------------------------------------------------
bool ringBuffer_addData(ringBuffer* rb, tIndex data)
{
    bool wasOperationSuccessful = false;

    if( rb->lenght < rb->cMaxlenght ){
        rb->buffer[rb->headIndex] = data;
        __incrementIndex__( &(rb->headIndex), rb->cMaxlenght );
        rb->lenght++;
        wasOperationSuccessful = true;
    }

    return wasOperationSuccessful;
}

// --------------------------------------------------------------------
tIndex ringBuffer_popData(ringBuffer* rb)
{
    tIndex popedData = HLL_NULL;

    if( rb->lenght > RING_BUFFER_EMPTY ){
        popedData = rb->buffer[rb->tailIndex];
        __incrementIndex__( &(rb->tailIndex), rb->cMaxlenght );
        rb->lenght--;
    }
    
    return popedData;
}