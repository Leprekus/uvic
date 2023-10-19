interface Stack<T> {
	
	/*
	 * Purpose: insert an item onto the top of the stack
	 * Parameters: T value - the value of the item to insert 
	 * Returns: void - nothing
	 */
	public void push(T value); 
	
	/*
	 * Purpose: removes and returns the top item from the stack
	 * Parameters: None
	 * Returns: int - the data value of the element removed
	 */
	public T pop();
	
	/*
	 * Purpose: accesses the top item on the stack
	 * Parameters: None
	 * Returns: int - the data value of the top element
	 */
	public T top();
	
	/*
	 * Purpose: determines whether the stack is empty
	 * Parameters: None
	 * Returns: boolean - true if the stack is empty, false otherwise
	 */
	public boolean isEmpty();
	
	/*
	 * Purpose: Accesses the top item on the stack
	 * Parameters: None
	 * Returns: void - nothing
	 */
	public void popAll();

}