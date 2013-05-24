/*******************************************************
 * File: pqueue-linkedlist.h
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 *
 * A priority queue class backed by a sorted, singly-linked list.
 */
#ifndef PQueue_LinkedList_Included
#define PQueue_LinkedList_Included

#include <string>
using namespace std;

/* Type: Cell
 * A cell in a linked list.
 */
struct Cell {
	string value;
	Cell* next = NULL;
};

/* A class representing a priority queue backed by a sorted,
 * singly-linked list.
 */
class LinkedListPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a sorted linked list. */
	LinkedListPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~LinkedListPriorityQueue();
	
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
    // Linked list
    Cell* list;

    // Size of the linked list. Instance variable used to store this so that
    //   size can operate in O(1) and not have to traverse the entire list.
    int listSize;

    // helper function useful for debugging
    void printLinkedList();
};

#endif
