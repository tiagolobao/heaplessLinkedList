
buildUtest: utest.c heaplessLinkedList.c
    gcc -o utest.exe ./src/heaplessLinkedList.c ./src/ringBuffer.c ./utest/test.c -I ./inc

dbgUtest: 
	gcc -og utest.exe ./src/heaplessLinkedList.c ./src/ringBuffer.c ./utest/test.c -I ./inc