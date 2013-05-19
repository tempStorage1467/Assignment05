/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 * Implementation file for the DoublyLinkedListPriorityQueue class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

/*
 * Constructor to initialize the linked list.
 */
DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
    numCells = 0;
    list = NULL;
}

/*
 * Destructor to deallocate all memory from the linked list.
 */
DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    while (!isEmpty()) {
        dequeueMin();
    }
}

/*
 * Return the size of the linked list.
 */
int DoublyLinkedListPriorityQueue::size() {
    return numCells;
}

/*
 * Return whether the list is empty.
 */
bool DoublyLinkedListPriorityQueue::isEmpty() {
    return (numCells == 0);
}

/*
 * Add a string to the list
 */
void DoublyLinkedListPriorityQueue::enqueue(string value) {
    DlCell* newCell = new DlCell;
    newCell->value = value;
    
    if (list == NULL) {
        // List is empty.
        list = newCell;
    } else {
        // Append new node to the end of the list.
        list->previous = newCell;
        newCell->next = list;
        list = newCell;
    }
    numCells++;
}

/*
 * Return the smallest element in the list.
 */
string DoublyLinkedListPriorityQueue::peek() {
    if (isEmpty()) error("Cannot peek at an empty list");
    DlCell* min = findMinCell();
    return min->value;
}

/*
 * Return the smallest element in the queue and remove it from the queue.
 */
string DoublyLinkedListPriorityQueue::dequeueMin() {
    if (isEmpty()) error("Empty queue");
    DlCell* toRemove = findMinCell();
    string val = toRemove->value;
    if (numCells == 1) {
        // List only contains one element, so it will be empty after removal
        list = NULL;
    } else if (toRemove->next == NULL) {
        // last element in list removed, so the new final cell is empty
        toRemove->previous->next = NULL;
    } else if (toRemove->previous == NULL) {
        // first element in list removed, so the new first cell should have
        //   no previous and the list now starts at a new location
        toRemove->next->previous = NULL;
        list = toRemove->next;
    } else {
        // removing an element that is not on an end
        toRemove->next->previous = toRemove->previous;
        toRemove->previous->next = toRemove->next;
    }
    
    // deallocate memory for the removed cell
    delete toRemove;
    numCells--;
	return val;
}

/*
 * Find the smallest cell in the list.
 */
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

/*
 * Print the linked list, helpful for debugging.
 */
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