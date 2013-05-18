/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
    numCells = 0;
    list = NULL;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    while (!isEmpty()) {
        dequeueMin();
    }
}

int DoublyLinkedListPriorityQueue::size() {
    return numCells;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
    return (numCells == 0);
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
    DlCell* newCell = new DlCell;
    newCell->value = value;
    if (list == NULL) {
        list = newCell;
    } else {
        list->previous = newCell;
        newCell->next = list;
        list = newCell;
    }
    numCells++;
    printLinkedList();
}

string DoublyLinkedListPriorityQueue::peek() {
    if (isEmpty()) error("Cannot peek at an empty list");
    DlCell* min = findMinCell();
    return min->value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
    if (isEmpty()) error("Empty queue");
    DlCell* toRemove = findMinCell();
    string val = toRemove->value;
    if (numCells == 1) {
        list = NULL;
    } else if (toRemove->next == NULL) {
        toRemove->previous->next = NULL;
    } else if (toRemove->previous == NULL) {
        toRemove->next->previous = NULL;
        list = toRemove->next;
    } else {
        toRemove->next->previous = toRemove->previous;
        toRemove->previous->next = toRemove->next;
    }
    delete toRemove;
    numCells--;
    printLinkedList();
	return val;
}

DlCell* DoublyLinkedListPriorityQueue::findMinCell() {
    DlCell* smallestCell = list;
    int i = 0;
    for (DlCell* nextCell = list;
         i < numCells;
         nextCell = nextCell->next, i++) {
        if (nextCell->value < smallestCell->value) {
            smallestCell = nextCell;
        }
    }
    return smallestCell;
}

void DoublyLinkedListPriorityQueue::printLinkedList() {
    return;
    cout << "going to print linked list of size " << endl;
    for (DlCell* current = list;
         current != NULL;
         current = current->next) {
        if (current != NULL) {
            cout << "printing: " << current->value << endl;
        }
    }
    cout << "finished printing linked list of size " << endl;
}