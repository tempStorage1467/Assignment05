/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

// used for debugging code
#include <iostream>
#include "simpio.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
    list = NULL;
    listSize = 0;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	// TODO: Fill this in!
}

int LinkedListPriorityQueue::size() {
    return listSize;
}

bool LinkedListPriorityQueue::isEmpty() {
	return (size() == 0);
}

void LinkedListPriorityQueue::enqueue(string value) {
    Cell* newCell = new Cell;
	newCell->value = value;

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
           // cout << "Last: " << last->value << endl;
            curr = curr->next;
            if (curr != NULL) {
          //      cout << "Current: " << curr->value << endl;
            }
        } while (curr != NULL && newCell->value >curr->value);

        newCell->next = last->next;
        last->next = newCell;
    }
    listSize++;
    printLinkedList();
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Cannot peek at an empty list");
	return list->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Cannot peek at an empty list");
    printLinkedList();
    Cell* currentSmallest = list;
    string val = currentSmallest->value;
    Cell* nextSmallest = list->next;
    list = nextSmallest;

    // delete the allocated memory to prevent a leak
    delete currentSmallest;
    listSize--;
    printLinkedList();
	return val;
}

void LinkedListPriorityQueue::printLinkedList() {
    return;
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
