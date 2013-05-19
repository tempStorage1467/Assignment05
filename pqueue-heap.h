/**********************************************
 * File: pqueue-heap.h
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 * A priority queue class backed by a binary heap.
 */
#ifndef PQueue_Heap_Included
#define PQueue_Heap_Included

#include <string>
#include <iostream>
#include "simpio.h"
using namespace std;

/* A class representing a priority queue backed by an
 * binary heap.
 */
class HeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a binary heap. */
	HeapPriorityQueue();

	/* Cleans up all memory allocated by this priority queue. */
	~HeapPriorityQueue();

	/* Returns the number of elements in the priority queue. */
	int size();

	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();

	/* Enqueues a new string into the priority queue. */
	void enqueue(string value);

	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();

private:
    // Size of the queue
    int listSize;

    // Num of elements that have been allocated for storage (eg., max capacity)
    int allozatedSize;

    // array of strings; used to store the data in the binary heap tree
    string* storage;
    
    // initial capacity in the heap tree
    const int DEFAULT_CAPACITY = 32;
    
    // expand the number of elements that can be stored
    void expandStorage();
    
    // bubble down from an element, therby ensuring that all children are
    //   larger than their parent
    void recursivelyBubbleDown(int elemNum);
    
    // swap elements
    void swapElem(int elemNumA, int elemNumB);

    // print the tree, which is useful for debugging
    void printTree();
};

#endif
