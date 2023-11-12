/*
* A7Tester.java
* A test program for Assignment 7.
*/

import java.util.Random;

public class A7Tester {
	private static int testPassCount = 0;
	private static int testCount = 0;
	public static boolean testHeapSolution = true;

	public static void main (String[] args) {
		
		if (args.length != 0 && args[0].equals("linked")) {
			testHeapSolution=false;
		}

		System.out.println("Testing " + (testHeapSolution ? "Heap" : "Linked" ) + " implementation.");
		
		testSize();
		testInsertionAndRemoval();
		testExceptions();
		testGeneric();
		testDuplicates();
		testMixed();
		//stressTest();

		System.out.println("Testing Application using Priority Queue.");
		testTask();
		testToDoList();
		

		System.out.println("PASSED " + testPassCount + "/" + testCount + " tests");
	}

	public static<T extends Comparable<T>> PriorityQueue<T> createNewPriorityQueue(Class<T> type)  {
		if (testHeapSolution) {
			return new HeapPriorityQueue<T>(type);
		} else {
			return new LinkedPriorityQueue<T>();
		}
	}

	public static<T extends Comparable<T>> PriorityQueue<T> createNewPriorityQueue(Class<T> type, int size)  {
		if (testHeapSolution) {
			return new HeapPriorityQueue<T>(type, size);
		}
		else {
			return new LinkedPriorityQueue<T>();
		}
	}


	public static void testSize() {
		System.out.println("\nBasic testing of size, isEmpty");
		PriorityQueue<Integer> pq = createNewPriorityQueue(Integer.class);
		
		displayResults (pq.size() == 0, "size on empty PQ");
		displayResults (pq.isEmpty(), "isEmpty on empty PQ");

		pq.insert(10);
		displayResults (pq.size() == 1, "size on 1 element PQ");
		displayResults (!pq.isEmpty(), "isEmpty on 1 element PQ");

		pq.insert(9);
		displayResults (pq.size() == 2, "size on 2 element PQ");

		pq.insert(7);
		displayResults (pq.size() == 3, "size on 3 element PQ");
	}

	public static void testInsertionAndRemoval() {
		System.out.println("\nBasic testing of removeMin");
		PriorityQueue<Integer> pq = createNewPriorityQueue(Integer.class);
		
		pq.insert(8);
		pq.insert(9);
		pq.insert(10);

		Integer result;
		result = pq.removeMin();
		// System.out.println("res: " + result + ":" + pq);
		displayResults(result.equals(8), "remove on multiple element PQ");
		displayResults(pq.size() == 2, "remove + size on multiple element PQ");

		result = pq.removeMin();
		// System.out.println("res: " + result + ":" + pq);
		displayResults(result.equals(9), "remove on multiple element PQ");
		displayResults(pq.size() == 1, "remove + size on multiple element PQ");

		result = pq.removeMin();
		displayResults(result.equals(10), "remove + size on 1 element PQ");
		displayResults(pq.isEmpty(), "remove + isEmpty on 1 element PQ");

		pq = createNewPriorityQueue(Integer.class);
		pq.insert(3);
		pq.insert(2);
		pq.insert(1);

		System.out.println("queue " + pq);
		result = pq.removeMin();
		System.out.println("removed " + result);
		displayResults(result.equals(1), "insert + remove on multiple element PQ");
		displayResults(pq.size() == 2, "insert + remove + size on multiple element PQ");

		System.out.println("queue " + pq);
		result = pq.removeMin();
		System.out.println("removed " + result);
		displayResults(result.equals(2), "insert + remove on multiple element PQ");
		displayResults(pq.size() == 1, "insert + remove + size on multiple element PQ");

		result = pq.removeMin();
		displayResults(result.equals(3), "insert + remove on 1 element PQ");
		displayResults(pq.isEmpty(), "insert + remove + size on 1 element PQ");
	}

	public static void testExceptions() {
		System.out.println("\nTesting of exceptions");
		PriorityQueue<Integer> pq = createNewPriorityQueue(Integer.class, 2);
		try {
			pq.removeMin();
			displayResults(false, "exception should have been thrown");
		} catch (HeapFullException e) {
			displayResults(false, "different exception should have been thrown");
		} catch (HeapEmptyException e) {
			displayResults(true, "HeapEmptyException should be thrown");
		}
		
		pq.insert(10);
		pq.insert(9);
		try {
			pq.insert(8);
			if(testHeapSolution) {
				displayResults(false, "exception should have been thrown in heap solution");
			} else {
				displayResults(true, "exception should not have been thrown in linked version");
			} 
		} catch (HeapEmptyException e) {
			displayResults(false, "different exception should have been thrown");
		} catch (HeapFullException e) {
			displayResults(true, "HeapFullException should be thrown");
		}
	}

