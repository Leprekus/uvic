/*
 * Lab2Tester.java
 *
 * A tester for the methods in Lab2.java
 *
 */
import java.util.Arrays;

import javax.print.event.PrintEvent;

public class Lab2Tester {
    
	private static int testPassCount = 0;
	private static int testCount = 0;

	// for approximate comparison of floating point numbers
	private static final double THRESHOLD = 0.01;

	public static void main(String[] args) {
		
		testGetHigherGradeStudent();
		testIsGradeAbove();
		testGetClasslist();
		testCountAbove();
		testGetClassAverage();
		testRegisterStudent();

		System.out.println("Passed " + testPassCount + " / " + testCount + " tests");
	}
	
	public static void testGetHigherGradeStudent() {
		// TODO: once you have implemented getHigherGradeStudent in Lab2.java
		//  uncomment the following tests - make sure you understand what they are testing

		
		Student s0  = new Student("abc", 50);
		Student s1a = new Student("def", 56);
		Student s1b = new Student("xyz", 56);
		Student s2  = new Student("xyz", 99);

		Student result;

		result = Lab2.getHigherGradeStudent(s0,s1a);
		//System.out.println("should be  " + s1a + " is " + result);
		displayResults(result.equals(s1a), "testGetHigherGradeStudent");

		result = Lab2.getHigherGradeStudent(s1a,s0);
		//System.out.println("should be  " + s1a + " is " + result);
		displayResults(result.equals(s1a), "testGetHigherGradeStudent");

		result = Lab2.getHigherGradeStudent(s1b,s1a);
		//System.out.println("should be  " + s1b + " is " + result);
		displayResults(result.equals(s1b) && result == s1b, "testGetHigherGradeStudent");

		result = Lab2.getHigherGradeStudent(s1b,s2);
		//System.out.println("should be  " + s2 + " is " + result);
		displayResults(result.equals(s2), "testGetHigherGradeStudent");
		
	}

	public static void testIsGradeAbove() {
		// TODO: write tests for Lab2.isGradeAbove
		System.out.println("tstIsGradeAbove");
		boolean result;
		boolean expected;
		Student s_greater = new Student("abc", 69);
		Student s_equal = new Student("abc", 70);
		Student s_less = new Student("abc", 80);

		result = Lab2.isGradeAbove(s_greater, 68);
		expected =  result == true ? true : false;
		System.out.println("Passed: " + expected);
		

		result = Lab2.isGradeAbove(s_equal, 70);
		expected =  result == false ? true: false;
		System.out.println("Passed: " + expected);

		result = Lab2.isGradeAbove(s_less, 100);
		expected =  result == false ? true : false;
		System.out.println("Passed: " + expected);
		
		

	}

	public static void testGetClasslist() {
		System.out.println("testGetClasslist");
		// TODO: write tests for Lab2.getClasslist
		Student s1 = new Student("abc", 99);
		Student s2 = new Student("def", 99);
		Student s3 = new Student("hij", 99);
		Student s4 = new Student("klm", 99);
		Student s5 = new Student("nop", 99);
		Student s6 = new Student("qrs", 99);
		Student s7 = new Student("tuv", 99);

		Student[] studentArr = { s1, s2, s3, s4, s5, s6, s7 };

		String[] myResult = Lab2.getClassList(studentArr);
		String[] expected = {  "abc", "def", "hij", "klm", "nop", "qrs", "tuv" };	

		System.out.println("Passed: " + Arrays.equals(myResult, expected));
		String testName = "testGetClasslist";

		displayResults(Arrays.equals(myResult, expected), testName);
		Student[] emptyArr = {};
		String[] emptyExpected = new String[ 0 ];
		System.out.println("Passed: " + Arrays.equals(Lab2.getClassList(emptyArr), emptyExpected));
	displayResults(Arrays.equals(myResult, expected), testName);
		// NOTE: the Arrays library has been imported for you.
		//       you can use the Arrays.equals method to compare
		//       2 arrays of String objects as String has a equals method
		// The API for Arrays.equals:
		//       equals(Object[] a, Object[] a2)
		//       Returns true if the two specified arrays are equal to one another.

		// TODO: once you have implemented getClasslist in Lab2.java
		// uncomment the following test. We have gotten you started with 
		// some initial test data and one test, but you should consider 
		// other cases (empty array, longer array)

		
		Student s0  = new Student("abc", 50);
		Student[] students1 = {s0};
		String[] expected1 = {"abc"};

		String[] result;

		result = Lab2.getClassList(students1);
		displayResults(Arrays.equals(result, expected1), "testGetClasslist - 1 elements");
		
	}


