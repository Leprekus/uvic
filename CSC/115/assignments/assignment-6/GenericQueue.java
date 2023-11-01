public class GenericQueue<T> implements Queue<T> {

	private Node<T> front;
	private Node<T> back;
	// no numElements for this implementation
	private void handleEmptyException(String m) throws EmptyQueueException{
		if(isEmpty()) throw new EmptyQueueException(m);
	}
	public GenericQueue() {
		front = null;
		back = null;
	}

	public int size() {
		int count = 0;
		Node<T> cur = front;
		
		while (cur != null) {
			count++;
			cur = cur.getNext();
		}
		return count;			
	}

	public boolean isEmpty() {
		return front == null;
	}

	public void enqueue (T value) {
		Node<T> n = new Node<T>(value);
		if (front == null) {
			front = n;
		} else {
			back.setNext(n);
		}
		back = n;
	}

	public T dequeue() throws EmptyQueueException {
		handleEmptyException("Cannot call dequeue on empty queue");
		T val = front.getValue();
		front = front.getNext();
		if (front == null) {
			back = null;
		}
		return val;
	}

	public T peek() throws EmptyQueueException  {
		handleEmptyException("Cannot call peek on empty queue.");
		return front.getValue();
	}


	public void dequeueAll() {
		front = null;
		back = null;
	}
}