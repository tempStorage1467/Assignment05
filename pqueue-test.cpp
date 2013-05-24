/********************************************************************
 * File: pqueue-test.cpp
 *
 * Name: Eric Beach
 * Section: Dawson Zhou
 *
 * Testing code for the various priority queue classes.  These tests
 * are written as template functions so that they can operate over
 * all different types of priority queues.
 *
 * I wrote two extensions: (1) Time testing. I wrote a test suite, 11,
 *   that will perform a number of operations on all the PQueue types
 *   and clock them, measuring the performance.
 *   (2) I wrote an edge-case test that will try to simulate some of the
 *   toughest scenarios to code that I encourted when I was writing the
 *   initial solutions.
 */
#include "pqueue-vector.h"
#include "pqueue-linkedlist.h"
#include "pqueue-doublylinkedlist.h"
#include "pqueue-heap.h"
#include "pqueue-fibonacciheap.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
#include "console.h"
#include "error.h"
#include <iostream>
#include <string>
#include <sstream>    // For istringstream
#include <algorithm>  // For sort, reverse
#include <functional> // For greater
using namespace std;

/* Macro: checkCondition
 * ------------------------------------------------------------
 * A utility macro that makes it easier to diagnose and report
 * test failures.
 */
#define checkCondition(expr, reason) docheckCondition((expr), (reason), #expr, __LINE__)

/* Function: docheckCondition
 * ------------------------------------------------------------
 * Checks whether the given condition is true.  If not, prints
 * a diagnostic error message.
 */
void docheckCondition(bool expr, string reason, string expression, int lineNumber) {
	if (expr) {
		cout << "  PASS: " << reason << endl;
	} else {
		cout << "! FAIL: " << reason << endl;
		cout << "     Test failed on line " << lineNumber << " of pqueue-test.cpp" << endl;
		cout << "     Specific expression: " << expression << endl;
		getLine("     Press ENTER to continue...");
	}
}

/* Function: beginTest
 * ------------------------------------------------------------
 * Displays a message indicating that a group of tests has
 * started.
 */
void beginTest(string testName) {
	cout << "================== BEGIN: " << testName << "==================" << endl;
}

/* Function: endTest
 * ------------------------------------------------------------
 * Displays a message indicating that a group of tests has
 * started.
 */
void endTest(string testName) {
	cout << "=================== END: " << testName << "===================" << endl;
	getLine("Press ENTER to continue...");
}

/* Function: logInfo
 * ------------------------------------------------------------
 * Displays an informational message.
 */
void logInfo(string message) {
	cout << "  info: " << message << endl;
}

/* Function: randomString
 * ------------------------------------------------------------
 * Returns a randomly-generated string of the given length.
 */
string randomString(int length = 16) {
	string result;
	for (int i = 0; i < length; i++) {
		result += char(randomInteger('A', 'Z'));
	}
	return result;
}

/*
 * Function: runSpeedTest
 * ------------------------------------------------------------
 * Return the number of seconds to perform nIter iterations of inserting
 *   then removing wordsPerIteration words from a queue.
 */
template <typename PQueue>
   double runSpeedTests(const int nIter, const int wordsPerIteration) {
   
       /* Clock the execution of the program Priority Queue */
       double beginTime = clock();
       
       for (int iteration = 0; iteration < nIter; iteration++) {
           Vector<string> randomValues;
           for (int i = 0; i < wordsPerIteration; i++) {
               randomValues += randomString();
           }
    
           /* Use C++'s provided sorting routine to sort these values. */
           sort(randomValues.begin(), randomValues.end(), greater<string>());
           
           /* Confirm each comes back correctly. */
           reverse(randomValues.begin(), randomValues.end());
           
           
           /* Feed these values into priority queue and pull them back out. */
           PQueue queue;
           foreach (string value in randomValues)
             queue.enqueue(value);
       
           for (int i = 0; i < randomValues.size(); i++)
               queue.dequeueMin();

       }
       
       /* Stop the clock */
       double endTime = clock();
       
       /* Compute the time difference and return it */
       return (endTime - beginTime) / CLOCKS_PER_SEC;
}

/*
 * Function: runSpeedTests
 * ------------------------------------------------------------
 * Add and then remove many words from each type of priority queue.
 *   Perform this operation multiple times and clock the total time
 *   taken. Print out this time for each priority queue.
 */
