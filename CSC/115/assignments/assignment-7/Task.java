/*
 * represents a task in a to-do list
 *
 * we have implemented some methods and constructors.
 * we have left the documentation for you to follow
 * to implement the compareTo and equals methods
 */

public class Task implements Comparable<Task>{

	private int priorityLevel; //task priorityLevel 1 is highest, 5 is lowest.
	private String title;
	private Time insertionTime;

	public Task(int priorityLevel, String title, Time insertionTime) {
		this.priorityLevel = priorityLevel;
		this.title = title;
		this.insertionTime = insertionTime;
	}

	public int getPriorityLevel() {
		return priorityLevel;
	}

	public void setPriorityLevel (int priorityLevel) {
		this.priorityLevel = priorityLevel;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle (String name) {
		this.title = name;
	}

	public Time getTime() {
		return insertionTime;
	}

	public void setTime (Time checkinTime) {
		this.insertionTime = checkinTime;
	}

	public String toString() {
		return title;
	}

	/* Method Name: compareTo
	 * Purpose: returns the result of comparing this priorityLevel and
	 *          other priorityLevel if they are not equal
 	 *	        if priorityLevels are equal, it returns the result of 
	 *          comparing this checkinTime to other checkinTime
	 * Parameters: Task other
	 * Precondition: other is not null
	 * Returns: a value < 0 if this Task is before other Task
	 *          a value == 0 if this Task is the same as other Task
	 *          a value > 0 if this Task is after other Task
	 * HINT: the Time class implements comparable too!
	 */
	public int compareTo(Task other) {
		// TODO: implement this
		return Integer.compare(this.priorityLevel, other.priorityLevel);

	}

	/* Method Name: equals
	 * Purpose: determines whether the title of this Task
	 *          is the same as the other Task's title
	 * Parameters: Task other
	 * Precondition: other is not null
	 * Returns: true if this Task is the same as other, false otherwise
	 */
	public boolean equals(Task other) {
		// TODO: implement this
		return this.title.equals(other.title); // so it compiles;
	}
}

