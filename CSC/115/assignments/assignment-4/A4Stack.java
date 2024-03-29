public class A4Stack<T> implements _Stack<T> {
	private A4Node<T> head;

	public A4Stack() {
		head = null;
	}

	/* returns last node in the stack */
	private A4Node<T>traverse() {

		if(head == null) return head;

		A4Node<T> current = head;
		while(current.next != null) {
			current = current.next; 
		}
		return current;
	}
	/*
	 * Purpose: insert an item onto the top of the stack
	 * Parameters: T value - the value to insert
	 * Returns: void - Nothing
	 */
	public void push(T value) {
		A4Node<T> newTail = new A4Node<T>(value);
		if(head == null) {
			head = newTail;
			return;
		};
		A4Node<T> current = head;
		while(current.next != null) {
			current = current.next;
		}
		System.out.println("current " + current.getData() + " next: " + current.getNext());	
		current.setNext(newTail);
		
	}; 
/*
	 * Purpose: removes and returns the top item from the stack
	 * Parameters: None
	 * Returns: T - the data value of the element removed
	 *              or null if the stack is empty
	 */
	public T pop() { 
		if(head == null) return null;

		if(head.next == null) {
			A4Node<T> temp = head;
			head = null;
			return temp.getData();
		}

		A4Node<T> current = head;
	
		while(current.next.next != null) {
			current = current.next;
			
		}
		A4Node<T> popped = current.next;
		current.next = null;
		return popped.getData();
		
	 };

	/*
	 * Purpose: determines whether the stack is empty
	 * Parameters: None
	 * Returns: boolean - true if the stack is empty, false otherwise
	 */
	public boolean isEmpty(){ return head == null; };
/*
	 * Purpose: get the top item on the stack
	 * Parameters: None
	 * Returns: T - the data value of the top element
	 *              or null if the stack is empty
	 */
	public T top(){ 
		if(head == null) return null;
		A4Node<T> tail = traverse();
		return tail.getData(); };

	/*
	 * Purpose: removes all elements from the stack
	 * Parameters: None
	 * Returns: void - nothing
	 */
	public void popAll(){ head = null; };

	public String toString() {
		A4Node<T> current = head;
		String result = "{";
		while(current != null) {
			result += current.getData() + ", ";
			current = current.next;
		}
		result += "}";
		return result;
	}
	
}