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
    // start with a default size, which can be expanded later
    //   if the size of the heap grows
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
     Note: I do not strictly follow the assignment here in terms of how
       to implement the Bubble Up. There is a slightly faster way to implement
       this that I implemented as an extension. It does not require swapping
       elements as the assignment's methodology does.
       In performing speed tests, this method consistently performed a tad
       faster. For "completeness", I have at the bottom of this function a
       commmented-out implementation that is strictly according to the book
       and passes all unit tests.
     
     This insertion step works by starting at the bottom of the tree and
       pulling down the tree to create room for the new cell at the proper
       location, thereby requiring no swaps.
     */
    int nextLocation;
    for (nextLocation = ++listSize;
         nextLocation > 1 && value < storage[nextLocation / 2];
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

    /* Below is an implementation of Bubble Up that is structly according
         to the assignment.
     int nextLocation;
     nextLocation = ++listSize;
     storage[nextLocation] = value;
     for (nextLocation = listSize;
          nextLocation > 1 && value < storage[nextLocation / 2];
          nextLocation /= 2) {
         string temp = storage[nextLocation / 2];
         storage[nextLocation / 2] = storage[nextLocation];
         storage[nextLocation] = temp;
     }
     */
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
	
    // Bubble Down as Heap is unsorted
    recursivelyBubbleDown(1);
    
    // Return min elem
	return minElem;
}

/*
 * Recursively bubble down the tree. Ensure that for a given element number,
 *  all its children are larger than that specific elemment number.
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