	public static void testGeneric() {
		System.out.println("\nTesting of removeMin with Strings");
		PriorityQueue<String> pq = createNewPriorityQueue(String.class);
		String result;

		pq.insert("abc");
		pq.insert("def");
		pq.insert("ghi");

		result = pq.removeMin();
		displayResults(result.equals("abc"), "insert/remove Strings");
		displayResults(pq.size() == 2, "insert/remove Strings");

		result = pq.removeMin();
		displayResults(result.equals("def"), "insert/remove Strings");
		displayResults(pq.size() == 1, "insert/remove Strings");

		result = pq.removeMin();
		displayResults(result.equals("ghi"), "insert/remove Strings");
		displayResults(pq.isEmpty(), "insert/remove Strings");

		pq = createNewPriorityQueue(String.class);
		pq.insert("ghi");
		pq.insert("def");
		pq.insert("abc");

		result = pq.removeMin();
		displayResults(result.equals("abc"), "insert/remove Strings");
		displayResults(pq.size() == 2, "insert/remove + size Strings");

		result = pq.removeMin();
		displayResults(result.equals("def"), "insert/remove Strings");
		displayResults(pq.size() == 1, "insert/remove + size Strings");

		result = pq.removeMin();
		displayResults(result.equals("ghi"), "insert/remove Strings");
		displayResults(pq.isEmpty(), "insert/remove + isEmpty Strings");
	}

	public static void testDuplicates() {
		System.out.println("\nTesting duplicates.");
		PriorityQueue<Integer> pq = createNewPriorityQueue(Integer.class);
		Integer result;

		pq.insert(4);
		pq.insert(1);
		pq.insert(1);
		pq.insert(5);
		pq.insert(0);
		// System.out.println("q after insert 4 1 1 5 0:" + q);
		result = pq.removeMin();
		displayResults(result == 0, "add duplicates + remove single");

		pq.insert(4);
		pq.insert(1);
		pq.insert(4);
		pq.insert(4);

		pq.insert(0);
		pq.insert(5);
		pq.insert(0);
		pq.insert(5);

		result = pq.removeMin();
		displayResults(result.equals(0), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(0), "add duplicates + remove duplicates");

		result = pq.removeMin();
		displayResults(result.equals(1), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(1), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(1), "add duplicates + remove duplicates");
		
		result = pq.removeMin();
		displayResults(result.equals(4), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(4), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(4), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(4), "add duplicates + remove duplicates");
		
		result = pq.removeMin();
		displayResults(result.equals(5), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(5), "add duplicates + remove duplicates");
		result = pq.removeMin();
		displayResults(result.equals(5), "add duplicates + remove duplicates");
		
		displayResults(pq.isEmpty(), "insert/remove + isEmpty");

	}

	public static void testMixed() {
		System.out.println("\nTesting insert mixed with removeMin.");
		PriorityQueue<Integer> pq = createNewPriorityQueue(Integer.class);
		Integer result;
		
		pq.insert(2);
		pq.insert(0);
		pq.insert(5);
		pq.insert(7);

		result = pq.removeMin();
		displayResults(result.equals(0), "inserts + remove");

		pq.insert(4);
		result = pq.removeMin();
		displayResults(result.equals(2), "inserts + remove");

		pq.insert(11);
		pq.insert(2);
		pq.insert(3);
		pq.insert(1);
		result = pq.removeMin();
		displayResults(result.equals(1), "inserts + remove");
		result = pq.removeMin();
		displayResults(result.equals(2), "inserts + remove");
		result = pq.removeMin();
		displayResults(result.equals(3), "inserts + remove");
		result = pq.removeMin();
		displayResults(result.equals(4), "inserts + remove");

		pq.insert(1);
		result = pq.removeMin();
		displayResults(result.equals(1), "inserts + remove");
		displayResults(pq.size() == 3, "inserts + remove + size");
	}

	public static boolean testRandomArray (int count)
	{
		/* These tests are effectively sorting the random values
			- for the heap, this is O (n log n)
			- for the linked list, this is O (n^2)
		*/
		PriorityQueue<Integer> q = createNewPriorityQueue(Integer.class, count);
		System.out.println("Testing size: " + count);
		Random r = new Random();
		for ( int i = 0; i < count; i++ ) {
			int val = r.nextInt(1000000);
			q.insert (val);
		}

		int oldVal = 0; //smallest possible value val could be
		int i = 0;
		while (!q.isEmpty()) {
			int val = q.removeMin().intValue(); // or a bug
			if ( val < oldVal ) {
				return false;
			}
			oldVal = val;
			i++;
		}
		return true;

	}

