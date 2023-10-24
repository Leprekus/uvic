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

	private static void calculateBeadsCount(A5Stack<BeadBox> s, int[] sum) {
		if(s.isEmpty()) return ;
		BeadBox current = s.pop();
		sum[0] += current.getNumberBeads();
		calculateBeadsCount(s, sum);
		s.push(current);
	}
	public static int beadsCount(A5Stack<BeadBox> s) {
		// TODO: implement this
		
		int[] sum = new int[] { 0 };
		calculateBeadsCount(s, sum);
		System.out.println("result " + sum[0]);
		return sum[0]; // so it compiles
	}

	/*
	 * Purpose: get the total number of bead boxes in s
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: int - the total number of bead boxes
	 * Post-condition: s is not modified
	 */
	private static void calculateBeadBoxesCount(A5Stack<BeadBox> s, int[] sum) {
		if(s.isEmpty()) {
			return;
		}
		BeadBox current = s.pop();
		sum[0] += 1;
		calculateBeadBoxesCount(s, sum);
		s.push(current);
	}
	public static int beadBoxesCount(A5Stack<BeadBox> s) {
		
		if(s == null) return 0;

		int[] sum = { 0 };
		calculateBeadBoxesCount(s, sum);
		return sum[0]; // so it compiles
	}
	
	/*
	 * Purpose: get the total weight of all bead boxes
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: double - the total weight of all boxes
	 * Post-condition: s is not modified
	 */

	private static void calculateTotalWeight(A5Stack<BeadBox> s, double[] sum) {
		if(s.isEmpty()) return ;
		BeadBox current = s.pop();
		sum[0] += current.boxWeight();
		calculateTotalWeight(s, sum);
		s.push(current);
	}
	public static double totalWeight(A5Stack<BeadBox> s) {
		if(s == null) return 0.0;

		double[] sum = { 0.0 };
		calculateTotalWeight(s, sum);

		return sum[0]; // so it compiles
	}

	/*
	 * Purpose: get the average weight of the bead boxes in s
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: double - the average weight of the bead boxes
	 *                   0.0 if there are no bead boxes in s
	 * Post-condition: s is not modified
	 */
	private static void calculateAverageWeight(A5Stack<BeadBox> s, double[] sum, int index) {
		if(s.isEmpty()){
			sum[0] = sum[0] / (index);
			return ;
			}
		BeadBox current = s.pop();
		sum[0] += current.boxWeight();
		calculateAverageWeight(s, sum, ++index);
		s.push(current);
	}
	public static double averageWeight(A5Stack<BeadBox> s) {
		if(s == null) return 0;

		if(s.isEmpty()) return 0;

		double[] sum = { 0.0 };
		calculateAverageWeight(s, sum, 0);

		return sum[0]; // so it compiles
	}

	/*
	 * Purpose: determine the bead boxes in s are stacked correctly
	 *          (ie. there is never a bead box stacked on top of 
	 *               another bead box that weighs LESS than it does)
	 * Parameters: Stack<BeadBox> s - the stack of bead boxes
	 * Returns: boolean - true if bead boxes in s are stacked correctly
	 * Post-condition: s is not modified
	 */
	private static void calculateStackedCorrectly(A5Stack<BeadBox> s, boolean[] isCorrect, BeadBox prev) {
		if(s.isEmpty()){
			//check if value has not been previously mutated
			if(isCorrect[1] == false) {
				isCorrect[0] = true;
				isCorrect[1] = true;
				}
				return ;
			}
		BeadBox current = s.pop();
		if(prev != null) {
			//if(prev.boxWeight() < current.boxWeight()) this is ok
			if(prev.boxWeight() > current.boxWeight()) {
				//check if value has not been previously mutated
				if(isCorrect[1] == false) {
					isCorrect[0] = false;
					isCorrect[1] = true;
				}

			} else { 
				//check if value has not been previously mutated
				if(isCorrect[1] == false) {
					isCorrect[0] = true;
					isCorrect[1] = true;
				}
			}
		}

		calculateStackedCorrectly(s, isCorrect, current);
		s.push(current);
	}	
	public static boolean stackedCorrectly(A5Stack<BeadBox> s) {
		
		if(s == null) return false;
		//[0] is result [1] indicates whether result is mutated
		//if [1] = true, then we shouldn't mutate [0] again
		boolean[] isCorrect = { false, false };
		calculateStackedCorrectly(s, isCorrect, null);
		return isCorrect[0]; // so it compiles
	}

}