import java.util.Arrays;

public class A5Tester {
	
	private static int testPassCount = 0;
	private static int testCount = 0;
	private static double THRESHOLD = 0.001; // allowable margin of error for floating point results
	
    public static void main(String[] args) {
		
		// PART I
		// implement and test each of the following methods
		// found in A5Exercises.java, one at a time
		
		testSumEven();
		testAddXToAll();
		testGetMaximum();
		testComesAfter();

		// PART II
		// implement and test each of the following methods
		// found in A5Exercises.java, one at a time.
		// Documentation can be found in A5Exercises.java

		testBeadsCount();
		testBeadBoxesCount();
		testTotalWeight();
		testAverageWeight();
		testStackedCorrectly();
		
		System.out.println("Passed " + testPassCount + " / " + testCount + " tests");
	}

	public static void testSumEven() {
		System.out.println("\nStarting sumEven tests");
		
		int[] arr0 = {};
		int[] arr1 = {4};
		int[] arr2 = {2, 7};
		int[] arr3 = {3, 7};
		int[] arr4 = {2, 3, 12, 4, 5, 23, 14};
		
		int result = 0;
		int expected = 0;
		
		result = A5Exercises.sumEven(arr0);
		expected = 0;
		displayResults(result==expected, "sumEven in empty array");

		result = A5Exercises.sumEven(arr1);
		expected = 4;
		displayResults(result==expected, "sumEven in arr1");
		
		result = A5Exercises.sumEven(arr2);
		expected = 2;
		displayResults(result==expected, "sumEven in arr2");

		result = A5Exercises.sumEven(arr3);
		expected = 0;
		displayResults(result==expected, "sumEven in arr3");
		
		result = A5Exercises.sumEven(arr4);
		expected = 2 + 12 + 4 + 14;
		displayResults(result==expected, "sumEven in arr3");
		
		// TODO: add more tests here
				
	}

	public static void testAddXToAll() {
		System.out.println("\nStarting addXToAll tests");
		
		int[] arr0 = {};
		int[] arr1 = {7, 1, 3};
		
		int[] expected0  = {};
		int[] expected1a = {9, 3, 5};
		
		A5Exercises.addXToAll(arr0, 5);
		displayResults(Arrays.equals(arr0, expected0), "add 5 to all values in empty array");
		
		A5Exercises.addXToAll(arr1, 2);
		displayResults(Arrays.equals(arr1, expected1a), "add 2 to all values in arr1");
		
		// TODO: add more tests here
	}
	
	public static void testGetMaximum() {
		System.out.println("\nStarting getMaximum tests");
		int[] arr0 = {};
		int[] arr1 = {7};
		int[] arr2 = {7, 8};
		int[] arr3 = {8, 7};
		int[] arr4 = {1, 2, 3, 4, 5};
		int[] arr5 = {9, 8, 7, 6, 5};
		
		int result = 0;
		int expected = 0;
		
		result = A5Exercises.getMaximum(arr0);
		expected = -1;
		displayResults(result==expected, "maximum of empty array");
		
		result = A5Exercises.getMaximum(arr1);
		expected = 7;
		displayResults(result==expected, "maximum of arr1");
		
		result = A5Exercises.getMaximum(arr2);
		expected = 8;
		displayResults(result==expected, "maximum of arr2");
		
		result = A5Exercises.getMaximum(arr3);
		expected = 8;
		displayResults(result==expected, "maximum of arr3");
		
		result = A5Exercises.getMaximum(arr4);
		expected = 5;
		displayResults(result==expected, "maximum of arr4");
		
		result = A5Exercises.getMaximum(arr5);
		expected = 9;
		displayResults(result==expected, "maximum of arr5");
		
		// TODO: add more tests here
		
	}