	public static void stressTest() {
		System.out.println("\nStress Tests.");
		displayResults(testRandomArray(100), "inserts + removes");
		displayResults(testRandomArray(10000), "inserts + removes");
		displayResults(testRandomArray(100000), "inserts + removes");

		//This takes too long using the linked list.
		if (testHeapSolution) {
			displayResults(testRandomArray(1000000), "inserts + removes");
		}
		
	}


	public static void testTask() {
		
		System.out.println("Testing Task creation, compareTo and equals.");
		Task t1 = new Task(2, "Book customers' trips", new Time(12,24));
		Task t2 = new Task(4, "Create new package", new Time(12,24));
		Task t3 = new Task(2, "Buy park tickets", new Time(12,24) );
		Task t4 = new Task(4, "Book hotel rooms", new Time(9,14) );
		Task t5 = new Task(4, "Book hotel rooms", new Time(10,20) );

		displayResults(t1.compareTo(t2) < 0, "testing Task compareTo (priorityLevel)");	
		displayResults(t2.compareTo(t1) > 0, "testing Task compareTo (priorityLevel)");
		displayResults(t1.compareTo(t1) == 0, "testing Task compareTo (priorityLevel)");
		displayResults(t1.compareTo(t3) == 0, "testing Task compareTo (priorityLevel and insertionTime tiebreaker)");
		displayResults(t1.compareTo(t4) < 0, "testing Task compareTo (priorityLevel)");
		displayResults(t4.compareTo(t1) > 0, "testing Task compareTo (priorityLevel)");
		// Missing some compareTo tests. Add them!
		
		displayResults(t4.equals(t5), "testing Task equals");
		displayResults(!t1.equals(t4), "testing Task equals");
		
	}

	public static void testToDoList() {
		System.out.println("Testing adding/removing Tasks from a to-do list.");
		Task t1 = new Task(3, "Book customers' trips", new Time(12,24));
		Task t2 = new Task(4, "Create new package", new Time(12,24));
		Task t3 = new Task(3, "Buy park tickets", new Time(12,40) );
		Task t4 = new Task(2, "Book hotel rooms", new Time(9,14) );
		Task t5 = new Task(4, "Coffee with colleagues", new Time(7,12));
		Task t6 = new Task(1, "Meeting with boss", new Time(12,24));

		ToDoList tdl1 = new ToDoList();
		displayResults(tdl1.numberTasksToDo() == 0, "testing ToDoList constructor + numTasksWaiting");

		tdl1.addTask(t1);
		tdl1.addTask(t2);
		tdl1.addTask(t3);
		tdl1.addTask(t4);
		tdl1.addTask(t5);

		displayResults(tdl1.numberTasksToDo() == 5, "testing ToDoList addTask + numTasksWaiting");
		Task nextP = tdl1.nextTask();
		// System.out.println(nextP);
		displayResults(t4.equals(nextP), "testing ToDoList addTask + numTasksWaiting");
		displayResults(tdl1.numberTasksToDo() == 4, "testing ToDoList addTask + numTasksWaiting");

		tdl1.addTask(t6);
		displayResults(tdl1.numberTasksToDo() == 5, "testing ToDoList addTask + numTasksWaiting");

		nextP = tdl1.nextTask();
		displayResults(t6.equals(nextP), "testing ToDoList addTask + nextTask");
		nextP = tdl1.nextTask();
		displayResults(t1.equals(nextP), "testing ToDoList addTask + nextTask");
		displayResults(tdl1.numberTasksToDo() == 3, "testing ToDoList addTask + numTasksWaiting");

		nextP = tdl1.nextTask();
		displayResults(t3.equals(nextP), "testing ToDoList addTask + nextTask");
		nextP = tdl1.nextTask();
		displayResults(t5.equals(nextP), "testing ToDoList addTask + nextTask");
		nextP = tdl1.nextTask();
		displayResults(t2.equals(nextP), "testing ToDoList addTask + nextTask");
		displayResults(tdl1.numberTasksToDo() == 0, "testing ToDoList addTask + numTasksWaiting");

		nextP = tdl1.nextTask();
		displayResults(nextP == null, "testing ToDoList nextTask - no more tasks");

		ToDoList smallTDL = new ToDoList(2);
		smallTDL.addTask(t1);
		smallTDL.addTask(t2);

		try {
			smallTDL.addTask(t3);
			displayResults(true, "testing ToDoList addTask to full ToDoList - should get here without exception");
		} catch (HeapFullException e) {
			displayResults(false, "testing ToDoList addTask to full ToDoList - should not get here");
		}
		
	}


	public static void displayResults (boolean passed, String testName) {
		testCount++;
		if (passed) {
			System.out.println ("Passed test " +testCount+": " + testName);
			testPassCount++;
		} else {
			System.out.println ("Failed test: " + testName + " at line "
								+ Thread.currentThread().getStackTrace()[2].getLineNumber());
		}
	}
}
