/*************************************************************
 * File: pqueue-extra.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 *
 * An implementation of a Fibonacci Heap priority queue.
 */
 
#include "pqueue-fibonacciheap.h"
#include "error.h"

/*
 * Constructor to create a new priority queue based upon the Fibonacci
 *   Heap implementation.
 */
FibonacciHeapPriorityQueue::FibonacciHeapPriorityQueue() {
    // the heap is empty of cells when first created, so add
    minCell = NULL;
    firstCell = NULL;
    numCells = 0;
    cellIdNum = 0;
}

/*
 * Destructor to eliminate the priority queue and its allocated memory.
 */
FibonacciHeapPriorityQueue::~FibonacciHeapPriorityQueue() {
    // as long as a cell remains in the queue, keep removing the next cell
    //   until there are no more cells left
    while (!isEmpty()) {
        dequeueMin();
    }
}

/*
 * Return the total size of the priority queue. This includes all cells, not
 *   just the number of cells in the root.
 */
int FibonacciHeapPriorityQueue::size() {
    return numCells;
}

/*
 * Return whether the priority queue is empty of cells.
 */
bool FibonacciHeapPriorityQueue::isEmpty() {
    return (numCells == 0);
}

/*
 * Add a new value to the priority queue.
 */
void FibonacciHeapPriorityQueue::enqueue(string value) {
    // create a new cell to store the value and information about related cells
    FCell* newCell = new FCell;
    
    // a new cell, upon just being created, will have no children so
    //   its degree is 0.
    newCell->degree = 0;
    
    // set the value the client wants stored in the queue
    newCell->value = value;
    
    // set the unique identifier number for the newly added cell.
    //   This enables printing out the heap easily. Due to the complexity
    //   of this entire program, this debugging code is kept in the final
    //   compilation so it could be debugged later if an add-on were added.
    newCell->cellIdNum = ++cellIdNum;

    // add the new cell to the root linked list
    //   this is described in slides 7-9 of the Princeton Lectures
    //   this is what the Wikipedia article calls the Insert step
    addCellToList(newCell, firstCell);
    
    // having added a new cell to the heap, increment the counter of the
    //   total number of cells in the heap
    numCells++;
    
    // determine whether the newly added value, contained within the newCell,
    //   should marked as the minimum value.
    // this occurs in one of two cases:
    //   (1) the heap had no cell, so the newCell will be the smallest
    //   (2) the newCell value is smaller than the current min cell value, which
    //           means the new cell will now be the new smallest cell
    if(numCells == 1 || newCell->value < minCell->value)
        minCell = newCell;
}

/*
 * Return, but do not remove, the smallest element currently in the heap.
 */
string FibonacciHeapPriorityQueue::peek() {
    if (isEmpty()) error("Cannot peek at an empty list");
    return minCell->value;
}

/*
 * Return and remove the smallest element currently in the heap.
 */
string FibonacciHeapPriorityQueue::dequeueMin() {
    if (isEmpty()) error("Cannot dequeue at an empty list");
    
    // before remiving the minimum cell, capture the string value
    //   in the minimum cell so that it can be returned after the
    //   cell is deallocated
    string val = minCell->value;
    
    // remove the smallest cell from its list, which is connected to the list
    //   that forms the linked list of root cells
    removeCellFromList(minCell, firstCell);
    
    // take all the children of the minimum cell, which lies in the root list
    //   and as long as there is still a child to the min cell, remove the
    //   child and promote it to the root list
    // Wikipedia calls this the extract Minimum Operation while the
    //   Princeton lectures call this the Delete Min operation

    FCell* temp; // create a temp variable to store the child once it is
                 //   cut from its parent
    while (minCell->child != NULL) {
        // temporarily store the child element
        temp = minCell->child;

        // remove the child of the minimum cell
        removeCellFromList(temp, minCell->child);
        
        // add the child of the minimum cell to the root list by
        //   adding it as a sibling of the first cell in the root list
        addCellToList(temp, firstCell);
    };
    
    // deallocate the memory dynamically allocated in enqueue() when
    //   a new cell is created.
    delete minCell;
    
    // since a cell is being removed from the heap, reduce the total count
    //   of cells
    numCells--;
    
    // if the heap is not empty, consolidate the list tree
    if (numCells > 0) {
        consolidate();
    }

    // return the minimum value
    return val;
}


