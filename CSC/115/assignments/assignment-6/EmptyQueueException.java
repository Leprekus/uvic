public class EmptyQueueException extends Exception{
    private String message;
	
	public EmptyQueueException (String m) {
		this.message = m;
	}

	public String toString(){
		return "StackEmptyException: " + message;
	}
}

