public class A1Exercises {
	// Name: RAUL RODRIGUEZ CASTRO 
	// Student number: V01030827
	
	/*
	 * A1Exercises
	 *
	 * Rules: 
	 *  - DO NOT use builtin java Arrays methods
	 */
	
	/*
	 * isFactor
	 * Purpose: Determine if x can be evenly divided by y
	 * Parameters int x, int y
	 * Returns: true if x can be evenly divided by y, false otherwise
	 * Precondition: y > 0
	 */
	public static boolean isFactor(int x, int y) {
		if (x % y == 0) {
			return true;
		} else {
			return false;
		}
	}	
	
	/*
	 * calcPower
	 * Purpose: Calculate the value of base^exp
	 * Parameters: int base - the base value
	 *             int exp - the exponent value
	 * Returns: int - the value of base^exp
	 * Pre-conditions: base > 0, exp >= 0
	 */
	public static int calcPower(int base, int exp) {
		int result = 1;
		for (int i = 0; i < exp; i++) {
			result *= base;
		}
		return result;
	}
	
	/*
	 * numFactors
	 * Purpose: Determine the number of factors of n
	 * Parameters: int n
	 * Returns: int - the number of factors of n
	 * Pre-conditions: n > 0
	 */
	public static int numFactors(int n) {
	
		// TODO: add code to finish the method implementation
		if(n <= 0) return n;

		int factorCount = 0;
		int i = n;
		while(i > 0) {
			if(n % i == 0) {
				factorCount += 1;
			}
			i--;
		}
		
		return factorCount;
	}
	
	/*
	 * isPrime
	 * Purpose: determine if n is a prime number
	 * Paramaters: int n
	 * Returns: boolean - true if n is prime, false otherwise
	 * Pre-conditions: n > 1
	 *
	 * Note: A prime number is a whole number greater 
	 * than 1 whose only factors are 1 and itself
	 *
	 * HINT: This method can be completed in 1 line.
	 */
	// TODO: write the isPrime method here
	public static boolean isPrime(int n) {

		int i = n - 1;
		while(i > 0) {

			if(i != 1 && n % i == 0) {
		
				return false;
			}
			i--;
		}
		return true;
	}
	
	/*
	 * greatestCommonDenominator
	 * Purpose: determine the largest integer that divides x and y
	 * Parameters: int x, int y
	 * Returns: int the largest integer that divides x and y
	 * Pre-conditions: x, y > 0
	 */
	// TODO: write the greatestCommonDenominator method here
	private static boolean  isInt(String data) {
		try {

			Integer.parseInt(data);
			return true;
		} catch(NumberFormatException e) {

			return false;

		}
	}

	public static int greatestCommonDenominator(int n1, int n2) {
		String num_arr = "";
		int len = 0;

		int smaller_num = n1 < n2 ? n1 : n2;
		int bigger_num = n1 > n2 ? n1 : n2;

		for(int i = smaller_num; i > 0; i--) {
			if(smaller_num % i == 0 && bigger_num % i == 0) {
				num_arr += Integer.toString(i) + " ";
			}
		}
		
		int max_num = 0;
		String[] parts = num_arr.split(" ");
		for(String part: parts){
			int current = isInt(part) ? Integer.parseInt(part) : 0;
			if(current > max_num) {
				max_num = current;
			}
		}
		
	
		return max_num;

	}
	
	/*
	 * printArray
	 * Purpose: prints all values in the array to the console
	 *          Example format:  {1,2,3,4}
	 * Parameters: int[] - an array of integers
	 * Returns: void - nothing
	 */
	public static void printArray (int[] array) {
		System.out.print("{");
		for(int i=0; i<array.length; i++) {
			System.out.print(array[i]);
			if(i<array.length-1) {
				System.out.print(",");
			}	
		}
		System.out.println("}");
	}
	
	/*
	 * sumArray
  	 * Purpose: computes the sum of all values in the given array
 	 * Parameters: int[] array - the array of integers
	 * Returns: int - sum of all values in the array
	 * Pre-conditions: the array is not null (but could be empty)
	 */
	// TODO: write the sumArray method here
	public static int sumArray(int[] numArr) {
		
		int sum = 0;
		for (int n: numArr) {

			sum += n;
		}

		return sum;
	}
	
	/*
	 * contains
	 * Purpose: determines whether the array contains x
	 * Parameters: int[] array - array of integers to search through
	 *             int x - the value to search for
	 * Returns: boolean - true if value is found, false otherwise
	 * Pre-conditions: the array is not null (but could be empty)
	 */
	// TODO: write the contains method here
	public static boolean contains(int[] numArr, int target) {
		for(int num: numArr) {
			if(num == target) return true;
		}
		return false;
	}
	/*
	 * countMatches
	 * Purpose: Determines the number of occurences of x in the given array
	 * Parameters: int[] array - array of integers to search through
	 *             int x - the value to search for
	 * Returns: int - the number of times x is found in the array
	 * Pre-conditions: the array is not null (but could be empty)
	 */
	// TODO: write the countMatches method here */
	public static int countMatches(int[] numArr, int target) {
		int count = 0;
		for(int num: numArr) {
			if(num == target) count ++;
		}

		return count;
	}
}