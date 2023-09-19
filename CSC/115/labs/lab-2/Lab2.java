/*
 * Lab2.java
 *
 * A class of static methods that operate on Students
 *
 */

import java.text.DecimalFormat;
import java.util.Arrays;

import javax.swing.text.StyledDocument;

import java.util.ArrayList;

public class Lab2 {
    
	/*
	 * Purpose: determines which student has the higher grade
	 * Parameters: Student - s1, Student - s2
	 * Returns: Student - the Student with the higher grade,
	 *                    s1 if they have the same grade
	 * Precondition: s1 and s2 are not null
	 */
	public static Student getHigherGradeStudent(Student s1, Student s2) {
		// TODO: implement getHigherGradeStudent
		int grade_s1 = s1.getGrade();
		int grade_s2 = s2.getGrade();

		if(grade_s1 == grade_s2) return s1;


		return grade_s1 > grade_s2 ? s1 : s2; // so it compiles
	}


	/*
	 * Purpose: determines whether the grade of Student s
	 *          is above the threshold
	 * Parameters: Student - s, int - threshold
	 * Returns: boolean - true if grade is above threshold, false otherwise
	 * Preconditions: s is not null
	 */
	// TODO: implement isGradeAbove
	public static boolean isGradeAbove(Student s, int threshold) { 

		return s.getGrade() > threshold ? true : false;
	 }


	/*
	 * Purpose: creates an array sIDs of all Students in students
	 * Parameters: Student[] - students
	 * Returns: String[] - array of sIDs
	 * Preconditions: students is not null and contains no null elements
	 */
	// TODO: implement getClasslist
	 public static String[] getClassList(Student[] students) {

		if(students.length == 0) return new String[ 0 ];

		ArrayList<String> studentIds = new ArrayList<String>();
		int len = 0;
		for (int i = 0; i < students.length; i++) {

			Student student = students[i];

			if(student.getSID() instanceof String) {
				String id = student.getSID();
				studentIds.add(id);
			
			} 
		}
		String[] arr = studentIds.toArray(new String[ studentIds.size() ]);

		
		return arr;
	 }

	/*
	 * Purpose: counts the number of Students in students
	 *          with grade above threshold
	 * Parameters: Student[] - students, int - threshold
	 * Returns: int - the number of students with a grade above threshold
	 * Preconditions: students is not null and contains no null elements
	 */
	// TODO: implement countAbove
	// HINT: you should be using the isGradeAbove method!
	 public static int countAbove(Student[] students, int threshold) {

		int count = 0;
		for(int i = 0; i < students.length; i++) {

			Student student = students[i];
			boolean isAbove = isGradeAbove(student, threshold);
			if(isAbove) count ++;

			
		}
		return count;
	 }

	/*
	 * Purpose: calculates the average grade of Students in students,
	 *          does NOT include students with -1 grade in calculation
	 *          average is 0.0 if students is empty or all students have -1 grade
	 * Parameters: Student[] - students
	 * Returns: double - the average grade
	 * Preconditions: students is not null and contains no null elements
	 */
	// TODO: implement getClassAverage
	// HINT: you can use the isGradeAbove method again
	public static double getClassAverage(Student[] students) {

		if(students.length == 0) return 0.0;

		int num = 0;
		int len = 0;
		for(int i = 0; i < students.length; i++) {
			Student student = students[i];
			boolean isAbove = isGradeAbove(student, 0);
			if(isAbove) {
				num += student.getGrade();
				len ++;
				//System.out.println("grade: " + student.getGrade() + " len" + );
			}
		}
		double average = ( double ) num / len;
		
		DecimalFormat df = new DecimalFormat("#.##");
		String roundedNumber = df.format(average);
        double roundedDouble = Double.parseDouble(roundedNumber);
		System.out.println(roundedDouble );
		return roundedDouble;
	}


	/*
	 * Purpose: creates a new array 1 longer than students
	 *          and adds all students and s to the new array
	 * Parameters: Student[] - students, Student s
	 * Returns: Student[] - the new array
	 * Preconditions: students is not null and contains no null elements
	 *                Student s is not already contained within students
	 */
	// TODO: implement registerStudent
	public static Student[] registerStudent(Student[] students, Student s) {

		if(students.length != 0 && s.getSID().length() > 0){
			Student[] arr = { s };
			return arr;
		};


		Student[] newStudents = new Student[ students.length + 1 ];
	
		newStudents[ students.length ] = s;

		return newStudents;

	}

}
