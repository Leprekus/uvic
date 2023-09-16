/*
* Student.java
* A Student class
*/
public class Student {

	private String sID;
	private int grade;

	public Student() {
		sID = "";
		grade = -1;
	}

	public Student(String sID, int grade) {
		this.sID = sID;
		this.grade = grade;
	}
	
	public String getSID() {
		return sID; // so it compiles
	}

	public String setSID(String newSID) {
		sID = newSID;
		return sID;
	}
	
	public int getGrade() {
		return grade; // so it compiles
	}

	public String toString() {

		return Integer.toString(grade);

	}

}