/*
 * Remove a marked cell from a specific anchor cell.
 * Note: I first wrote this seemingly simple function almost exactly as is,
 *   though without the isMarked value set to true and without the
 *   second parameter pointer passed by reference. However, as I struggled
 *   through the consolidate function and adopted some of the
 *   specifics of a solution online (as noted below), I had to likewise
 *   adopt this solution as pass by reference for the second pointer.
 */
void FibonacciHeapPriorityQueue::removeCellFromList(FCell* toRemove,
                                                    FCell*& anchorCell) {
   if (toRemove->isMarked) {
        if (toRemove == toRemove->right) {
            // the cell being removed has no siblings and is linked to itself
            anchorCell = NULL;
        } else {
            // the cell being removed is referenced to itself
            if(anchorCell == toRemove) {
                anchorCell = anchorCell->right;
            }
            
            // close the gap in the list left when a specific cell is
            //   removed
            toRemove->right->left = toRemove->left;
            toRemove->left->right = toRemove->right;
        }
    }
    
    // the cell has been removed from the list, so it is no longer marked
    toRemove->isMarked = false;
}

/*
 * Add a new cell as a neighbor to a specific anchor cell.
 * Note: I first wrote this seemingly simple function almost exactly as is,
 *   though without the isMarked value set to true and without the
 *   second parameter pointer passed by reference. However, as I struggled
 *   through the consolidate function and adopted some of the
 *   specifics of a solution online (as noted below), I had to likewise
 *   adopt this solution as pass by reference for the second pointer.
 */
void FibonacciHeapPriorityQueue::addCellToList(FCell* toAdd,
                                               FCell*& anchorCell) {
    if (anchorCell == NULL) {
        // anchor cell is empty, so link a cell to itself
        toAdd->left = toAdd;
        toAdd->right = toAdd;
        anchorCell = toAdd;
    } else {
        // anchor cell is not empty, so insert it into the list next to
        //   the anchor cell; adjust the pointers to the neighboring cells
        toAdd->left = anchorCell;
        toAdd->right = anchorCell->right;
        anchorCell->right = toAdd;
        toAdd->right->left = toAdd;
    }

    // denote that the new cell being added is marked
    // Note: when I read the descriptions of what marking a cell is for
    //   on Wikipedia as well as in the Princeton lectures, it
    //   seems to me that a cell upon first being added should not be
    //   marked as it has not yet been cut. However, I simply could not
    //   get the code to work in this fashion and eventually defaulted
    //   to the following implementation based upon more research online.
    //   According to the StackExchange article referenced above, this simply
    //   decreases the efficiency of the program.
    toAdd->isMarked = true;
}

/*
 * Consolidate the heap.
 * This step ensures that heap tree never has two roots of the same degree.
 *   In other words, ensure that each cell never has the same number of
 *   children.

http://www.cse.yorku.ca/~aaw/Jason/FibonacciHeapAlgorithm.html
CONSOLIDATE(H)
 for i:=0 to D(n[H])
   Do A[i] := NIL

 for each node w in the root list of H
   do x:= w
      d:= degree[x]
      while A[d] <> NIL
         do y:=A[d]
            if key[x]>key[y]
               then exchange x<->y
            Fibonacci-Heap-Link(H, y, x)
            A[d]:=NIL
            d:=d+1
      A[d]:=x

 min[H]:=NIL
 
 for i:=0 to D(n[H])
     do if A[i]<> NIL
        then add A[i] to the root list of H
             if min[H] = NIL or key[A[i]]<key[min[H]]
                then min[H]:= A[i]

I relied especially heavily on the following code as an example implementation.
http://wiki.openihs.org/Priority_queue_using_Fibonacci_heap
 
 */
