// Name: Raul Rodriguez Castro
// Student number: v001030827Sign

import java.util.NoSuchElementException;
import java.lang.Math;
public class A3LinkedList implements A3List {
	private A3Node head;
	private A3Node tail;
	private int length;

	private void increaseCount() { 
		length++; 
	}
	private void decreaseCount() { 
		if(length > 0)
			length --; 
	}
	
	public A3LinkedList() {
		head = null;
		tail = null;
		length = 0;
	}
	
	public void addFront(String s) {
		// TODO: implement this
	
			
		if(head != null) { //handles existing head
			A3Node prevHead = head;
			A3Node newHead = new A3Node(s);
			newHead.setNext(prevHead);
			prevHead.setPrev(newHead);

			head = newHead;


		} else { //handle new head
			
			head = new A3Node(s);
			tail = head;
			
		}

		increaseCount();


		
	}

	public void addBack(String s) {
		// TODO: implement this
		if(head != null) { //handle existing list

			A3Node prevTail = tail;
			A3Node newTail = new A3Node(s);
			prevTail.setNext(newTail);
			newTail.setPrev(prevTail);
			tail = newTail;
		} else { //handle new list
			head = new A3Node(s);
			tail = head;
		}
		increaseCount();
	}
	
	public int size() {
		return length;
	}
	
	public boolean isEmpty() {
		return length==0;
	}
	
	public void removeFront() {
		// TODO: implement this
		if(head != null) { //handle non-empty list
			A3Node newHead = head.getNext();
			head = newHead;
			decreaseCount();
		} else { //handle empty list
			throw new NoSuchElementException("Cannot remove from an empty linked list");
		}
	}
	
	public void removeBack() {
		// TODO: implement this
		if(head != null) { //handle non-empty list
			A3Node newTail = tail.getPrev();
			newTail.setNext(null);
			tail = newTail;
		} else { //handle empty list
			throw new NoSuchElementException("Cannot remove from an empty linked list");
		}
	}
	
	public void insertInto(String s) {

		if(head == null) { //handle new head
			A3Node head = new A3Node(s);
			tail = head;
			return;
		}

		A3Node current = head;
		while(current != null) {
			if(current.getNext() != null){
				char[] next = current.getNext().getData().toCharArray();
				char[] curr = current.getData().toCharArray();
				char[] input = s.toCharArray();
				int index = Math.min(
					curr.length,
					Math.min(
					next.length,
					input.length
				));
				for(int i = 0; i < index; i++ ) {
					if(curr[i] <= input[i] && next[i] > input[i]) {
						A3Node temp = current.getNext();

						A3Node newNode = new A3Node(s);

						current.setNext(newNode);
						newNode.setPrev(current);

						newNode.setNext(temp);
						temp.setPrev(newNode);
						return;
					}
				}
				current = current.getNext();
			} else {
				A3Node newNode = new A3Node(s);
				current.setNext(newNode);
				newNode.setPrev(current);
				current = null;
			}
			
			
		}

		
				

		
	}

	public void rotate(int n) {
		// TODO: implement this

		//handle edge cases
		if(head == null || n == 0 || n == length) return;

		
	

		
	}
	
	/*
	 * Purpose: return a string representation of the list 
	 *          when traversed from front to back
	 * Parameters: none
	 * Returns: nothing
	 *
	 * USED TO HELP YOU WITH DEBUGGING
	 * DO NOT CHANGE THIS METHOD
	 */
	public String frontToBack() {
		String result = "{";
		A3Node cur = head;
		while (cur != null) {
			result += cur.getData();
			cur = cur.next;
		}
		result += "}";
		return result;
	}
	
	/*
	 * Purpose: return a string representation of the list 
	 *          when traversed from back to front
	 * Parameters: none
	 * Returns: nothing
	 *
	 * USED TO HELP YOU WITH DEBUGGING
	 * DO NOT CHANGE THIS METHOD
	 */
	public String backToFront() {
		String result = "{";
		A3Node cur = tail;
		while (cur != null) {
			result += cur.getData();
			cur = cur.prev;
		}
		result += "}";
		return result;
	}
}
	