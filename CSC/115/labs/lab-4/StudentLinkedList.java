public class StudentLinkedList implements StudentList {

	StudentNode head;
	StudentNode tail;
	int count;

	public StudentLinkedList() {
		head = null;
		tail = null;
		count = 0;
	}
	private void decreaseCount() {
		if(count == 0) return ;
		else count --;
	}

	public void add(Student s) {
		count++;
		// TODO: complete
		if(head == null) {
			head = new StudentNode(s, null);
			tail = head;
			return ;
		};

		StudentNode newTail = new StudentNode(s, null);

		if(tail != null) {
			tail.setNext(newTail);
		}
		
		tail = newTail;
	}

	public int size() {
		// TODO: complete
		return count;
	}

	public void removeFront() {
		// TODO: complete
		decreaseCount();
		if(head == null) {
			return ;
		}
		
		if(head.getNext() == null) { //handle one element
			head = null;
			tail = null;
			return ;
		}
		head = head.getNext();
		

	}

	public boolean contains(Student s) {

		if(head == null) return false;

		StudentNode current = head;

		while(current != null) {

			Student data = current.getData();
			if(data.equals(s)) return true;
			current = current.next;
		}
		return false;
	}
	
	/*
	 * Purpose: returns a String reprensentation of elements
	 *      in this list separated by newlines
	 * Parameters: none
	 * Returns: String - the representation
	 */
	public String toString() {
		// DO NOT CHANGE THIS METHOD - it is correct
		// Changing it will result in your code getting
		// a score of 0 from the autograder for this lab
		
		String s = "";
		StudentNode tmp = head;

		while(tmp != null) {
			s  += tmp.getData() + "\n";
			tmp = tmp.next;
		}

		return s;
	}
}