void runSpeedTests() {
    const int NUM_ITERATIONS = 1;
    
    // using a large number of words per iteration also serves as a
    //   check against memory leaks in the program
    const int WORDS_PER_ITERATION = 30000;

    double vectorSpeed =
    runSpeedTests<VectorPriorityQueue> (NUM_ITERATIONS, WORDS_PER_ITERATION);
    cout << "Vector: " << vectorSpeed << " seconds" << endl;
    
    double linkedListSpeed =
      runSpeedTests<LinkedListPriorityQueue> (NUM_ITERATIONS, WORDS_PER_ITERATION);
    cout << "Linked List: " << linkedListSpeed << " seconds" << endl;
    
    double dLinkedListSpeed =
      runSpeedTests<DoublyLinkedListPriorityQueue> (NUM_ITERATIONS, WORDS_PER_ITERATION);
    cout << "Double Linked List: " << dLinkedListSpeed << " seconds" << endl;

    double heapPriorityQueueSpeed =
      runSpeedTests<HeapPriorityQueue> (NUM_ITERATIONS, WORDS_PER_ITERATION);
    cout << "Heap Priority Queue: " << heapPriorityQueueSpeed << " seconds" << endl;
    
    double fibHeapSpeed =
      runSpeedTests<FibonacciHeapPriorityQueue> (NUM_ITERATIONS, WORDS_PER_ITERATION);
    cout << "Fib Heap: " << fibHeapSpeed << " seconds" << endl;
}


/* Function: basicStructuralTests
 * ------------------------------------------------------------
 * A function that does basic structural checks on a priority
 * queue.  It verifies that size/isEmpty agree, that the size
 * matches the number of elements, etc.
 *
 * If these tests are failing, it likely indicates that there
 * is a bug in your code that is causing your implementation
 * to remember size information correctly.
 */
