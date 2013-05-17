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
        /*
         hello
         zoo
         tom
         abc
         inside
         */
        Cell* curr = list;
        Cell* last;
        do {
            last = curr;
            curr = list->next;
        } while (curr != NULL && newCell->value >curr->value);
        
        /*
        for (curr = list;
             newCell->value > curr->value && curr->next != NULL;
             curr = curr->next) {
            
        }
         */
            /*
        while (newCell->value > curr->value) {
            cout << newCell->value << " is larger than " << curr->value << endl;
            if (curr->next == NULL) {
                break;
            } else {
                curr = curr->next;
            }
        }
             */
        newCell->next = last->next;
        last->next = newCell;
    }
    listSize++;
    cout << listSize << endl;
    printLinkedList();
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Cannot peek at an empty list");
	return list->value;
}

string LinkedListPriorityQueue::dequeueMin() {
    printLinkedList();
	if (isEmpty()) error("Cannot peek at an empty list");
    Cell* currentSmallest = list;
    string val = currentSmallest->value;
    Cell* nextSmallest = list->next;
    if (nextSmallest->next != NULL) {
        // we have more than one element in the list, so move
        //   the pointer to the first (and smallest) element left
        //   in the list after we remove the current one
        list = nextSmallest;
    }

    listSize--;
    delete currentSmallest; // delete the allocated memory to prevent a leak
	return val;
}

void LinkedListPriorityQueue::printLinkedList() {
    cout << "going to print linked list" << endl;
    for (Cell* current = list;
         current != NULL;
         current = current->next) {
        cout << "printing: " << current->value << endl;
    }
}
