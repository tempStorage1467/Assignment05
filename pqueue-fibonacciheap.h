/**********************************************
 * File: pqueue-extra.h
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 *
 * This file defines the Fibonacci Heap. At a high level, the Fibonacci Heap
 *   works as follows:
 *
 * Create a pointer to the minimum cell. This makes finding the minimum 
 *   very easy and ensures that finding the minimum runs in O(1) time.
 * Create a pointer to the first cell in the root element list. This makes
 *   travering the roots very easy.
 * Create a circular doubly linked list of root elements. Each cell in the
 *   root list can also have a child and that child can have siblings, who
 *   are also linked using a doubly linked circular list.
 * Inserting a new cell into the structure is done by placing the new cell
 *   next to the first cell in the root list. During the insert operation,
 *   the new cell is checked against the current minimum (i.e., smallest)
 *   cell which lies in the root linked list. The minimum is updated if
 *   necessary. The total number of cells in the heap is incremented by one.
 *   This operation therefore runs in O(1) time.
 * Determinimg the total number of cells (i.e., size) as well as whether
 *   the heap is empty is as easy as accessing the cell counter, which
 *   will run in O(1) time.
 * Peeking at the minimum cell is as easy as returning the value contained
 *   in the minimum cell, which can always be located by looking at the
 *   pointer that points to the minimum cell. This runs in O(1) time.
 * Dequeueing the minimum cell, and all the attendant operations, is the
 *   most complex part of the Fibonacci Heap. The current minimum cell is
 *   accessed and the value is captured as the underlying memory will be
 *   freed up during this operation. The minimum cell is removed from the root
 *   list. Then, the children of that minimum root cell (and we know
 *   the minimum cell is always stored in the root list) are consolidated
 *   into the root's linked list. Next, the cells are consolidated. During the
 *   consolidation stage, an array is created to help keep track of the degree
 *   of all the cells in the root list. The consolidation stage ensures that
 *   no two cells in the root list have the same degree. In other words,
 *   it ensures that no two cells sit atop trees with the same number of
 *   children. Trees are merged until such a time as each cell in the root
 *   list has a separate degree.
 */
#ifndef PQueue_Extra_Included
#define PQueue_Extra_Included

#include <string>
#include <list>
#include <iostream>
#include "simpio.h"
using namespace std;