template <typename PQueue>
	void basicStructuralTests() {
	beginTest("Basic Structural Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		 
		/* Basic test: Add 5 elements to the queue and ensure that
		 * the size is correct at each step.
		 */
		{
			logInfo("These tests will check size() isEmpty() without calling dequeueMin().");
		
			PQueue queue;
			checkCondition(queue.isEmpty(), "New priority queue should be empty.");
			checkCondition(queue.size() == 0, "New priority queue should have size 0.");
			
			for (int i = 0; i < 5; i++) {
				queue.enqueue("Test String");
				
				checkCondition(queue.size() == i + 1, "Queue should have proper size after inserting a value.");
				checkCondition(!queue.isEmpty(), "Queue containing elements should not be empty.");
			}
		}
		
		/* Slightly more complex test: Enqueue and dequeue elements, ensuring the
		 * size matches at each step.
		 */
		{
			logInfo("We're about to start calling dequeueMin().");
			PQueue queue;
			
			for (int i = 0; i < 5; i++) {
				queue.enqueue("Test String");
			}
			
			for (int i = 5; i > 0; i--) {
				checkCondition(queue.size() == i, "Queue should have proper size after dequeues.");
				checkCondition(!queue.isEmpty(), "Queue should not be empty before all elements are removed.");
				queue.dequeueMin();
			}
			
			checkCondition(queue.size() == 0, "After removing all elements, the queue should have size 0.");
			checkCondition(queue.isEmpty(), "After removing all elements, the queue should be empty.");
		}
		
		/* Getting harder: The value dequeued should always match the value of peek(). */
		{
			logInfo("This next test will check whether peek() matches dequeueMin().");
			PQueue queue;
			
			for (int i = 0; i < 5; i++) {
				queue.enqueue(randomString());
			}
			
			while (!queue.isEmpty()) {
				string expected = queue.peek();
				checkCondition(queue.dequeueMin() == expected, "Value returned by peek() matches value returned by dequeueMin()");
			}
		}
		
		/* A different one - let's make sure that peeking at an empty queue causes an
		 * error.
		 */
		{
			PQueue queue;
			bool didThrow = false;
			try {
				logInfo("About to peek into an empty queue.  This may cause a crash");
				logInfo("if your implementation is incorrect.");
				queue.peek();			
			} catch (ErrorException&) {
				didThrow = true;
			}
			
			checkCondition(didThrow, "Priority queue uses 'error' when peek() called on empty queue.");
		}
		
		/* In the same vein - what happens if we dequeue from an empty queue? */
		{
			PQueue queue;
			bool didThrow = false;
			try {
				logInfo("About to dequeue from an empty queue.  This may cause a crash");
				logInfo("if your implementation is incorrect.");
				queue.dequeueMin();			
			} catch (ErrorException&) {
				didThrow = true;
			}
			
			checkCondition(didThrow, "Priority queue uses 'error' when dequeueMin() called on empty queue.");
		}
	
	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Basic Structural Tests");
}


/* Function: sortAscendingTests
 * ------------------------------------------------------------
 * A function that uses the priority queue to sort data that
 * has already been sorted.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void sortAscendingTests() {
	beginTest("Sort Ascending Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		 
		/* Basic test: Feed the strings A through H into the priority queue and confirm
		 * that they come back in the same order.
		 */
		{
			logInfo("Enqueuing A - H into the queue and confirming it comes back sorted.");
			PQueue queue;
			for (char ch = 'A'; ch <= 'H'; ch++) {
				/* The code
				 *
				 *    string(1, ch)
				 *
				 * converts the character ch into a one-character string.
				 */
				queue.enqueue(string(1, ch));
			}
			
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		 
		/* Harder test: Sort 10 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Enqueuing ten sorted random strings and confirming it comes back sorted.");
			Vector<string> randomValues;
			for (int i = 0; i < 10; i++) {
				randomValues += randomString();
			}
			
			/* Use C++'s provided sorting routine to sort these values. */
			sort(randomValues.begin(), randomValues.end());
			
			/* Feed these values into the priority queue and pull them back out. */			
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Confirm each comes back correctly. */
			for (int i = 0; i < randomValues.size(); i++) {
				checkCondition(queue.dequeueMin() == randomValues[i],
				               "Expecting to get value " + randomValues[i] + " from queue.");
			}
		}
		
		/* Much harder test: Sort 10000 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Generating 10000 random strings.");
			Vector<string> randomValues;
			for (int i = 0; i < 10000; i++) {
				randomValues += randomString();
			}
			
			/* Use C++'s provided sorting routine to sort these values. */
			logInfo("Sorting 10000 random strings.");
			sort(randomValues.begin(), randomValues.end());
			
			/* Feed these values into the priority queue and pull them back out. */			
			logInfo("Enqueuing 10000 random strings.");
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Confirm each comes back correctly. */
			logInfo("Dequeuing 10000 random strings.");
			bool isCorrect = true;
			for (int i = 0; i < randomValues.size(); i++) {
				if (queue.dequeueMin() != randomValues[i]) {
					isCorrect = false;
					break;
				}
			}
			
			checkCondition(isCorrect, "Queue correctly sorted 10000 random strings.");
		}

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Sort Ascending Tests");
}

/* Function: sortDescendingTests
 * ------------------------------------------------------------
 * A function that uses the priority queue to sort data that
 * has already been sorted in reverse.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void sortDescendingTests() {
	beginTest("Sort Descending Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		 
		/* Basic test: Feed the strings H through A into the priority queue and confirm
		 * that they come back in the same order.
		 */
		{
			logInfo("Enqueuing A - H in reverse order and checking that it comes back sorted.");
			PQueue queue;
			for (char ch = 'H'; ch >= 'A'; ch--) {
				/* The code
				 *
				 *    string(1, ch)
				 *
				 * converts the character ch into a one-character string.
				 */
				queue.enqueue(string(1, ch));
			}
			
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		 
		/* Harder test: Sort 10 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Enqueuing ten reverse sorted random strings and confirming it comes back sorted.");
			Vector<string> randomValues;
			for (int i = 0; i < 10; i++) {
				randomValues += randomString();
			}
			
			/* Use C++'s provided sorting routine to sort these values. */
			sort(randomValues.begin(), randomValues.end(), greater<string>());
			
			/* Feed these values into the priority queue and pull them back out. */			
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Confirm each comes back correctly. */
			reverse(randomValues.begin(), randomValues.end());
			for (int i = 0; i < randomValues.size(); i++) {
				checkCondition(queue.dequeueMin() == randomValues[i],
				               "Expecting to get value " + randomValues[i] + " from queue.");
			}
		}
		
		/* Much harder test: Sort 10000 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Generating 10000 random strings.");
			Vector<string> randomValues;
			for (int i = 0; i < 10000; i++) {
				randomValues += randomString();
			}
			
			/* Use C++'s provided sorting routine to sort these values. */
			logInfo("Sorting 10000 random strings.");
			sort(randomValues.begin(), randomValues.end(), greater<string>());
			
			/* Feed these values into the priority queue and pull them back out. */			
			logInfo("Enqueuing 10000 random strings.");
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Confirm each comes back correctly. */
			logInfo("Dequeuing 10000 random strings.");
			bool isCorrect = true;
			reverse(randomValues.begin(), randomValues.end());
			for (int i = 0; i < randomValues.size(); i++) {
				if (queue.dequeueMin() != randomValues[i]) {
					isCorrect = false;
					break;
				}
			}
			
			checkCondition(isCorrect, "Queue correctly sorted 10000 random strings.");
		}

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Sort Descending Tests");
}

