/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 * Implementation file for the VectorPriorityQueue class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

/*
 * Return the current size of the queue.
 */
int VectorPriorityQueue::size() {
	return storage.size();
}

/*
 * Return whether the queue is empty.
 */
bool VectorPriorityQueue::isEmpty() {
	return (storage.size() == 0);
}

/*
 * Add a string to the end of the queue.
 */
void VectorPriorityQueue::enqueue(string value) {
    storage.add(value);
}

/*
 * Look at the value of the next element to be dequeued.
 */
string VectorPriorityQueue::peek() {
    if (isEmpty()) error("The queue is empty");

    int smallestIndex = getSmallestIndex();
    return storage[smallestIndex];
}

/*
 * Remove and return the smallest element in the queue.
 */
string VectorPriorityQueue::dequeueMin() {
    if (isEmpty()) error("The queue is empty");

    int smallestIndex = getSmallestIndex();
    string val = storage[smallestIndex];
    storage.remove(smallestIndex);
    return val;
}

/*
 * Get the index number of the smallest element in the queue.
 */
int VectorPriorityQueue::getSmallestIndex() {
    if (isEmpty()) error("The queue is empty");
    int smallestIndex = 0;
    
    // traverse the list and continually check whether the next cell
    //   is smaller than the currently smallest cell
    for (int i = 0; i < storage.size(); i++) {
        if (storage[i] < storage[smallestIndex]) {
            smallestIndex = i;
        }
    }
    return smallestIndex;
}
