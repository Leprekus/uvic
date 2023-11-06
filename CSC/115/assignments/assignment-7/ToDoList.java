/*
 * represents a to-do list of a work day
 */

public class ToDoList {

	private PriorityQueue<Task> tasks;
	// DO NOT ADD ANY ADDITIONAL FIELDS. YOU SHOULD BE ABLE TO 
	// SOLVE ALL THE PROBLEMS BELOW WITH JUST THE FIELD PROVIDED.

	/* 
 	 * constructor that initializes a to-do list to a default-sized priority queue
	 */
	public ToDoList() {
		// DO NOT CHANGE THIS
		tasks = new HeapPriorityQueue<Task>(Task.class);
	}

	/* 
	 * constructor that initializes a to-do list to a priority queue of given size
	 */
	public ToDoList(int size) {
		// DO NOT CHANGE THIS
		tasks = new HeapPriorityQueue<Task>(Task.class, size);
	}


	/* Purpose: add given Task t to tasks
	 *          a notification message if to-do list is full
	 * Parameters: Task t - the Task that was just inserted to
	 *             the to-do list and needs to be added
	 * Returns: void - nothing
	 */
	public void addTask(Task t) {
		// TODO: implement this
	}


	/* Purpose: gets the number of tasks waiting to be done
	 * Parameters: none
	 * Returns: int - number of tasks waiting to be done
	 */
	public int numberTasksToDo() {
		// TODO: implement this
		return -1; // so it compiles
	}


	/* Purpose: removes and returns the next Task from tasks
	 * Parameters: none
	 * Returns: Task - the next Task, null if there is no more Tasks.
	 */
	public Task nextTask() {
		// TODO: implement this
		return null; // so it compiles
	}
}

