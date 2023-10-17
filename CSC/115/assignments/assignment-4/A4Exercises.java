public class A4Exercises {
	
	/*
	 * Purpose: determine if the stack of plates has been
	 *          stacked correctly (ie. there is never a plate
	 *          on top of a smaller plate)
	 * Parameters: Stack<Plate> s - a stack of plates
	 * Returns: boolean - true if the plates are stacked correctly
	 *                    false otherwise
	 * Post-condition: the contents of s are not modified
	 */
	
	
	public static boolean stackedCorrectly(Stack<Plate> s) {

		Plate top = s.pop();
		while(!s.isEmpty()) {
			Plate current = s.top();
			if(top.getDiameter() > current.getDiameter()) return false;
			top = current;
			s.pop();
		} return true;

	
	}
	
	/*
	 * Purpose: insert p into the correct location in the
	 *          stack such that there are no smaller plates 
	 *          below p and no larger plates above p
	 * Parameters: Stack<Plate> s - a stack of plates
	 *             Plate p - the plate to insert into s
	 * Returns: void - nothing
	 * Pre-condition: plates in s have been stacked correctly
	 */
	public static void insertPlate(Stack<Plate> s, Plate p) {
		Stack<Plate> temp = new A4Stack<Plate>();
		
	}
}