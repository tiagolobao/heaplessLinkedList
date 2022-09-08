# heaplessLinkedList
linked list C library for applications that does not have heap (like microcontrolers)

## Installation
To add it to your project, just compile the files insice src/ folder and include the files in inc/ folder
you can also run the UTEST in the test/ folder just by running make.

## What is this libray?
This is a way of having linked lists without dynamic allocation with functions like malloc. You may need this if you want to keep lower complexity [ O(1) ] algorithms for adding and removing elements when you don't have heap.

## Usage
You can check all the functions (with descriptions) that you can use inside heaplessLinkedList.h
A complete idea about how to use you can get from the unit test

### Basic usage
Here is an example

```C

#define HEAPLESS_LIST_MAX_SIZE 30

// static varables that should never be touched directly!!!
// heap memory in this case is not allocated in heap
static tIndex __allocationArray__[HEAPLESS_LIST_MAX_SIZE];
static heaplessListNode __heapMemory__[HEAPLESS_LIST_MAX_SIZE];

// Static variables used in the code
void foo(void)
{
    heaplessList my_l;
    tListData my_data;

    // initialize the list
    heaplessList_init(&my_l, __allocationArray__, __heapMemory__, HEAPLESS_LIST_MAX_SIZE);

    // now you can start adding data
    heaplessList_append(&my_l, 298u);
    heaplessList_append(&my_l, 123u);

    // and getting it back
    heaplessList_pop(&my_l, &my_data); // my_data => 298

}

```

### Iterators
Using iterators is a good idea to make some operatios faster. You can pop a data from the middle of the list
without going through the list again

```C
void bar(void)
{
    heaplessListNode* my_n;

    // Suppose that the list now is { 298, 123, 444, 555 }

    // initialize the iterator (steps from last example are necessary before this)
    my_n = heaplessList_initIt(&my_l);
    
    heaplessList_getItData(my_n); // my_n => 298

    heaplessList_nextIt(&my_l, &my_n);

    heaplessList_getItData(my_n); // my_n => 123

    //and now the magical function to remove from the middle of the list
    heaplessList_removeAndNextIt(&my_l, &my_n);

    heaplessList_getItData(my_n); // my_n => 444

    // The list here now is { 298, 444, 555 }
}

```

> OBS: check the userDefines.h file to change the type of list you want to lose


## Limitations / Known Issues
Unfortunately it is necessary to know previously the maximum size of the list, in order to allocate the memory. <br>
For now you can only append in the end of the list. Maybe next step is to add append to the begginning or add after an iterator. Or you can feel free to add them and make a PR :)