/*******************************************************
 * File: pqueue-doublylinkedlist.h
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 * A priority queue class backed by an unsorted, doubly-linked list.
 */
#ifndef PQueue_DoublyLinkedList_Included
#define PQueue_DoublyLinkedList_Included

#include <string>
#include <iostream>
#include "simpio.h"

using namespace std;

/* Type: Cell
 * A cell in a linked list.
 */
struct DlCell {
	string value;
	DlCell* next = NULL;
	DlCell* previous = NULL;
};

/* A class representing a priority queue backed by an unsorted,
 * doubly-linked list.
 */
class DoublyLinkedListPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a doubly-linked list. */
	DoublyLinkedListPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~DoublyLinkedListPriorityQueue();
	
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
    // doubly linked list
    DlCell* list;
    
    // the number of cells in the linked list
    int numCells;
    
    // find the smallest cell in the list
    DlCell* findMinCell();
    
    // print the linked list; useful for debugging
    void printLinkedList();
};

#endif