	public static void testComesAfter() {
		System.out.println("\nStarting comesAfter tests");
		int[] arr0 = {};
		int[] arr1 = {7};
		int[] arr2 = {7, 8};
		int[] arr3 = {3, 4, 3};
		int[] arr4 = {1, 2, 1, 1, 3, 4, 4, 4, 3};
		
		boolean result = false;
		boolean expected = false;
		
		result = A5Exercises.comesAfter(arr0, 8, 7);
		expected = false;
		displayResults(result==expected, "an 8 comes after first 7 in empty array");
		
		result = A5Exercises.comesAfter(arr1, 8, 7);
		expected = false;
		displayResults(result==expected, "an 8 comes after first 7 in arr1");
		
		result = A5Exercises.comesAfter(arr2, 8, 7);
		expected = true;
		displayResults(result==expected, "an 8 comes after first 7 in arr2");
		
		result = A5Exercises.comesAfter(arr2, 9, 8);
		expected = false;
		displayResults(result==expected, "a 9 comes after first 8 in arr2");
		
		result = A5Exercises.comesAfter(arr2, 7, 6);
		expected = false;
		displayResults(result==expected, "a 7 comes after first 6 in arr2");
		
		result = A5Exercises.comesAfter(arr3, 4, 3);
		expected = true;
		displayResults(result==expected, "a 4 comes after first 3 in arr3");
		
		result = A5Exercises.comesAfter(arr3, 3, 4);
		expected = false;
		displayResults(result==expected, "a 3 comes after first 4 in arr3");
		
		// TODO: add more tests here

	}

	
	public static void testBeadsCount() {
		System.out.println("\nStarting beadsCount tests");
		
		BeadBox b1 = new BeadBox(200, 0.01);
		BeadBox b2 = new BeadBox(500, 0.01);
		BeadBox b3 = new BeadBox(1000, 0.02);
		BeadBox b4 = new BeadBox(1000, 0.05);
		BeadBox b5 = new BeadBox(2000, 0.02);
		BeadBox b6 = new BeadBox(2000, 0.05);
		
		BeadBox[] arr1 = {b1};
		BeadBox[] arr2 = {b1, b2, b3};
		BeadBox[] arr3 = {b1, b2, b3, b4, b5, b6};
		
		A5Stack<BeadBox> s0 = new A5Stack<BeadBox>();
		A5Stack<BeadBox> s1 = new A5Stack<BeadBox>(arr1);
		A5Stack<BeadBox> s2 = new A5Stack<BeadBox>(arr2);
		A5Stack<BeadBox> s3 = new A5Stack<BeadBox>(arr3);
		
		int result = 0;
		int expected = 0;
		
		result = A5Exercises.beadsCount(s0);
		expected = 0;
		displayResults(result==expected, "total beads in empty stack");
		
		result = A5Exercises.beadsCount(s1);
		expected = 200;
		displayResults(result==expected, "total beads in s1");
		displayResults(s1.toString().equals(Arrays.toString(arr1)), "stack contents unchanged");
		// System.out.println(s1);
		
		result = A5Exercises.beadsCount(s2);
		expected = 200 + 500 + 1000;
		// System.out.println(result);
		displayResults(result==expected, "total beads in s2");
		displayResults(s2.toString().equals(Arrays.toString(arr2)), "stack contents unchanged");
		
		result = A5Exercises.beadsCount(s3);
		expected = 200 + 500 + 1000 + 1000 + 2000 + 2000;
		// System.out.println(result);
		displayResults(result==expected, "total beads in s3");	
		displayResults(s3.toString().equals(Arrays.toString(arr3)), "stack contents unchanged");
			
	}
	
