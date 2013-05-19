/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 * Implementation file for the HeapPriorityQueue class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

/*
 * Constructor to initialize an array of strings.
 */
HeapPriorityQueue::HeapPriorityQueue() {
    storage = new string[DEFAULT_CAPACITY];
    allozatedSize = DEFAULT_CAPACITY;
    listSize = 0;
}

/*
 * Destructor to deallocate memory for strings.
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] storage;
}

/*
 * Return the size of the queue.
 */
int HeapPriorityQueue::size() {
    return listSize;
}

/*
 * Return whether the queue is empty.
 */
bool HeapPriorityQueue::isEmpty() {
    return (size() == 0);
}

/*
 * Add a new string to the queue. Add it such that each parent is smaller
 *   than its children.
 */
void HeapPriorityQueue::enqueue(string value) {
    // Check whether underlying array needs to be expanded to store new elems
    if (listSize + 1 >= allozatedSize) {
        expandStorage();
    }

    // Bubble Up
    /*
     Start at the bottom of the tree in the next element position, which
       is the list size + 1 (this takes into account the fact that the first
       node in the tree is at element 1 and not element 0.
     Ensure that the nextLocation to check for insertion is always
       greater than 1 (which is the top of the tree) and make sure the
       value we're trying to insert is smaller than its parent (otherwise
       we will go too far up the tree in finding the final location for the
       next element we're trying to insert).
     As we move up the tree, as long as the child in the path we're traversing
       up from the new element to the root is smaller than its parent,
       we swap such that a child is always lexically larger than its parent.
     */
    int nextLocation;
    for (nextLocation = ++listSize;
         nextLocation > 1 && value < storage[nextLocation/2];
         nextLocation /= 2) {
        storage[nextLocation] = storage[nextLocation / 2];
    }
    /*
     Since we have gone as far up the tree as we can such that the value
       we are trying to insert is less than its child, we're at the place
       we should now insert since going any further would cause a parent
       to be larger than its child.
     */
    storage[nextLocation] = value;
    printTree();
}

/*
 * Return the smallest element in the queue.
 */
string HeapPriorityQueue::peek() {
    if (isEmpty()) error("no elements in queue");
	return storage[1];
}

/*
 * Remove the smallest element in the queue and return it.
 */
string HeapPriorityQueue::dequeueMin() {
    if (isEmpty()) error("no elements in queue");
    // Swap top and bottom elements
    string minElem = storage[1];
    storage[1] = storage[listSize];
    storage[listSize] = "";
    listSize--;
	
    // Bubble Down as Heap is Unsorted
    recursivelyBubbleDown(1);
    
    // Return min elem
	return minElem;
}

/*
 * Recursively bubble down the tree. Ensure that for a given element number,
 *  all its children are smaller than that specific elemment number.
 */
void HeapPriorityQueue::recursivelyBubbleDown(int elemNum) {
    if ((elemNum * 2) > listSize) {
        // Base Case: Reached Bottom of Tree; No Children
        return;
    } else {
        // Recursive Case: More Children to Potentially Swap With
        // Step 1. Find Smallest Child
        int smallestChildNum;
        if ((elemNum * 2) + 1 > listSize) {
            // No Right Child, Smallest Child is Left Child
            smallestChildNum = elemNum * 2;
        } else {
            // Right Child Exists, Compare Left & Right Child for Smallest
            if (storage[elemNum * 2] > storage[(elemNum * 2) + 1]) {
                // Right Child is Smallest
                smallestChildNum = (elemNum * 2) + 1;
            } else {
                // Left Child is Smallest
                smallestChildNum = elemNum * 2;
            }
        }

        // Step 2.A. Compare Smallest Child With Current Elem
        if (storage[elemNum] > storage[smallestChildNum]) {
            // 2.B. Child Is Smaller, Swap and Recurse
            swapElem(elemNum, smallestChildNum);
            recursivelyBubbleDown(smallestChildNum);
        }
    }
}

/*
 * Swap two elements in the underlying storage awway.
 */
void HeapPriorityQueue::swapElem(int elemNumA, int elemNumB) {
    string temp = storage[elemNumB];
    storage[elemNumB] = storage[elemNumA];
    storage[elemNumA] = temp;
}

/*
 * Expand the capacity in the underlying storage array.
 */
void HeapPriorityQueue::expandStorage() {
    string* temp = new string[allozatedSize * 2];
    copy(storage, storage + allozatedSize, temp);
    delete[] storage;
    storage = temp;
    allozatedSize *= 2;
}

/*
 * Print the heap tree, which is useful for debugging.
 */
void HeapPriorityQueue::printTree() {
    //return;
    cout << "List Size: " << listSize << endl;
    for (int elemNum = 1; elemNum <= listSize; elemNum *= 2) {
        string space = "";
        int numSpaces = 40 / elemNum;
        for (int s = 0; s < numSpaces; s++) {
            space += " ";
        }
        cout << space;
        int rowEnd = elemNum*2;
        for (int i = elemNum; i < rowEnd; i++) {
            if (storage[i].length() > 0) {
                cout << i << ", " << storage[i] << space << space;
            }
        }
        cout << endl;
    }
}