#include <stdio.h>
#include <stdbool.h>
#include "heaplessLinkedList.h"
#include "ringBuffer.h"

int test(bool condition, char* message){
    int r = 0;
    if (!condition){
        printf("TEST FAILED: %s \n", message);
        r = -1;
    }
    return r;
}

int main(void)
{
    int testResults = 0;
    
    int count;
    bool functionReturn;

    ringBuffer my_rb;
    tIndex popedData;
    tIndex previousData;


    printf("...Starting test...\n");

    /*************************************************************************************/
    /***************************** Testing the ringbuffer ********************************/
    /*************************************************************************************/
    
    //void ringBuffer_init(ringBuffer* rb)
    // ---------------------------------------------------------
    ringBuffer_init(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 0 &&
            my_rb.tailIndex == 0 &&
            my_rb.buffer != NULL &&
            my_rb.lenght == 0
        ),
        "Ring Buffer - 1. init a buffer" 
    );

    //ringBuffer_addData
    // ---------------------------------------------------------
    functionReturn = ringBuffer_addData(&my_rb, 0x20);
    testResults |= test( 
        (
            functionReturn == true && // operation was ok
            my_rb.headIndex == 1 &&
            my_rb.tailIndex == 0 &&
            my_rb.buffer[my_rb.tailIndex] == 0x20 &&
            my_rb.lenght == 1
        ),
        "Ring Buffer - 2.1 add elements to the buffer" 
    );

    // ---------------------------------------------------------
    functionReturn = ringBuffer_addData(&my_rb, 0x32);
    testResults |= test( 
        (
            functionReturn == true && // operation was ok
            my_rb.headIndex == 2 &&
            my_rb.tailIndex == 0 &&
            my_rb.buffer[my_rb.tailIndex] == 0x20 &&
            my_rb.buffer[my_rb.headIndex - 1] == 0x32 &&
            my_rb.lenght == 2
        ),
        "Ring Buffer - 2.2 add elements to the buffer" 
    );

    // ---------------------------------------------------------
    for( count = my_rb.lenght; count < HEAPLESS_LIST_MAX_SIZE-1; count++ ){

        functionReturn = ringBuffer_addData(&my_rb, count-2); // add from 0 to MAX
        testResults |= test( 
            (
                functionReturn == true && // operation was ok
                my_rb.headIndex == my_rb.lenght &&
                my_rb.tailIndex == 0 &&
                my_rb.buffer[my_rb.tailIndex] == 0x20 &&
                my_rb.buffer[my_rb.headIndex - 1] == count-2 &&
                my_rb.lenght == (count+1)
            ),
            "Ring Buffer - 2.3 fill list till it has one free space" 
        );
    }

    // ---------------------------------------------------------
    functionReturn = ringBuffer_addData(&my_rb, 0xAA);
    testResults |= test( 
        (
            functionReturn == true &&
            my_rb.headIndex == 0u && //head completes the circle
            my_rb.tailIndex == 0 &&
            my_rb.buffer[HEAPLESS_LIST_MAX_SIZE-1] == 0xAA &&
            my_rb.lenght == HEAPLESS_LIST_MAX_SIZE
        ),
        "Ring Buffer - 2.4 fill the last free space" 
    );

    // ---------------------------------------------------------
    functionReturn = ringBuffer_addData(&my_rb, 0xAA);
    testResults |= test( 
        (
            functionReturn == false && //operation was not ok
            my_rb.headIndex == 0u &&
            my_rb.tailIndex == 0 &&
            my_rb.buffer[HEAPLESS_LIST_MAX_SIZE-1] == 0xAA &&
            my_rb.lenght == HEAPLESS_LIST_MAX_SIZE
        ),
        "Ring Buffer - 2.5 not possible to add elements anymore" 
    );



    //ringBuffer_popData
    // ---------------------------------------------------------
    previousData = my_rb.buffer[my_rb.tailIndex];
    popedData = ringBuffer_popData(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 0u &&
            my_rb.tailIndex == 1u &&
            previousData == popedData &&
            my_rb.lenght == HEAPLESS_LIST_MAX_SIZE - 1
        ),
        "Ring Buffer - 3.1 pops the first element" 
    );

    // ---------------------------------------------------------
    previousData = my_rb.buffer[my_rb.tailIndex];
    popedData = ringBuffer_popData(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 0u &&
            my_rb.tailIndex == 2u &&
            previousData == popedData &&
            my_rb.lenght == HEAPLESS_LIST_MAX_SIZE - 2
        ),
        "Ring Buffer - 3.2 pops the second element" 
    );

    // ---------------------------------------------------------
    for( count = my_rb.lenght; count > 1; count-- ){
        previousData = my_rb.buffer[my_rb.tailIndex];
        popedData = ringBuffer_popData(&my_rb);
        testResults |= test( 
            (
                my_rb.headIndex == 0u &&
                my_rb.tailIndex == HEAPLESS_LIST_MAX_SIZE - my_rb.lenght &&
                previousData == popedData &&
                my_rb.lenght == count - 1
            ),
            "Ring Buffer - 3.2 pops until there is only one element left" 
        );
    }

    // ---------------------------------------------------------
    previousData = my_rb.buffer[my_rb.tailIndex];
    popedData = ringBuffer_popData(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 0u &&
            my_rb.tailIndex == 0u &&
            previousData == popedData &&
            my_rb.lenght == 0
        ),
        "Ring Buffer - 3.3 pops the last element" 
    );

    // ---------------------------------------------------------
    popedData = ringBuffer_popData(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 0u &&
            my_rb.tailIndex == 0u &&
            popedData == HLL_NULL &&
            my_rb.lenght == 0
        ),
        "Ring Buffer - 3.4 there is no elements left" 
    );


    // ---------------------------------------------------------
    functionReturn = ringBuffer_addData(&my_rb, 0x20);
    functionReturn = ringBuffer_addData(&my_rb, 0x31);
    functionReturn = ringBuffer_addData(&my_rb, 0x32);
    functionReturn = ringBuffer_addData(&my_rb, 0x45);
    popedData = ringBuffer_popData(&my_rb);
    popedData = ringBuffer_popData(&my_rb);
    testResults |= test( 
        (
            my_rb.headIndex == 4u &&
            my_rb.tailIndex == 2u &&
            popedData == 0x31 &&
            my_rb.buffer[2] == 0x32 &&
            my_rb.buffer[3] == 0x45 &&
            my_rb.lenght == 2
        ),
        "Ring Buffer - 4.1 Ring buffer indexes in the middle of the array" 
    );

    /*************************************************************************************/
    /***************************** Testing the heaplessList ******************************/
    /*************************************************************************************/

    functionReturn = true;
    while( functionReturn )
        functionReturn = ringBuffer_addData(&my_rb, my_rb.lenght);
    while( my_rb.lenght > 10 )
        (void)ringBuffer_popData(&my_rb);

    functionReturn = ringBuffer_addData(&my_rb, 0xEE);
    testResults |= test(
        (
            functionReturn == true &&
            my_rb.lenght == 11u &&
            my_rb.buffer[my_rb.headIndex-1] == 0xEE
        ),
        "Ring Buffer - 4.2 Ring buffer indexes in the end and beggining of the array"
    );

    popedData = ringBuffer_popData(&my_rb);
    testResults |= test(
        (
            popedData == 290 &&
            my_rb.lenght == 10u &&
            my_rb.buffer[my_rb.headIndex-1] == 0xEE
        ),
        "Ring Buffer - 4.3 Ring buffer indexes in the end and beggining of the array"
    );

    return testResults;
}