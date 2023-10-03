/*
 * A3Tester
 *
 * A class to test the methods required for Assignment 3
 *
 */

import java.util.NoSuchElementException;

public class A3Tester {
    
	private static int testPassCount = 0;
	private static int testCount = 0;

    public static void main(String[] args) {
		
		/*
		Uncomment each method one at a time, 
		and implement the method in A3LinkedList.java 
		until all tests pass. It is strongly
		recommended that you add additional tests 
		to ensure the correctness of your implementation.
		*/
		
		/* Part 1: */
		testAddFront();
		testAddBack();
		 testSizeAndIsEmpty();
		 testRemoveFront();
		 testRemoveBack();
		
		/* Part 2 */
		testInsertInto();
testRotate();
		
		System.out.println("Passed " + testPassCount + " / " + testCount + " tests");
    }
	
		
	public static void testAddFront() {
		String result = "";
		A3LinkedList list1 = new A3LinkedList();
		
		result = list1.frontToBack();
		displayResults(result.equals("{}"), "testAddFront");

		list1.addFront("A");
		result = list1.frontToBack();
		displayResults(result.equals("{A}"), "testAddFront");
		
		list1.addFront("V");
		list1.addFront("A");
		list1.addFront("J");
		result = list1.frontToBack();
		displayResults(result.equals("{JAVA}"), "testAddFront");
		result = list1.backToFront();
		displayResults(result.equals("{AVAJ}"), "testAddFront");
		
		//my tests
		A3LinkedList List = new A3LinkedList();
		List.addFront("a");
		List.addFront("b");
		List.addFront("c");
		List.addFront(null);
		
		result = List.frontToBack();
		displayResults(result.equals(("{nullcba}")), result);
		
		result = List.backToFront();
		displayResults(result.equals(("{abcnull}")), result);

		A3LinkedList List2 = new A3LinkedList();
		List2.addFront("start");

		result = List2.frontToBack();
		displayResults(result.equals("{start}"), result);

		result = List2.backToFront();
		displayResults(result.equals("{start}"), result);
		
		
	}
	
	public static void testAddBack() {
		String result = "";
		A3LinkedList list1 = new A3LinkedList();
		
		result = list1.frontToBack();
		displayResults(result.equals("{}"), "testAddBack");
		
		list1.addBack("F");
		result = list1.frontToBack();
		displayResults(result.equals("{F}"), "testAddBack");
		
		list1.addBack("U");
		list1.addBack("N");
		result = list1.frontToBack();
		displayResults(result.equals("{FUN}"), "testAddBack");
		result = list1.backToFront();
		displayResults(result.equals("{NUF}"), "testAddBack");

		//my tests

		A3LinkedList List = new A3LinkedList();
		List.addBack("end");

		result = List.frontToBack();
		displayResults(result.equals("{end}"), result);

		result = List.backToFront();
		displayResults(result.equals("{end}"), result);

	}
	
	public static void testSizeAndIsEmpty() {
		int result = 0;
		A3LinkedList list1 = new A3LinkedList();

		result = list1.size();
		displayResults(result==0, "testSizeAndIsEmpty");
		displayResults(list1.isEmpty()==true, "testSizeAndIsEmpty");
		
		list1.addFront("C");
		list1.addFront("S");
		list1.addFront("C");
		result = list1.size();
		displayResults(result==3, "testSizeAndIsEmpty");
		displayResults(list1.isEmpty()==false, "testSizeAndIsEmpty");
		
		list1.addBack("115");
		result = list1.size();
		displayResults(result==4, "testSizeAndIsEmpty");
		
	}
	