/* Function: sortRandomTests
 * ------------------------------------------------------------
 * A function that uses the priority queue to sort data that
 * is stored in no particular order.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void sortRandomTests() {
	beginTest("Sort Random Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		 
		/* Basic test: Feed the strings H through A into the priority queue in some order,
		 * then confirm that they come back in sorted order
		 */
		{
			logInfo("Enqueuing a random permutation of A - H and checking whether it leaves sorted.");
			Vector<string> letters;
			for (char ch = 'A'; ch <= 'H'; ch++) {
				/* The code
				 *
				 *    string(1, ch)
				 *
				 * converts the character ch into a one-character string.
				 */
				letters += string(1, ch);
			}
			
			/* Scramble the letters with the STL random_shuffle algorithm. */
			random_shuffle(letters.begin(), letters.end());
		
			/* Load the letters into the priority queue. */
			PQueue queue;
			foreach (string letter in letters)
				queue.enqueue(letter);
			
			/* Confirm they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		 
		/* Harder test: Sort 10 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Enqueuing 10 random strings and checking whether it leaves sorted.");
			/* Create 10 random strings. */
			Vector<string> randomValues;
			for (int i = 0; i < 10; i++) {
				randomValues += randomString();
			}
			
			/* Feed these values into the priority queue and pull them back out. */			
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Confirm each comes back correctly. */
			sort(randomValues.begin(), randomValues.end());
			for (int i = 0; i < randomValues.size(); i++) {
				checkCondition(queue.dequeueMin() == randomValues[i],
				               "Expecting to get value " + randomValues[i] + " from queue.");
			}
		}
		
		/* Much harder test: Sort 10000 random strings and confirm that the priority queue hands
		 * them back in the same order.
		 */
		{
			logInfo("Generating 10000 random strings.");
			Vector<string> randomValues;
			for (int i = 0; i < 10000; i++) {
				randomValues += randomString();
			}
			
			/* Feed these values into the priority queue and pull them back out. */			
			logInfo("Enqueuing 10000 random strings.");
			PQueue queue;
			foreach (string value in randomValues)
				queue.enqueue(value);
				
			/* Use C++'s provided sorting routine to sort these values. */
			logInfo("Sorting 10000 random strings.");
			sort(randomValues.begin(), randomValues.end(), greater<string>());
				
			/* Confirm each comes back correctly. */
			logInfo("Dequeuing 10000 random strings.");
			bool isCorrect = true;
			reverse(randomValues.begin(), randomValues.end());
			for (int i = 0; i < randomValues.size(); i++) {
				if (queue.dequeueMin() != randomValues[i]) {
					isCorrect = false;
					break;
				}
			}
			
			checkCondition(isCorrect, "Queue correctly sorted 10000 random strings.");
		}

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Sort Random Tests");
}


