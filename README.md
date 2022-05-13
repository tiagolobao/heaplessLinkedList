# heaplessLinkedList
linked list C library for applications that does not have heap (like microcontrolers)

## Installation
To be done. I just started writing the code!

## What is this libray?
This is a way of having linked lists without dynamic allocation with functions like malloc. You may need this if you want to keep lower complexity [ O(1) ] algorithms for adding and removing elements when you don't have heap

## Limitations / Known Issues
Unfortunately it is necessary to know previously the maximum size of the list, in order to allocate the memory