/*
 * Researching and writing the Fibonacci Heap took a little over 12 hours.
 *   I relied heavily on the resources below, which I found
 *   helpful. I thoroughly read and re-read the following articles that
 *   describe the data structure, its operations, simulations, pseudocode,
 *   full code solutions etc.
 *
 * As I researched the Fibonacci Heap online, it became clear
 *   from reading articles and looking at lots of sample code that there
 *   are some different ways of implementing this.
 *
 * In the end, while I have a program below that passes all the unit tests,
 *   I am not convinced that it is a 100% true Fibonacci Heap. For example,
 *   the way I treat marking cells is not the way that the Wikipedia article
 *   and other sites indicated it should be treated. I simply could not get
 *   things to work without this and without relying on sample code that worked
 *   the way I implement things below. The implication of this one example of
 *   being too agressive in marking nodes is that the code
 *   is only slightly less efficient, as you can see from http://programmers.stackexchange.com/questions/187410/marked-nodes-in-fibonacci-heaps
 * 
 * To look into this further, I wrote an extension that performs a detailed
 *   speed test comparison. The results for that comparison found that the
 *   most efficient solution is the HeapPriorityQueue and I would expect
 *   based upon my research that the Fibonacci Heap would actually be more
 *   efficient. In particular, my extension clocked the following speeds:
 *              Vector PQ:          29.6 seconds
 *              Double Linked List: 14.11 seconds
 *              Linked List PQ:     9.29 seconds
 *              Fibonacci Heap:     2.46 seconds
 *              Heap PQ:            0.12 seconds
 *
 * Further, getting the program, especially the consolidate() function,
 *   to work properly proved quite a bit more complex than the pseudocode
 *   made it look to be, so I relied heavily on written code "solutions" as
 *   I note below. This I think forced me to hack a few other parts of the
 *   program to get it to work, which made the PQueue less efficient.
 *
 * Nonetheless, the Fibonacci Heap provided here passes all the unit tests
 *   and runs in very fast time!
 *
 * All in all, the experience of researching and trying to write a
 *   Fibonacci Heap implementation proved to be extremely exciting and a
 *   great learning experience. I eventually ran out of time trying to fix
 *   some of the imperfections and had to settle here as my time is really
 *   limited due to my job.
 *
 * Credits:
http://en.wikipedia.org/wiki/Fibonacci_heap
http://www.cs.duke.edu/courses/fall05/cps230/L-11.pdf
http://www.cs.arizona.edu/classes/cs545/fall09/fibon1.prn.pdf
http://www.youtube.com/watch?v=Wbw8_YaAvBY
http://www.youtube.com/watch?v=0zaT7lWBEiU
http://wiki.openihs.org/Priority_queue_using_Fibonacci_heap
http://www.java2s.com/Code/Java/Collections-Data-Structure/Fibonacciheapdatastructure.htm
http://gauss.ececs.uc.edu/Courses/C228/LectureNotes/FibonacciHeaps/fibheap.pdf
http://www-bcf.usc.edu/~dkempe/teaching/fibonacci.pdf
http://www.cs.uiuc.edu/~jeffe/teaching/algorithms/notes/xl-fiboheap.pdf
http://www.cs.princeton.edu/~wayne/cs423/lectures/fibonacci-4up.pdf
http://www.cse.yorku.ca/~aaw/Jason/FibonacciHeapAlgorithm.html
http://www.keithschwarz.com/interesting/code/?dir=fibonacci-heap
http://stackoverflow.com/questions/9272854/how-to-insert-an-element-to-a-fibonacci-tree
http://stackoverflow.com/questions/15185208/implementing-consolidate-in-fibonacci-heap
http://ideone.com/9jYnv
http://programmers.stackexchange.com/questions/187410/marked-nodes-in-fibonacci-heaps
http://stackoverflow.com/questions/15815744/dijkstra-on-java-getting-interesting-results-using-a-fibonacci-heap-vs-priorit
http://win.ua.ac.be/~vanhoudt/graph/fibonacci.pdf
http://codereview.stackexchange.com/questions/10077/care-to-review-my-fibonacci-heap
http://classes.soe.ucsc.edu/cmps201/Fall07/slides/Fibsli.pdf
 */

/*
 * A cell (aka node or element) of a Fibonacci Heap.
 */
struct FCell {
    // pointer to child cell
    FCell* child = NULL;
    
    // pointer to left cell; used for doubly linked circular list
    FCell* left = NULL;
    
    // pointer to left cell; used for doubly linked circular list
    FCell* right = NULL;
    
    // value stored in this element cell
    string value;
    
    // the degree of the cell (which is the number of children)
    int degree;
    
    // A node is marked if at least one of its children was cut since
    //   this node was made a child of another node
    bool isMarked = false;
    
    // A unique number to mark the cell to enable clear printing
    //   of the heap. This is an important component for debugging.
    int cellIdNum;
};

/*
 * Fibonacci Heap implementation of the priority queue.
 */
class FibonacciHeapPriorityQueue {
  public:
	/* Constructs a new, empty priority queue. */
	FibonacciHeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~FibonacciHeapPriorityQueue();
	
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
    // number of cells stored in the entire data structure
    int numCells;
    
    // the most recently used unique cell identifier number
    int cellIdNum;
    
    // consolidate the trees
    void consolidate();
    
    // add a cell to list, anchoring it at a specific cell
    void addCellToList(FCell* toAdd, FCell*& anchorCell);
    
    // remove a cell from the list, with reference to its anchor cell
    void removeCellFromList(FCell* toRemove, FCell*& anchorCell);
    
    // pointer to the minimum cell in the data structure
    FCell* minCell;
    
    // pointer to the end of the list; used to simplify the insertion
    //   into the circular doubly linked list
    FCell* firstCell;
    
    // function crucial for debugging. prints out the heap visually
    //   so it can be seen. writing an fib heap without this is
    //   almost impossible
    void printHeap(FCell* anchorCell);
    void printHeap();
};

#endif