/* Function: sortCraftedTests
 * ------------------------------------------------------------
 * A function that constructs specific inputs known to give
 * buggy priority queues trouble, then tests the given priority
 * queue on them.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void sortCraftedTests() {
	beginTest("Sort Crafted Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		
		/* First test: Two concatenated sequences of ascending values that
		 * interleave with one another.
		 */
		{
			logInfo("Sorting two sequences that need to be interleaved.");
			Vector<string> sequence;
			sequence += string(1, 'A');
			sequence += string(1, 'C');
			sequence += string(1, 'E');
			sequence += string(1, 'G');
			sequence += string(1, 'B');
			sequence += string(1, 'D');
			sequence += string(1, 'F');
			sequence += string(1, 'H');
			
			/* Feed this sequence into the priority queue. */
			PQueue queue;
			foreach (string letter in sequence) {
				queue.enqueue(letter);
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		
		/* Second test: Two interleaving sequences, alternate version.. */
		{
			logInfo("Sorting two sequences that need to be interleaved, version two.");
			Vector<string> sequence;
			sequence += string(1, 'B');
			sequence += string(1, 'D');
			sequence += string(1, 'F');
			sequence += string(1, 'H');
			sequence += string(1, 'A');
			sequence += string(1, 'C');
			sequence += string(1, 'E');
			sequence += string(1, 'G');
			
			/* Feed this sequence into the priority queue. */
			PQueue queue;
			foreach (string letter in sequence) {
				queue.enqueue(letter);
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		
		/* Third test: Two decreasing interleaved sequences. */
		{
			logInfo("Sorting two decreasing sequences that need to be interleaved.");
			Vector<string> sequence;
			sequence += string(1, 'H');
			sequence += string(1, 'F');
			sequence += string(1, 'D');
			sequence += string(1, 'B');
			sequence += string(1, 'G');
			sequence += string(1, 'E');
			sequence += string(1, 'C');
			sequence += string(1, 'A');
			
			/* Feed this sequence into the priority queue. */
			PQueue queue;
			foreach (string letter in sequence) {
				queue.enqueue(letter);
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}
		
		/* Fourth test: Two decreasing interleaved sequences, alternate version. */
		{
			logInfo("Sorting two decreasing sequences that need to be interleaved, version 2.");
			Vector<string> sequence;
			sequence += string(1, 'G');
			sequence += string(1, 'E');
			sequence += string(1, 'C');
			sequence += string(1, 'A');
			sequence += string(1, 'H');
			sequence += string(1, 'F');
			sequence += string(1, 'D');
			sequence += string(1, 'B');
			
			/* Feed this sequence into the priority queue. */
			PQueue queue;
			foreach (string letter in sequence) {
				queue.enqueue(letter);
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				string expected(1, ch);
				checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
			}
		}

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Sort Crafted Tests");
}

/* Function: sortDuplicateTests
 * ------------------------------------------------------------
 * A function that tests the priority queue's ability to handle
 * duplicate elements.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void sortDuplicateTests() {
	beginTest("Sort Duplicate Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		/* The use of curly braces here introduces a new block scope.  We
		 * use this so that we can construct multiple different priority
		 * queues.
		 */
		
		/* First test: Duplicate the letters A - D, in order, and see if they
		 * come back correctly.
		 */
		{
			logInfo("Loading a sorted sequence containing duplicates.");	
			/* Populate a priority queue accordingly. */
			PQueue queue;
			for (char ch = 'A'; ch <= 'D'; ch++) {
				queue.enqueue(string(1, ch));
				queue.enqueue(string(1, ch));
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'D'; ch++) {
				for (int i = 0; i < 2; i++) {
					string expected(1, ch);
					checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
				}
			}
		}
		
		/* Second test: Duplicate the letters A - D, in reverse order, and see if they
		 * come back correctly.
		 */
		{
			logInfo("Loading a reverse sorted sequence containing duplicates.");
			
			/* Populate a priority queue accordingly. */
			PQueue queue;
			for (char ch = 'D'; ch >= 'A'; ch--) {
				queue.enqueue(string(1, ch));
				queue.enqueue(string(1, ch));
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'D'; ch++) {
				for (int i = 0; i < 2; i++) {
					string expected(1, ch);
					checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
				}
			}
		}
		
		/* Third test: Enqueue the letters A - H, then duplicate the sequence. */
		{
			logInfo("Loading two sorted sequences, one after the other.");
			/* Populate a priority queue accordingly. */
			PQueue queue;
			for (int i = 0; i < 2; i++) {
				for (char ch = 'A'; ch <= 'H'; ch++) {
					queue.enqueue(string(1, ch));
				}
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				for (int i = 0; i < 2; i++) {
					string expected(1, ch);
					checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
				}
			}
		}
		
		/* Fourth test: Enqueue the letters A - H, in reverse order, then duplicate the sequence. */
		{			
			logInfo("Loading two reverse sorted sequences, one after the other.");
					
			/* Populate a priority queue accordingly. */
			PQueue queue;
			for (int i = 0; i < 2; i++) {
				for (char ch = 'H'; ch >= 'A'; ch--) {
					queue.enqueue(string(1, ch));
				}
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				for (int i = 0; i < 2; i++) {
					string expected(1, ch);
					checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
				}
			}
		}
		
		/* Fifth test: Load a sorted sequence, then duplicate the middle 50%. */
		{			
			logInfo("Loading a sorted sequence, then repeating the middle.");
					
			/* Load the first range, then the second. */
			PQueue queue;
			for (char ch = 'A'; ch <= 'H'; ch++) {
				queue.enqueue(string(1, ch));
			}
			for (char ch = 'C'; ch <= 'F'; ch++) {
				queue.enqueue(string(1, ch));
			}
			
			/* Dequeue the elements and confirm that they come back sorted. */
			for (char ch = 'A'; ch <= 'H'; ch++) {
				int numTimes = (ch >= 'C' && ch <= 'F'? 2 : 1);
				for (int i = 0; i < numTimes; i++) {
					string expected(1, ch);
					checkCondition(queue.dequeueMin() == expected, "Queue should yield " + expected + ".");
				}
			}
		}
	

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Sort Duplicate Tests");
}

/* Function: reuseTests
 * ------------------------------------------------------------
 * A function that tests the priority queue's ability to fill
 * up, then empty, then fill up again.
 *
 * If your code is failing these tests, you probably have a bug
 * in either your enqueue() or dequeueMin() functions.
 */
template <typename PQueue>
	void reuseTests() {
	beginTest("Reuse Tests");
	
	/* This try ... catch system is designed to catch any errors that the
	 * program explicitly generates.  It does not guard against runtime
	 * crashes from errors like following a bad pointer, so if your
	 * program crashes and pulls up the debugger, you should be looking
	 * for memory errors.
	 */
	try {
		PQueue queue;
		
		bool isSorted = true;
		for (int i = 0; i < 5; i++) {
			/* Generate 10000 values to enter and remove. */
			logInfo("Generating 10000 random strings.");
			Vector<string> strings;
			for (int i = 0; i < 10000; i++) {
				strings += randomString();
			}
			
			/* Enqueue them into the priority queue. */
			logInfo("Enqueuing 10000 random strings.");
			foreach (string str in strings) {
				queue.enqueue(str);
			}
			
			/* Sort the strings so we can check that we have them in the right
			 * order.
			 */
			logInfo("Sorting 10000 random strings.");
			sort(strings.begin(), strings.end());
			
			/* Confirm they come back correctly. */
			logInfo("Dequeuing 10000 random strings.");
			foreach(string str in strings) {
				if (queue.dequeueMin() != str) {
					isSorted = false;
				}
			}
			
			if (!isSorted) break;
		}
		
		checkCondition(isSorted, "Strings were consistently sorted.");
	

	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("Reuse Tests");
}

/* Function: myVeryOwnTests
 * ------------------------------------------------------------------
 * If you'd like to add any testing code of your own, feel free to
 * add them!  This test currently has no functionality associated
 * with it, but you can improve upon that if you'd like.
 */
template <typename PQueue>
	void myVeryOwnTests() {
	beginTest("My Very Own Tests");
	
    /*
     In writing these programs and manually testing them, I found that
       the most likely to fail situations occurred with one, two, or a few
       elements.
     */
        
    try {
        // Test Corner Cases With One Element
        PQueue queue1;
        string s1 = "a";
        queue1.enqueue(s1);

        checkCondition(queue1.peek() == s1,
                       "Peeked at single element vector successfully");
        
        checkCondition(queue1.dequeueMin() == s1,
                       "Pulled single element vector successfully");
        
        // Test Corner Cases With Two Elements
        PQueue queue2;
        checkCondition(queue2.size() == 0,
                       "Size is proper");

        
        checkCondition(queue2.isEmpty() == true,
                       "Size is proper");
        
        string s2a = "z";
        string s2b = "a";
        queue2.enqueue(s2a);
        queue2.enqueue(s2b);

        checkCondition(queue2.peek() == s2b,
                       "Peeked at single element vector successfully");
        checkCondition(queue2.size() == 2, "Size is proper");
        
        checkCondition(queue2.dequeueMin() == s2b,
                       "Pulled off element successfully");

        checkCondition(queue2.size() == 1,
                       "Size is proper");
        
        checkCondition(queue2.dequeueMin() == s2a,
                       "Pulled off element successfully");
        
        // Test Corner Cases With Four Elements
        PQueue queue3;
        string s3a = "z";
        string s3b = "b";
        string s3c = "a";
        string s3d = "c";
        queue3.enqueue(s3a);
        queue3.enqueue(s3b);
        queue3.enqueue(s3c);
        queue3.enqueue(s3d);
        
        checkCondition(queue3.peek() == s3c,
                       "Peeked at single element vector successfully");
        checkCondition(queue3.size() == 4, "Size is proper");
        
        checkCondition(queue3.dequeueMin() == s3c,
                       "Pulled off element successfully");
        
        checkCondition(queue3.size() == 3,
                       "Size is proper");
        
        checkCondition(queue3.dequeueMin() == s3b,
                       "Pulled off element successfully");
        
        checkCondition(queue3.isEmpty() == false,
                       "Not empty");
        
        checkCondition(queue3.dequeueMin() == s3d,
                       "Pulled off element successfully");
        
        checkCondition(queue3.dequeueMin() == s3a,
                       "Pulled off element successfully");
        
        checkCondition(queue3.isEmpty() == true,
                       "Empty");
	} catch (ErrorException& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.getMessage() << endl;
	} catch (exception& e) {
		cout << "TEST FAILURE: Unexpected exception: " << e.what() << endl;
	} catch (...) {
		cout << "TEST FAILURE: Unknown exception." << endl;
	}
	
	endTest("My Very Own Tests");	
}

/* Function: testPriorityQueue
 * ------------------------------------------------------------------
 * A function that runs the full battery of tests on a priority
 * queue of arbitrary type.
 */
template <typename PQueue>
	void testPriorityQueue() {
	
	basicStructuralTests<PQueue> ();
	sortAscendingTests<PQueue> ();
	sortDescendingTests<PQueue> ();
	sortRandomTests<PQueue> ();
	sortCraftedTests<PQueue> ();
	sortDuplicateTests<PQueue> ();
	reuseTests<PQueue> ();
	
	myVeryOwnTests<PQueue> ();
}

/* Function: printReplInstructions
 * ------------------------------------------------------------------
 * Prints instructions on how to use the REPL environment.
 */
void printReplInstructions() {
	cout << "Interactive Priority Queue Test" << endl;
	cout << "===============================" << endl;
	cout << "This environment allows you to type in commands that will be" << endl;
	cout << "executed on your priority queue.  The interpreter knows the" << endl;
	cout << "following commands:" << endl;
	cout << endl;
	cout << "   isEmpty:        Reports whether the priority queue is empty." << endl;
	cout << "   size:           Reports the size of the priority queue" << endl;
	cout << "   enqueue <str>:  Enqueues the string <str>" << endl;
	cout << "   peek:           Peeks at the minimum element of the priority queue." << endl;
	cout << "   dequeueMin:     Dequeues the minimum element of the priority queue." << endl;
	cout << "   quit:           Quits the interpret and returns to the menu." << endl;
	cout << endl;
	cout << "The first letter of any command can be used as a substitute" << endl;
	cout << "for any command name." << endl;
}

/* Function: replTestPriorityQueue
 * ------------------------------------------------------------------
 * A function that executes a read/eval/print loop (REPL) that allows
 * you to manually test out the priority queue.  Entering commands
 * like
 *
 *    enqueue This is a test
 *
 * or
 *
 *    peek
 *
 * will execute those corresponding commands on the priority queue.
 * You can use this code to interactively step through your priority
 * queue.
 */
template <typename PQueue>
	void replTestPriorityQueue() {
	printReplInstructions();
	
	/* These curly braces introduce a new layer of scoping.  This ensures that if
	 * your priority queue's destructor causes an error, the error occurs before
	 * this function returns.
	 */
	{
		PQueue queue;
		while (true) {
			/* Get a command from the user. */
			istringstream command(getLine("Enter command: "));
		
			/* Extract the action. */
			string action;
			command >> action >> ws;
		
			if (!command) {
				cout << "Please enter a command." << endl;
			} else {
				try {
					if (action == "peek" || action == "p") {
						cout << queue.peek() << endl;
					} else if (action == "isEmpty" || action == "i") {
						cout << boolalpha << queue.isEmpty() << endl;
					} else if (action == "size" || action == "s") {
						cout << queue.size() << endl;
					} else if (action == "dequeueMin" || action == "d") {
						cout << queue.dequeueMin() << endl;
					} else if (action == "quit" || action == "q") {
						cout << "Leaving test environment...   " << flush;
						break;
					} else if (action == "enqueue" || action == "e") {
						string toEnqueue;
						if (!getline(command, toEnqueue)) {
							cout << "Must specify a string to enqueue." << endl;
						} else {
							queue.enqueue(toEnqueue);
							cout << "Enqueued string \"" << toEnqueue << "\"." << endl;
						}
					} else {
						cout << "Unknown command." << endl;
					}
				
				} catch (ErrorException& e) {
					cout << "ERROR: " << e.getMessage() << endl;
				} catch (exception& e) {
					cout << "ERROR: " << e.what() << endl;
				} catch (...) {
					cout << "Unknown error." << endl;
				}
			}
		}
	}
	
	/* If we made it here, the destructor didn't crash. */
	cout << "success." << endl;
	cout << endl;
}

/* Constants for the menu. */
enum {
	REPL_VECTOR = 1,
	TEST_VECTOR,
	REPL_LINKED_LIST,
	TEST_LINKED_LIST,
	REPL_DOUBLY_LINKED_LIST,
	TEST_DOUBLY_LINKED_LIST,
	REPL_HEAP,
	TEST_HEAP,
	REPL_FIBONACCI,
	TEST_FIBONACCI,
    SPEED_TEST,
	QUIT
};

/* Function: displayMenu
 * ------------------------------------------------------------------
 * Displays the menu of testing options.
 */
void displayMenu() {
	cout << "CS106B Priority Queue Testing Harness" << endl;
	cout << "=====================================" << endl;
	cout << REPL_VECTOR << ": Manually test VectorPriorityQueue" << endl;
	cout << TEST_VECTOR << ": Automatically test VectorPriorityQueue" << endl;
	cout << REPL_LINKED_LIST << ": Manually test LinkedListPriorityQueue" << endl;
	cout << TEST_LINKED_LIST << ": Automatically test LinkedListPriorityQueue" << endl;
	cout << REPL_DOUBLY_LINKED_LIST << ": Manually test DoublyLinkedListPriorityQueue" << endl;
	cout << TEST_DOUBLY_LINKED_LIST << ": Automatically test DoublyLinkedListPriorityQueue" << endl;
	cout << REPL_HEAP << ": Manually test HeapPriorityQueue" << endl;
	cout << TEST_HEAP << ": Automatically test HeapPriorityQueue" << endl;
	cout << REPL_FIBONACCI << ": Manually test FibonacciHeapPriorityQueue" << endl;
	cout << TEST_FIBONACCI << ": Automatically test FibonacciHeapPriorityQueue" << endl;
    cout << SPEED_TEST << ": Test speeds of various queue implementations (takes a while)" << endl;
	cout << QUIT << ": Quit" << endl;
}

/* Prompts you to choose which tests to run.  Please feel free to add or edit
 * the tests in this file.
 */
int main() {
	while (true) {
		displayMenu();
		
		/* Respond to the user's choice. */
		switch (getInteger("Enter choice: ")) {
		case TEST_VECTOR:
			testPriorityQueue<VectorPriorityQueue> ();
			break;
		case REPL_VECTOR:
			replTestPriorityQueue<VectorPriorityQueue> ();
			break;
		case TEST_LINKED_LIST:
			testPriorityQueue<LinkedListPriorityQueue> ();
			break;
		case REPL_LINKED_LIST:
			replTestPriorityQueue<LinkedListPriorityQueue> ();
			break;
		case TEST_DOUBLY_LINKED_LIST:
			testPriorityQueue<DoublyLinkedListPriorityQueue> ();
			break;
		case REPL_DOUBLY_LINKED_LIST:
			replTestPriorityQueue<DoublyLinkedListPriorityQueue> ();
			break;			
		case TEST_HEAP:
			testPriorityQueue<HeapPriorityQueue> ();
			break;
		case REPL_HEAP:
			replTestPriorityQueue<HeapPriorityQueue> ();
			break;
		case TEST_FIBONACCI:
			testPriorityQueue<FibonacciHeapPriorityQueue> ();
			break;
		case REPL_FIBONACCI:
			replTestPriorityQueue<FibonacciHeapPriorityQueue> ();
			break;
        case SPEED_TEST:
            runSpeedTests();
            break;
		case QUIT:
			return 0;
		default:
			cout << "Sorry, but I don't know how to do that." << endl;
			break;
		}
	}
	
	return 0;
}

