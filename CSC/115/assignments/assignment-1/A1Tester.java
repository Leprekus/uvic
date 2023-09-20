
public class A1Tester {
	/*
	 * A1Tester
	 * A class to test the methods in Assignment 1
	 * 
	 // Name: RAUL RODRIGUEZ CASTRO 
	 // Student number: V01030827
	 */
    
	private static int testPassCount = 0;
	private static int testCount = 0;

	public static void main(String[] args) {

		// PART 1 - variables, methods, ifs, and loops

		//TODO: uncomment testIsFactor,
		// testCalcPower, testNumFactors, testIsPrime
		//testGreatestCommonDenominator
		//testSumArray
		//testcountmat

		/* I have correctly implemented the first two methods for you. 
		 * Read through the tests and look at the implementation
		 * in the A1Exercises.java file to get an idea of what is expected
		 * for the other parts of the assignment you will complete yourself
		 */
		testIsFactor();
		testCalcPower();

		/* I have also provided you with some tests for numFactors */
		testNumFactors();


		/* Uncomment the method call below when you have finished numFactors */
		testIsPrime();


		/* Uncomment the method call below when you have finished isPrime
		 * You will need to write tests for this method 
		 */
		testGreatestCommonDenominator();



		// PART 2 - arrays

		/* Some examples to demonstrate how to intialize and test arrays. 
		 * The printArrays method have been completed for you.
		 */
		 printArrayExample();
		 testSumArray();

		 testContains();
		 testCountMatches();

	}
	
	public static void testIsFactor() {
		boolean result = false;
		boolean expected = false;
		
		System.out.println("Testing isFactor(3,3)");
		result = A1Exercises.isFactor(3, 3);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing isFactor(3,2)");
		result = A1Exercises.isFactor(3, 2);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing isFactor(8,2)");
		result = A1Exercises.isFactor(8, 2);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing isFactor(42,5)");
		result = A1Exercises.isFactor(42, 5);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing isFactor(102,17)");
		result = A1Exercises.isFactor(102, 17);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
	}
	
	public static void testCalcPower() {
		int result = 0;
		int expected = 0;
		
		System.out.println("Testing calcPower(2,0)");
		result = A1Exercises.calcPower(2, 0);
		expected = 1;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing calcPower(2,3)");
		result = A1Exercises.calcPower(2, 3);
		expected = 2*2*2;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing calcPower(5,4)");
		result = A1Exercises.calcPower(5, 4);
		expected = 5*5*5*5;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing calcPower(13,2)");
		result = A1Exercises.calcPower(13, 2);
		expected = 13*13;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
	}
	
	public static void testNumFactors() {
		int result = 0;
		int expected = 0;
		
		System.out.println("Testing numFactors(1)");
		result = A1Exercises.numFactors(1);
		expected = 1;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing numFactors(6)");
		result = A1Exercises.numFactors(6);
		expected = 4; // There are 4 factors of 6: 1, 2, 3, and 6
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing numFactors(24)");
		result = A1Exercises.numFactors(24);
		expected = 8; // There are 8 factors of 24: 1,2,3,4,6,8,12,24
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing numFactors(51)");
		result = A1Exercises.numFactors(51);
		expected = 4; // There are 4 factors of 51: 1,3,17,51
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
	}
	
	public static void testIsPrime() {
		boolean result = false;
		boolean expected = false;

		System.out.println("Testing isPrime(2)");
		result = A1Exercises.isPrime(2);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing isPrime(6)");
		result = A1Exercises.isPrime(6);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing isPrime(11)");
		result = A1Exercises.isPrime(11);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing isPrime(15)");
		result = A1Exercises.isPrime(15);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing isPrime(1046527)");
		result = A1Exercises.isPrime(1046527);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing isPrime(1046529)");
		result = A1Exercises.isPrime(1046529);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

	}


	
	public static void testGreatestCommonDenominator() {
		int result = A1Exercises.greatestCommonDenominator(0, 0);
		int expected = 0;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);


		result = A1Exercises.greatestCommonDenominator(1235145, 6346645);
		expected = 5;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);


		result = A1Exercises.greatestCommonDenominator(432, 9123);
		expected = 3;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		result = A1Exercises.greatestCommonDenominator(125, 234545);
		expected = 5;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		result = A1Exercises.greatestCommonDenominator(7345, 9234);
		expected = 1;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		result = A1Exercises.greatestCommonDenominator(2958, 238);
		expected = 34;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		result = A1Exercises.greatestCommonDenominator(2468, 2468);
		expected = 2468;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		// Write tests here. Format of tests shown below:
/*
		result = A1Exercises.greatestCommonDenominator(1, 1);
		expected = 1;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
*/
		
	}
	
	public static void printArrayExample() {
		int[] array0 = {};
		int[] array1 = {8};
		int[] array2 = {2, 3, 2, 0};
		int[] array3 = {-1, 2, 4, 1, 3};
		
		A1Exercises.printArray(array0);
		A1Exercises.printArray(array1);
		A1Exercises.printArray(array2);
		A1Exercises.printArray(array3);
	}
	
	public static void testSumArray() {
		int result = 0;
		int expected = 0;
		int[] array0 = {};
		int[] array1 = {8};
		int[] array2 = {2, 3, 2, 0};
		int[] array3 = {-1, 2, 4, 1, 3};

		System.out.println("Testing sumArray with an empty array");
		result = A1Exercises.sumArray(array0);
		expected = 0;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);
		
		System.out.println("Testing sumArray with {8}");
		result = A1Exercises.sumArray(array1);
		expected = 8;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing sumArray with {2, 3, 2, 0}");		
		result = A1Exercises.sumArray(array2);
		expected = 2+3+2;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing sumArray with {-1, 2, 4, 1, 3}");
		result = A1Exercises.sumArray(array3);
		expected = -1+2+4+1+3;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

	}	
    
	public static void testContains() {
		boolean result = false;
		boolean expected = false;
		int[] arr0 = {};
		int[] arr1 = {2, 3, 2, 0};
		int[] arr2 = {-1, 2, 4, 1, 3};

		System.out.println("Testing contains with an empty array");
		result = A1Exercises.contains(arr0, 2);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr1, 2)");
		result = A1Exercises.contains(arr1, 2);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr1, 0)");
		result = A1Exercises.contains(arr1, 0);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr1, 4)");
		result = A1Exercises.contains(arr1, 4);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr2, 4)");
		result = A1Exercises.contains(arr2, 4);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr2, 3)");
		result = A1Exercises.contains(arr2, 3);
		expected = true;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		System.out.println("Testing contains(arr2, 5)");
		result = A1Exercises.contains(arr2, 5);
		expected = false;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

	}
	
	public static void testCountMatches() {
		int result = 0;
		int expected = 0;
		int[] arr1 = {2, 3, 2, 0};
		// add more arrays

		System.out.println("Testing countMatches(arr1, 1)");
		
		int[] arr2 = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 };
		result = A1Exercises.countMatches(arr2, 69);
		expected = 1;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		int[] arr3 = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 420, 81, 82, 83, 84, 420, 86, 87, 420, 420, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 };	
		result = A1Exercises.countMatches(arr3, 420);
		expected = 4;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);

		int[] arr4 = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 666, 84, 85, 86, 87, 666, 89, 90, 666, 92, 93, 94, 95, 96, 97, 98, 99 };	
		result = A1Exercises.countMatches(arr4, 666);
		expected = 3;
		System.out.println("Expected: "+expected);
		System.out.println("Returned: "+result);



	}  
}