	public static void testBeadBoxesCount() {
		System.out.println("\nStarting beadsBoxesCount tests");
		
		BeadBox b1 = new BeadBox(200, 0.01);
		BeadBox b2 = new BeadBox(500, 0.01);
		BeadBox b3 = new BeadBox(1000, 0.02);
		BeadBox b4 = new BeadBox(1000, 0.05);
		BeadBox b5 = new BeadBox(2000, 0.02);
		BeadBox b6 = new BeadBox(2000, 0.05);
		
		BeadBox[] arr1 = {b1};
		BeadBox[] arr2 = {b1, b2, b3};
		BeadBox[] arr3 = {b1, b2, b3, b4, b5, b6};
		
		A5Stack<BeadBox> s0 = new A5Stack<BeadBox>();
		A5Stack<BeadBox> s1 = new A5Stack<BeadBox>(arr1);
		A5Stack<BeadBox> s2 = new A5Stack<BeadBox>(arr2);
		A5Stack<BeadBox> s3 = new A5Stack<BeadBox>(arr3);
		
		int result = 0;
		int expected = 0;
		
		result = A5Exercises.beadBoxesCount(s0);
		expected = 0;
		displayResults(result==expected, "total bead boxes in empty stack");
		
		result = A5Exercises.beadBoxesCount(s1);
		expected = 1;
		displayResults(result==expected, "total bead boxes in s1");
		displayResults(s1.toString().equals(Arrays.toString(arr1)), "stack contents unchanged");
		// System.out.println(s1);
		
		result = A5Exercises.beadBoxesCount(s2);
		expected = 3;
		// System.out.println(result);
		displayResults(result==expected, "total bead boxes in s2");
		displayResults(s2.toString().equals(Arrays.toString(arr2)), "stack contents unchanged");
		
		result = A5Exercises.beadBoxesCount(s3);
		expected = 6;
		// System.out.println(result);
		displayResults(result==expected, "total bead boxes in s3");	
		displayResults(s3.toString().equals(Arrays.toString(arr3)), "stack contents unchanged");
			
	}
	public static void testTotalWeight() {
		System.out.println("\nStarting totalWeight tests");
		
		BeadBox b1 = new BeadBox(200, 0.01);
		BeadBox b2 = new BeadBox(500, 0.01);
		BeadBox b3 = new BeadBox(1000, 0.02);
		BeadBox b4 = new BeadBox(1000, 0.05);
		BeadBox b5 = new BeadBox(2000, 0.02);
		BeadBox b6 = new BeadBox(2000, 0.05);
		
		BeadBox[] arr1 = {b1};
		BeadBox[] arr2 = {b1, b2, b3};
		BeadBox[] arr3 = {b1, b2, b3, b4, b5, b6};
		
		A5Stack<BeadBox> s0 = new A5Stack<BeadBox>();
		A5Stack<BeadBox> s1 = new A5Stack<BeadBox>(arr1);
		A5Stack<BeadBox> s2 = new A5Stack<BeadBox>(arr2);
		A5Stack<BeadBox> s3 = new A5Stack<BeadBox>(arr3);
			
		double result = 0.0;
		double expected = 0.0;
		
		result = A5Exercises.totalWeight(s0);
		expected = 0.00;
		displayResults(Math.abs(result-expected)<THRESHOLD, "total weight in empty stack");
		
		result = A5Exercises.totalWeight(s1);
		expected = 2.00;
		displayResults(Math.abs(result-expected)<THRESHOLD, "total weight in s1");
		displayResults(s1.toString().equals(Arrays.toString(arr1)), "stack contents unchanged");
		
		result = A5Exercises.totalWeight(s2);
		expected = 2.00 + 5.00 + 20.00;
		// System.out.println(result);
		displayResults(Math.abs(result-expected)<THRESHOLD, "total weight in s2");
		displayResults(s2.toString().equals(Arrays.toString(arr2)), "stack contents unchanged");
		
		result = A5Exercises.totalWeight(s3);
		expected = 2.00 + 5.00 + 20.00 + 50.00 + 40.00 + 100.00;
		// System.out.println(result);
		displayResults(Math.abs(result-expected)<THRESHOLD, "total weight in s3");	
		displayResults(s2.toString().equals(Arrays.toString(arr2)), "stack contents unchanged");
	}
	
