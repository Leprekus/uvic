public class A5Exercises {
	
	// PART 1

	/*
	 * Purpose: get the sum of the even values in the array
	 * Parameters: int[] arr
	 * Returns: int - the sum number of even-valued elements
	 * Post-condition - the array contents are unchanged
	 */
	public static int sumEven(int[] arr) {
		// TODO: implement this
		int sum = 0;
		for(int n: arr) {
			if(n % 2 == 0) sum += n;
		}

		return sum; // so it compiles
	}
	
	/*
	 * Purpose: add x to all values in the given array
	 * Parameters: int[] array - the array to modify
	 *             int x - the value to add to all elements
	 * Returns: void - nothing
	 */
	public static void addXToAll(int[] array, int x) {
		
		for(int i = 0; i < array.length; i++) {
			int item = array[i] + x;
			array[i] = item;
		}

	}
	
	/*
	 * Purpose: get the maximum value found in the array
	 * Parameters: int[] array - the array to search
	 * Returns: int - maximum value found in the array
	 *                or -1 if the array is empty
	 * Post-condition - the array contents are unchanged
	 */
	public static int getMaximum(int[] array) {
		// TODO: implement this
		if(array.length == 0) return -1;
		if(array.length == 1) return array[0];

		int max = array[0];
		for(int n: array) {
			if( n > max ) max = n;
		}
		return max; // so it compiles
	}
	
	/*
	 * Purpose: determines if there is at least one occurrence of x
	 *          after the occurrence of y in the array
	 * Parameters: int[] array - the array to search
	 *             int x - the value that must come second
	 *             int y - the value that must come first
	 * Returns: boolean - true if there is at least one occurrence
	 *                    of x after the occurrence of y
	 */
	private static String stringify(int[] array) {
		String result = "{ ";
		for(int n : array) { result += "" + n  + ", ";}
		result += "}";
		return result;
	}
	public static boolean comesAfter(int[] array, int x, int y) {
		// TODO: implement this
		if(array.length < 2) return false;

		boolean isFirstSeen = false;
		int first = y;

		boolean isSecondSeen = false;
		int second = x;
		
		for(int i = 0; i < array.length; i++) {
			int current = array[i];

			if(current == first) { isFirstSeen = true; }
			if(current == second) { isSecondSeen = true; }

			if(isSecondSeen && !isFirstSeen){ return false; }
		}
	
		if(isFirstSeen && isSecondSeen){ return true; }
		return false; // so it compiles
	}
	


	/////////////
	// PART II //
	/////////////
	
	/*
	 * Purpose: get the total number of beads in s
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: int - the total number of beads
	 * Post-condition: s is not modified
	 */

	public static int beadsCount(Stack<BeadBox> s) {
		// TODO: implement this
		BeadBox current = s.pop();
		int sum = 0;
		while(current != null) {
			sum += current.getNumberBeads();
			current = s.pop();

		}

		return sum; // so it compiles
	}

	/*
	 * Purpose: get the total number of bead boxes in s
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: int - the total number of bead boxes
	 * Post-condition: s is not modified
	 */
	public static int beadBoxesCount(Stack<BeadBox> s) {
		// TODO: implement this
		return -1; // so it compiles
	}
	
	/*
	 * Purpose: get the total weight of all bead boxes
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: double - the total weight of all boxes
	 * Post-condition: s is not modified
	 */
	public static double totalWeight(Stack<BeadBox> s) {
		// TODO: implement this
		return -1.0; // so it compiles
	}

	/*
	 * Purpose: get the average weight of the bead boxes in s
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: double - the average weight of the bead boxes
	 *                   0.0 if there are no bead boxes in s
	 * Post-condition: s is not modified
	 */
	public static double averageWeight(Stack<BeadBox> s) {
		// TODO: implement this
		return -1.0; // so it compiles
	}

	/*
	 * Purpose: determine the bead boxes in s are stacked correctly
	 *          (ie. there is never a bead box stacked on top of 
	 *               another bead box that weighs LESS than it does)
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: boolean - true if bead boxes in s are stacked correctly
	 * Post-condition: s is not modified
	 */
	public static boolean stackedCorrectly(Stack<BeadBox> s) {
		// TODO: implement this
		return false; // so it compiles
	}

}