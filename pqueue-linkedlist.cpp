/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 *
 * Implementation file for the LinkedListPriorityQueue class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

// used for debugging code
#include <iostream>
#include "simpio.h"

/*
 * Constructor to start the list empty and initialize the list size as empty.
 */
LinkedListPriorityQueue::LinkedListPriorityQueue() {
    list = NULL;
    listSize = 0;
}

/*
 * Destructor to empty the queue and deallocate memory.
 */
LinkedListPriorityQueue::~LinkedListPriorityQueue() {
    while (!isEmpty()) {
        dequeueMin();
    }
}

/*
 * Return the size of the list.
 */
int LinkedListPriorityQueue::size() {
    return listSize;
}

/*
 * Return whether the current queue is empty.
 */
bool LinkedListPriorityQueue::isEmpty() {
	return (size() == 0);
}

/*
 * Add a new element to the queue and place it at the proper sorted location.
 */
void LinkedListPriorityQueue::enqueue(string value) {
    Cell* newCell = new Cell;
	newCell->value = value;

    // since the assignment requires storing in a sorted, singly linked
    //   list, inserting the new cell requires finding the proper location
    //   to insert the new cell, which requires traversing the list
    if (listSize == 0) {
        list = newCell;
    } else if (newCell->value < list->value) {
        Cell* temp = newCell;
        temp->next = list;
        list = temp;
    } else {
        Cell* curr = list;
        Cell* last;
        do {
            last = curr;
            curr = curr->next;
            if (curr != NULL) {
            }
        } while (curr != NULL && newCell->value > curr->value);

        newCell->next = last->next;
        last->next = newCell;
    }
    listSize++;
}

/*
 * Look at the smallest element in the queue.
 */
string LinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Cannot peek at an empty list");
	return list->value;
}

/*
 * Remove the smallest element from the queue and return it to the caller.
 */
string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Cannot peek at an empty list");
    Cell* currentSmallest = list;
    string val = currentSmallest->value;
    Cell* nextSmallest = list->next;
    list = nextSmallest;

    // delete the allocated memory to prevent a leak
    delete currentSmallest;
    listSize--;
	return val;
}

/*
 * Helper function used for debugging.
 */
void LinkedListPriorityQueue::printLinkedList() {
    cout << "going to print linked list of size " << listSize << endl;
    for (Cell* current = list;
         current != NULL;
         current = current->next) {
        if (current != NULL) {
            cout << "printing: " << current->value << endl;
        }
    }
    cout << "finished printing linked list of size " << listSize << endl;
}