	public static void testAverageWeight() {
		System.out.println("\nStarting averageWeight tests");
		
		BeadBox b1 = new BeadBox(200, 0.01);
		BeadBox b2 = new BeadBox(500, 0.01);
		BeadBox b3 = new BeadBox(1000, 0.02);
		BeadBox b4 = new BeadBox(1000, 0.05);
		BeadBox b5 = new BeadBox(2000, 0.02);
		BeadBox b6 = new BeadBox(2000, 0.05);
		
		BeadBox[] arr1 = {b1};
		BeadBox[] arr2 = {b1, b2, b3};
		BeadBox[] arr3 = {b1, b2, b3, b4, b5, b6};
		
		A5Stack<BeadBox> s0 = new A5Stack<BeadBox>();
		A5Stack<BeadBox> s1 = new A5Stack<BeadBox>(arr1);
		A5Stack<BeadBox> s2 = new A5Stack<BeadBox>(arr2);
		A5Stack<BeadBox> s3 = new A5Stack<BeadBox>(arr3);
			
		double result = 0.0;
		double expected = 0.0;
		
	
		result = A5Exercises.averageWeight(s0);
		expected = 0.0;
		displayResults(Math.abs(result-expected)<THRESHOLD, "average weight in empty stack");
		
		result = A5Exercises.averageWeight(s1);
		expected = 2.00/1.0;
		displayResults(Math.abs(result-expected)<THRESHOLD, "average weight in s1");
		displayResults(s1.toString().equals(Arrays.toString(arr1)), "stack contents unchanged");
		
		result = A5Exercises.averageWeight(s2);
		expected = (2.00 + 5.00 + 20.00)/3.0;
		// System.out.println(result);
		displayResults(Math.abs(result-expected)<THRESHOLD, "average weight in s2");
		displayResults(s2.toString().equals(Arrays.toString(arr2)), "stack contents unchanged");
		
		result = A5Exercises.averageWeight(s3);
		expected = (2.00 + 5.00 + 20.00 + 50.00 + 40.00 + 100.00)/6.0;
		// System.out.println(result);
		displayResults(Math.abs(result-expected)<THRESHOLD, "average weight in s3");	
		displayResults(s3.toString().equals(Arrays.toString(arr3)), "stack contents unchanged");
	}
	
	
	public static void testStackedCorrectly() {
		System.out.println("\nStarting stackedCorrectly tests");
		
		BeadBox b1 = new BeadBox(200, 0.01);
		BeadBox b2 = new BeadBox(500, 0.01);
		BeadBox b3 = new BeadBox(1000, 0.02);
		BeadBox b4 = new BeadBox(1000, 0.05);
		BeadBox b5 = new BeadBox(2000, 0.02);
		BeadBox b6 = new BeadBox(2000, 0.05);
		
		BeadBox[] arr1 = {b1}; // trivially correct
		BeadBox[] arr2 = {b1, b2}; // stacked correctly
		BeadBox[] arr3 = {b2, b1}; // stacked incorrectly
		
		A5Stack<BeadBox> s0 = new A5Stack<BeadBox>();
		A5Stack<BeadBox> s1 = new A5Stack<BeadBox>(arr1);
		A5Stack<BeadBox> s2 = new A5Stack<BeadBox>(arr2);
		A5Stack<BeadBox> s3 = new A5Stack<BeadBox>(arr3);
		
		String s0Contents = s0.toString();
		String s1Contents = s1.toString();
		String s2Contents = s2.toString();
		String s3Contents = s3.toString();
			
		boolean result = false;
		boolean expected = false;
		
		result = A5Exercises.stackedCorrectly(s0);
		expected = true;
		displayResults(result==expected, "empty stack stacked correctly");
		displayResults(s0Contents.equals(s0.toString()), "stack unchanged");
		
		result = A5Exercises.stackedCorrectly(s1);
		expected = true;
		displayResults(result==expected, "s1 stacked correctly");
		displayResults(s1Contents.equals(s1.toString()), "stack unchanged");
		
		result = A5Exercises.stackedCorrectly(s2);
		expected = true;
		displayResults(result==expected, "s2 stacked correctly");
		displayResults(s2Contents.equals(s2.toString()), "stack unchanged");

		result = A5Exercises.stackedCorrectly(s3);
		expected = false;
		displayResults(result==expected, "s3 stacked correctly");
		displayResults(s3Contents.equals(s3.toString()), "stack unchanged");

		// TODO: add more tests here
		
	}
	
	public static void displayResults (boolean passed, String testName) {
		testCount++;
		if (passed) {
			System.out.println ("Passed test: " + testName);
			testPassCount++;
		} else {
			System.out.println ("Failed test: " + testName + " at line "
				+ Thread.currentThread().getStackTrace()[2].getLineNumber());
		}
    }
}