public class QueueRefBased implements Queue {

	private QueueNode front;
	private QueueNode back;
	// no numElements for this implementation

	public QueueRefBased() {
		front = null;
		back = null;
	}

	public int size() {
		int count = 0;		
		return count;			
	}

	public boolean isEmpty() {
		return false;
	}

	public void enqueue (int value) {
		
	}

	public int dequeue() {
		return -1; // so it compiles
	}

	public int peek()  {
		return -1; // so it compiles
	}

	public void makeEmpty() {

	}
}