	public static void testCountAbove() {
		System.out.println("testCountAbove");

		// TODO: write tests for Lab2.countAbove
		Student s1 = new Student("", 10);
		Student s2 = new Student("", 56);
		Student s3 = new Student("", 97);
		Student s4 = new Student("", 75);
		Student s5 = new Student("", 80);

		Student[] studentAr = { s1, s2, s3, s4, s5, };

		int result = Lab2.countAbove(studentAr, 75);
		boolean expected = result == 2 ? true : false;
		System.out.println("Passed " + expected);
		String testName= "testCountAbove";

		result = Lab2.countAbove(studentAr, 0);
		expected = result == 5 ? true : false;
		System.out.println("Passed " + expected);
		displayResults(expected, testName);

		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);
		displayResults(expected, testName);


		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);
		


		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);


		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);


		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);


		result = Lab2.countAbove(studentAr, 100);
		expected = result == 0 ? true : false;
		System.out.println("Passed " + expected);


	}


	public static void testGetClassAverage() {
		Student s0 = new Student("abc", 50);
		Student s1 = new Student(); // considered invalid for average average calculation
		Student s2 = new Student("xyz", 99);
		Student s3 = new Student("def", 88);

		Student[] students0 = {};
		Student[] students1 = {s0};
		Student[] students2 = {s0, s1, s2};
		Student[] students3 = {s0, s2, s3};

		double result = 0.0;
		double expected = 0.0;
		
		// Some of you noticed in Lab1 that floating point arithmetic sometimes
		// produces results with many decimals places. We use the threshold
		// (defined as a global variable at the top) to specify the margin 
		// of error we are okay with, and just make sure the expected and 
		// returned results are within the threshold of acceptable error.

		
		result = Lab2.getClassAverage(students0);
		expected = 0;
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - empty");
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - empty");
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - empty");

		result = Lab2.getClassAverage(students1);
		expected = 50.0;
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - 1 student");
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - 1 student");
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - 1 student");

		result = Lab2.getClassAverage(students2);
		expected = (50 + 99) / 2.0; // because s1 does not have a "real" grade
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - 3 students, count 2");

		result = Lab2.getClassAverage(students3);
		expected = (50 + 99 + 88) / 3.0;
		displayResults(Math.abs(result-expected)<THRESHOLD, "testGetClasslist - 3 students, count 3");
        
	}

	public static void testRegisterStudent() {
		// TODO: write tests for Lab2.registerStudent
		// HINT: the Student class also has a equals method so you
		// can use Arrays.equals again to compare 2 Student arrays
		Student s1 = new Student("abc", 0);
		Student s2 = new Student("abc", 0);
		Student s3 = new Student("abc", 0);
		Student s4 = new Student("abc", 0);
		Student newS = new Student("", 0);
		String testName = "testRegisterStudent";
		Student[] studentList = { s1, s2, s3, s4 };
		Student[] result = Lab2.registerStudent(studentList, newS);
		boolean expected = result.length == 5;
		System.out.println("Passed: " + expected + "Actual: " + result.length);
		displayResults(expected, null);


		Student[] studentList2 = {};
		result = Lab2.registerStudent(studentList2, null);
		expected = result.length == 1;
		System.out.println("Passed: " + expected + "Actual: " + result.length);
		displayResults(expected, null);

		Student[] studentList3 = {};
		result = Lab2.registerStudent(studentList3, s1);
		expected = result.length == 1;
		System.out.println("Passed: " + expected + "Actual: " + result.length);
		displayResults(expected, null);


	}
	
	public static void displayResults (boolean passed, String testName) {
		/* There is some magic going on here getting the line number
		 * Borrowed from:
		 * http://blog.taragana.com/index.php/archive/core-java-how-to-get-java-source-code-line-number-file-name-in-code/
		 */
		
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
