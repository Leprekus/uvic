// SongTester.java
//
// We will write code here to test our implementation of Song.java
//
// We have put this code in a different file to help you understand 
// the difference between static and non-static.  In the real world,
// this style of test code might be included in the Song.java file.

public class SongTester {

	public SongTester() {}

	boolean comparator (Song s1, Song s2) {
		return s1.equals(s2);
	}
    
	public static void main (String[] args) {

		/* Recall: if I want to declare an integer: */
		int i; // what is it initialized to?
		
		/* if I want to declare an array of integers: */
		int[] array;
		
		/* what is it initialized to?
		how would I initialize it to point to an array of size 10? */
		
		/* 3a. declare and initialize two new blank songs */
		Song s1 = new Song("Some song", "Queen");
		Song s2 = new Song("Party Rock", "LMFAO");



		/* 3b. print the values of the fields for each song */
		System.out.println(s1.getSongTitle());
		System.out.println(s2.getSongTitle());

		SongTester songTester = new SongTester();
		boolean areSongsEqual = songTester.comparator(s1, s2);
		System.out.println(areSongsEqual);

		/* 3c. set the fields in the song to some values and reprint */
		

		/* 5a. update statements where necessary from 3b and 3c */

		/* 6a. call constructor with args, then print fields */


		/* 6b. call alternative constructor, then print fields */





		/* 6c. try to print one of the songs
		   ie. pass your Song variable to System.out.println() */


		/* 7a. recompile and run your program - what changes in 6c. */
	}
}