/**
 * @file ringBuffer.h
 * @author Tiago Lobao (tiago.blobao@gmail.com)
 * @brief header of the ring buffer module
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include "userDefines.h"

/*****************************************
 ********** Constant definitions *********
 *****************************************/

#define RING_BUFFER_EMPTY 0u
#define RING_BUFFER_FULL HEAPLESS_LIST_MAX_SIZE


/*****************************************
 ********** Type definitions *************
 *****************************************/
typedef struct {
  tIndex buffer[HEAPLESS_LIST_MAX_SIZE];
  tIndex tailIndex;
  tIndex headIndex;
  tIndex lenght;
} ringBuffer;

/*****************************************
 ********** function prototypes **********
 *****************************************/

/**
 * @brief initialization of the ringBuffer
 * initializing values of tail and index to make an empty list
 * 
 * @param rb [ pointer to the ring buffer to be init ]
 * @return void
 */
void ringBuffer_init(ringBuffer* rb);

/**
 * @brief adds one element to the ring buffer on the first
 * free position (element on the head)
 * {This works as a "free" function}
 * 
 * @param rb [ ring buffer pointer to be added ]
 * @param data [ data to be added ]
 * @return boolean that tells if the operation was OK
 */
bool ringBuffer_addData(ringBuffer* rb, tIndex data);

/**
 * @brief removes the element located on the tail, that
 * means, the first element on the buffer
 * {This works as a "malloc" function}
 * 
 * @param rb 
 * @return tIndex with the data. returns 0xFF | 0xFFFF if list is empty 
 */
tIndex ringBuffer_popData(ringBuffer* rb);


#endif /* RINGBUFFER_H */