void FibonacciHeapPriorityQueue::consolidate() {
    /*
     * Create an array that can store the cells in the heap list.
     *   This is used as a key part of what most of the aforementioned
     *   lecture materials call the DeleteMin stage. In particular,
     *   it keeps track of the root cell for any given cell degree (i.e., 
     *   cell rank -- i.e., number of children).
     *
     *  This is necessary because in the consolidate stage, we cannot have
     *   any two groups of trees with the same degree.
     */
    FCell* rootCells[numCells];

    /*
     * See 4:30 into http://www.youtube.com/watch?v=Wbw8_YaAvBY where a
     *   crucial part of the delete min stage (which includes consolidation)
     *   is creating an array of NULL for all the possible degree root cells.
     *
     * For each possible root cell, we are setting the values to NULL
     *   initially and they can be overridden.
     */
    for (int i = 0; i < numCells; i++) {
        rootCells[i] = NULL;
    }
    
    FCell* rootListCell;
    
    // create a temporary variable to store the degree (i.e., num children)
    int cellDegree;
    
    // for each cell in the root node list (which we start by getting with
    //   the firstCell pointer), get the degree of the root cell,
    //   remove that from the list, capture the number of degrees,
    //   and then iterate through those children swapping to ensure the
    //   heap order is preserved where children are larger than their
    //   parents
    while (firstCell != NULL) {
        rootListCell = firstCell;
        removeCellFromList(rootListCell, firstCell);
        
        // get the degree of the current root list cell
        cellDegree = rootListCell->degree;
        
        // as long as there is a cell of a specific degree, loop
        //  through the root cells 
        while (rootCells[cellDegree] != NULL) {
            // get the cell with the degree of the current root list
            FCell* y;
            y = rootCells[cellDegree];
            if (rootListCell->value > y->value) {
                // swap the parent and child as the heap order has been
                //   violated
                FCell* temp;
                temp = rootListCell;
                rootListCell = y;
                y = temp;
            }

            // remove the cell which does not belong in the root list
            //   from the root list; we are ensured it does not belong
            //   in the root list because we have swaped with the rootCell
            //   if the heap order was violated
            removeCellFromList(y, firstCell);
            
            // now that y has been removed from the root list, add it to the
            //   child of the root list
            addCellToList(y, rootListCell->child);
            
            // as the cell has been added to the child, the child's
            //   degree needs to be incremented since it now has a new child
            (rootListCell->degree)++;
            
            // as the tree has been adjusted, we clear the root cell
            //   for a particular degree
            rootCells[cellDegree] = NULL;
            
            // as we have now checked everything in a specific degree,
            //   we are ready to increment cell degree and check for the next
            //   cell degree
            cellDegree++;
        }
        
        // store the new root cell for a given degree in the array
        rootCells[cellDegree] = rootListCell;
    }
    
    // since we have significantly adjusted the tree structure of the heap
    //   and removed the minimum cell in dequeueMin, we now do not know
    //   what is the minimum cell, so set the pointer that points
    //   to the minimum value cell blank; we will find this value next
    minCell = NULL;

    /*
     * Now that computing the root cell linked list is finished...
     *   (1) for every cell identified as a root cell, add it to the
     *         root linked list, which is done by adding it to the pointer
     *         for the first cell (i.e., make it a sibling of the first cell).
     *   (2) compute the proper pointer to the minimum cell by checking
     *         whether each new cell added to the root is less than the
     *         current minimum.
     */
    for (int i = 0; i < numCells; i++) {
        // check if a valid head cell exists for a given degree
        if (rootCells[i] != NULL) {
            // add the head of a tree to the root cell list by chaining it
            //   to the first cell pointer
            addCellToList(rootCells[i], firstCell);
            
            // check whether the cell newly added to the root is the
            //   smallest value
            if(minCell == NULL || rootCells[i]->value < minCell->value){
                minCell = rootCells[i];
            }
        }
    }
}

/*
 * Visually represent the heap and the connections of the cells in the heap.
 * Function used for debugging.
 */
void FibonacciHeapPriorityQueue::printHeap(FCell* anchorCell) {
    // traverse all siblings
    FCell* siblingCellToPrint = anchorCell;
    while (siblingCellToPrint != NULL) {
        
        // print information about the current cell
        cout << siblingCellToPrint->value
             << " ID:" << siblingCellToPrint->cellIdNum
             << " C:";
        if (siblingCellToPrint->child == NULL) {
            cout << "N";
        } else {
            cout << siblingCellToPrint->child->cellIdNum;
        }
        cout << " SL:" << siblingCellToPrint->left->cellIdNum
             << " SR:" << siblingCellToPrint->right->cellIdNum;
        cout << endl;

        // traverse all children of the current cell
        FCell* childToPrint = siblingCellToPrint->child;
        if (childToPrint != NULL) {
            // recursively print children
            printHeap(childToPrint);
        }
        
        // advance to the next sibling and check for the base case
        siblingCellToPrint = siblingCellToPrint->right;
        if (siblingCellToPrint == anchorCell)
            return;
    }
}

/*
 * Visually represent the heap and the connections of the cells in the heap.
 * Function used for debugging.
 */
void FibonacciHeapPriorityQueue::printHeap() {
    if (firstCell == NULL) {
        cout << "tree is empty" << endl;
        return;
    }
    printHeap(firstCell);
}
