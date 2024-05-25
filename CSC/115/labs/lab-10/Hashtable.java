public class Hashtable{
    
private static final int TABLE_SZ = 7; // a prime number

	Student[] table;
	int count;  // number of Students in the table

	public Hashtable() {
		table = new Student[TABLE_SZ];
		count = 0;
	}

	private void handleCollisionException() throws CollisionException { throw new CollisionException(); }
	private void handleTableFullException() throws TableFullException { throw new TableFullException(); }
	private void handleKeyNotFoundException() throws KeyNotFoundException { throw new KeyNotFoundException(); }

	private int probe(Student s, Integer memo) {
		if(memo != null) return ++memo % table.length;
		return s.hashCode() % table.length;
	}
	private boolean SIDequals(Student local, Student other) {
		return local.getSID().equals(other.getSID());
	}

	/* MethodName: insertCollisions
	 * Purpose: insert (or update entry) s in this Hashtable with no collision handling strategy
	 * Parameters: Student - s
	 * Throws:  TableFullException - if inserting a new key into a full table
	 *          CollisionException - if inserting a new key into table at index that is full
	 * Returns: nothing
	 */
	// TODO: complete this function
	public void insertCollisions(Student s) throws CollisionException, TableFullException {
		
		int i = probe(s, null);
		
		if(table[i] != null && SIDequals(s, table[i])){
			table[i] = s;
			return;
		}
		if(count == table.length) handleTableFullException();

		else if(table[i] != null) handleCollisionException();

		table[i] = s;
		count++;
	}


	/* MethodName: getCollisions
	 * Purpose: find Student with sid in this Hashtable with no collision handling and returns their grade
	 * Parameters: String - sid
	 * Throws:  KeyNotFoundException  - if Student with sid is not found in table
	 * Returns: int - the grade of Student with sid
	 */
	// TODO: complete this function
	public int getCollisions(String sid) throws KeyNotFoundException {
		Student key = new Student(sid, 0);
		for(int i = 0; i < table.length; ++i)
			if(SIDequals(table[i], key)) return table[i].getGrade();
		
		throw new KeyNotFoundException();
	}


	/* MethodName: insertLinearProbing
	 * Purpose: insert (or update entry) s in this Hashtable with Linear Probing to handle collisions
	 * Parameters: Student - s
	 * Throws: TableFullException  - if inserting a new key into a full table
	 * Returns: nothing
	 */
	// TODO: complete this function
	public void insertLinearProbing(Student s) throws TableFullException {
		
		int i = probe(s, null);
		int counter = 0;
		while(counter <= table.length || table[i] != null){
			if(counter == table.length) handleTableFullException();

			if(table[i] == null) { //insert
				table[i] = s;
				return;
			}
			if(table[i] != null && SIDequals(table[i], s)) { //update
				table[i] = s;
				return;
			}

			
			i = probe(s, i);
			System.out.println("hash " + i );
			counter++;
		}
		if(count == table.length) handleTableFullException();

		table[i] = s;
		count++;
		
	}

	



	/* getLinearProbing
	 * Purpose: find Student with sid in this Hashtable that uses Linear Probing and returns their grade
	 * Parameters: String - sid
	 * Throws:  KeyNotFoundException  - if Student with sid is not found in table
	 * Returns: int - the grade of Student with sid
	 */
	// TODO: complete this function
	public int getLinearProbing(String sid) throws KeyNotFoundException {
		
		Student key = new Student(sid, 0);
		int i = probe(key, null);
		int counter = 0;
		while(counter <= table.length){

			if(table[i] != null && SIDequals(table[i], key))
				return table[i].getGrade();
	
			i = probe(key, i);
			counter++;
		}
		throw new KeyNotFoundException();

	}

	/*
	 * Purpose: returns the number of elements in this Hashtable
	 * Parameters: none
	 * Returns: int - the number of elements
	 */
	public int size() {
		return count;
	}

	/*
	 * Purpose: returns a String reprensentation of elements
	 *      in this Hashtable separated by newlines
	 * Parameters: none
	 * Returns: String - the representation
	 */
	public String toString() {
		String s = "";

		for(int i=0; i<TABLE_SZ; i++) {
			if (table[i] != null) {
				s  += table[i] + "\n";
			}
		}
		
		return s;
	}
    

}
