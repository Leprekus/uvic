public class QueueRefBased implements _Queue {

	private QueueNode front;
	private QueueNode back;
	// no numElements for this implementation
	private int count = 0;

	public QueueRefBased() {
		front = null;
		back = null;
	}

	public int size() {
		return count;
	}
	private int increaseSize() {
		return count++;
	}

	private int decreaseSize() {
		count = count-- < 0 ? 0 : count--;
		return count;
	}

	public boolean isEmpty() {
		System.out.println("size " + size());
		return size() == 0 ? true : false;
	}

	public void enqueue (int value) { //same as push
		increaseSize();
		QueueNode node = new QueueNode(value);
		if(front == null) { //handle empty queue
			front = node;
			back = node;
			return;
		}
		QueueNode temp = back;
		temp.next = node;
		back = node;

		
	}

	public int dequeue() { //same as unshift
		decreaseSize();
		if(front == null) return -1;
		QueueNode temp = front;
		front = front.next;
		return temp.data; // so it compiles
	}

	public int peek()  {

		return front == null ? -1 : front.data; // so it compiles
	}

	public void makeEmpty() {
		count = 0;
		front = null;
		back = null;

	}
}