	public static void testRemoveFront() {
		String result = "";
		A3LinkedList list1 = new A3LinkedList();
		
		list1.addBack("P");
		list1.addBack("I");
		list1.addBack("N");
		list1.addBack("K");
		result = list1.frontToBack();
		displayResults(result.equals("{PINK}"), "testRemoveFront");
		
		list1.removeFront();
		result = list1.frontToBack();
		displayResults(result.equals("{INK}"), "testRemoveFront");
		
		/* Write additional tests here to ensure all of your pointers
		   have been updated correctly.
		
		   You should also ensure that your size and isEmpty 
		   methods work with removal as well as addition */
		//my tests
		A3LinkedList List = new A3LinkedList();
		try {
			List.removeFront();
		} catch(NoSuchElementException e) {
			result  = e.getMessage();
			displayResults(result.equals("Cannot remove from an empty linked list"), result);

			displayResults(List.size() == 0, "List length should be 0");
		}
				
	}
	
	public static void testRemoveBack() {
		// Write all of your own tests here 
		String result = "";
		A3LinkedList List = new A3LinkedList();
		
		List.addBack("P");
		List.addBack("I");
		List.addBack("Z");
		List.addBack("Z");
		List.addBack("E");
		List.addBack("R");
		List.addBack("I");
		List.addBack("A");

		result = List.frontToBack();
		displayResults(result.equals("{PIZZERIA}"), result);

		result = List.backToFront();
		displayResults(result.equals("{AIREZZIP}"), result);

		displayResults(List.size() == 8, "Size should be 8");

		A3LinkedList List1 = new A3LinkedList();
		try {

			List1.removeBack();

		} catch(NoSuchElementException e) {
			result = e.getMessage();
			displayResults(result.equals("Cannot remove from an empty linked list"), result);

			displayResults(List1.size() == 0, "Size should be 0");
		}


	}
	
	public static void testInsertInto() {
		A3LinkedList list1 = new A3LinkedList();
		
		String result1;
		
		list1.addBack("c");
		list1.addBack("s");
		list1.addBack("y");
		
		list1.insertInto("h");
		result1 = list1.frontToBack();
		displayResults(result1.equals("{chsy}"), "insertInto h");
		
		list1.insertInto("t");
		result1 = list1.frontToBack();
		displayResults(result1.equals("{chsty}"), "insertInto t");
		
		list1.insertInto("z");
		result1 = list1.frontToBack();
		displayResults(result1.equals("{chstyz}"), "insertInto z");
		
		// Write more tests here
		A3LinkedList List = new A3LinkedList();
		List.addFront("ab");
		List.insertInto("c");

		result1 = List.frontToBack();
		displayResults(result1.equals("{abc}"), "List should be abc");
		
		
		A3LinkedList Lista = new A3LinkedList();
		Lista.addFront("ONE");
		Lista.addBack("OOE");
		Lista.insertInto("ONE");

		result1 = Lista.frontToBack();
		displayResults(result1.equals("{ONEONEOOE}"), result1);


	}
	
	
	public static void testRotate() {
		String result = "";
		A3LinkedList list1 = new A3LinkedList();
		
		list1.addBack("A");
		list1.addBack("B");
		list1.addBack("C");
		list1.addBack("D");
		list1.addBack("E");
		list1.addBack("F");
		list1.addBack("G");
		result = list1.frontToBack();
		displayResults(result.equals("{ABCDEFG}"), "testRotate");
		
		list1.rotate(1);
		result = list1.frontToBack();
		displayResults(result.equals("{GABCDEF}"), "testRotate");
		
		list1.rotate(3);
		result = list1.frontToBack();
		displayResults(result.equals("{DEFGABC}"), "testRotate");
		
		/* Write additional tests here to ensure all of your pointers
		   have been updated correctly. */
		
	}

	public static void displayResults (boolean passed, String testName) {
       /* There is some magic going on here getting the line number
        * Borrowed from:
        * http://blog.taragana.com/index.php/archive/core-java-how-to-get-java-source-code-line-number-file-name-in-code/
        */
        
        testCount++;
        if (passed)
        {
            System.out.println ("Passed test: " + testName);
            testPassCount++;
        }
        else
        {
            System.out.println ("Failed test: " + testName + " at line "
                                + Thread.currentThread().getStackTrace()[2].getLineNumber());
        }
    }
	
}
