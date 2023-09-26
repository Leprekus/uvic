public class ShapeList{
    
	private static final int DEFAULT_SIZE = 2;

	private Shape[] elements;
	private int count;

	public ShapeList() {
		elements = new Shape[DEFAULT_SIZE];
		count = 0;
	}

	/*
	 * Purpose: returns the number of elements in list
	 * Parameters: none
	 * Returns: int - the number of elements
	 */
	private void increaseCount() {
		count += 1;
	}
	private void decreaseCount() {
		if(count - 1 >= 0) count -= 1;
		else count = 0; 
	}
	public int size() {
		return count;
	}

	/*
	 * Purpose: adds Shape s to back of this list
	 * Parameters: Shape - s
	 * Returns: nothing
	 *
	 * IMPLEMENTATION NOTE: if elements is full, a new
	 * array should be created to hold all of the original
	 * elements and also shape s.
	 */
	public void add(Shape s) {

		// handle full
		if(size() == elements.length) {

			Shape[] temp = new Shape[ elements.length + 1 ];

			for(int i = 0; i < elements.length; i++) {
				Shape item = elements[i];
				temp[i] = item;
			}
			temp[temp.length - 1] = s;
			elements = temp;

		} else {
			elements[size()] = s;
		}
		
		increaseCount();
		
	
	}

	/*
	 * Purpose: returns a String reprensentation of the elements
	 *      in this list separated by newlines
	 * Parameters: none
	 * Returns: String - the representation
	 */
	public String toString() {
		String s = "";

		for(int i=0; i<count; i++) {
			s  += elements[i] + "\n";
		}

		return s;
	}

	/*
	 * Purpose: removes the first element in this list
	 * Parameters: none
	 * Returns: nothing
	 *
	 * IMPLEMENTATION NOTE: After removing the first element,
	 * all other elements should be shuffled forward 
	 * so that there are no gaps in the array.
	 */
	public void removeFront() {
		// You are not required to implement this method for lab 2
		// It is here as an additional exercise for those of you
		// who would like a little more practice working with 
		// arrays of objects in Java.


		for(int i = 1; i < elements.length; i++) {
			Shape item = elements[i];
			elements[i - 1] = item;
			
			//removes turns i to null
			//to avoid i -1 and i duplicates
			if(i == elements.length - 1) {
				elements[i] = null;
			}
		}
		decreaseCount();
	}
    
    
}
