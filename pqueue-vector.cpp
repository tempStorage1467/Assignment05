/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	// TODO: Fill this in!
}

VectorPriorityQueue::~VectorPriorityQueue() {
	// TODO: Fill this in!
}

int VectorPriorityQueue::size() {
	return storage.size();
}

bool VectorPriorityQueue::isEmpty() {
	return (storage.size() == 0);
}

void VectorPriorityQueue::enqueue(string value) {
    storage.add(value);
}

string VectorPriorityQueue::peek() {
    if (isEmpty()) {
        error("The queue is empty");
    }
    int smallestIndex = getSmallestIndex();
    return storage[smallestIndex];
}

string VectorPriorityQueue::dequeueMin() {
    if (isEmpty()) {
        error("The queue is empty");
    }
    int smallestIndex = getSmallestIndex();
    string val = storage[smallestIndex];
    storage.remove(smallestIndex);
    return val;
}

int VectorPriorityQueue::getSmallestIndex() {
    if (isEmpty()) error("The queue is empty");
    int smallestIndex = 0;
    for (int i = 0; i < storage.size(); i++) {
        if (storage[i] < storage[smallestIndex]) {
            smallestIndex = i;
        }
    }
    return smallestIndex;
}

