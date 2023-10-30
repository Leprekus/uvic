public class A6Tester {	
	private static int testPassCount = 0;
	private static int testCount = 0;	
	
	public static void main(String[] args) {
		queueTests();
		danceClubTests();
		
		System.out.println("Passed " + testPassCount + " / " + testCount + " tests");
	}
	
	public static void queueTests() {
		System.out.println("\nTesting queue implementation");
		
		Queue<Integer> q1 = new GenericQueue<Integer>();
		Integer expected = 0;
		Integer result = 0;
		
		displayResults(q1.size()==0, "size of empty queue");
		displayResults(q1.isEmpty(), "queue is empty");
		
		q1.enqueue(2);
		displayResults(q1.size()==1, "size after inserting to front");
		displayResults(!q1.isEmpty(), "after insertion no longer empty");
		
		try {
			result = q1.peek();
			expected = 2;
			displayResults(result.equals(expected), "correct element at front of queue");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		q1.enqueue(4);
		q1.enqueue(9);
		q1.enqueue(1);
		displayResults(q1.size()==4, "size after inserting to front");
		
		try {
			result = q1.peek();
			expected = 2;
			displayResults(result.equals(expected), "correct element at front of queue");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		try {
			result = q1.dequeue();
			result = q1.dequeue();
			expected = 4;
			displayResults(result.equals(expected), "correct element is dequeued");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		try {
			result = q1.peek();
			expected = 9;
			displayResults(result.equals(expected), "correct element at front of queue");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		try {
			result = q1.dequeue();
			result = q1.dequeue();
			expected = 1;
			displayResults(result.equals(expected), "correct element at dequeued");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		displayResults(q1.size()==0, "size of empty queue");
		displayResults(q1.isEmpty(), "queue is empty");
		
		try {
			result = q1.peek();
			displayResults(false, "cannot peek when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		try {
			result = q1.dequeue();
			displayResults(false, "cannot dequeue when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}		
		
		Queue<String> q2 = new GenericQueue<String>();

		displayResults(q2.size()==0, "q2 initially has size 0");
		displayResults(q2.isEmpty(), "q2 initially empty");

		try {
			q2.peek();
			displayResults(false, "cannot peek when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		try {
			q2.dequeue();
			displayResults(false, "cannot dequeue when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		q2.enqueue(new String("csc115"));
		q2.enqueue(new String("assignment6"));
		
		displayResults(q2.size()==2, "size after two enqueues");
		displayResults(!q2.isEmpty(), "not empty after two enqueues");
		
		q2.dequeueAll();
		
		try {
			q2.peek();
			displayResults(false, "cannot peek when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		try {
			q2.dequeue();
			displayResults(false, "cannot dequeue when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		q2.enqueue(new String("more inserts"));
		
		displayResults(q2.size()==1, "size after two enqueues");
		displayResults(!q2.isEmpty(), "not empty after two enqueues");
		
		try {
			displayResults(q2.peek().equals("more inserts"), "correct element at front of queue");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		try {
			displayResults(q2.dequeue().equals("more inserts"), "correct element at dequeued from queue");
		} catch (EmptyQueueException e) {
			displayResults(false, "exception thrown when it should not have been");
		}
		
		try {
			q2.peek();
			displayResults(false, "cannot peek when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
		
		try {
			q2.dequeue();
			displayResults(false, "cannot dequeue when queue is empty");
		} catch (EmptyQueueException e) {
			displayResults(true, "exception thrown when it should have been");
		} catch (Exception e) {
			displayResults(false, "incorrect exception thrown");
		}
	}
		
	public static void danceClubTests() {
		System.out.println("\nTesting dance club lineup implementation");
		
		Clubber p1 = new Clubber("Renton", 23);
		Clubber p2 = new Clubber("TechnoAddict", 23);
		Clubber p3 = new Clubber("Spud", 23);
		Clubber p4 = new Clubber("TranceX", 23);
		Clubber p5 = new Clubber("Begbie", 23);
		Clubber p6 = new Clubber("Diane", 23);
		Clubber p7 = new Clubber("SickBoy", 23);
		
		ClubLine d = new ClubLine();
		Clubber result = null;
		Clubber expected = null;
		boolean inserted = false;
		int numRemoved = 0;
		
		displayResults(d.clubbersInLine()==0, "clubbers in empty line");
		
		try {
			result = d.nextClubber();
			displayResults(result == null, "handled clubber in empty line");
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled clubber in empty line");
		}
		
		d.enterLine(p1);
		displayResults(d.clubbersInLine()==1, "clubbers in line 1");
		d.enterLine(p2);
		d.enterLine(p3);
		d.enterLine(p4);
		displayResults(d.clubbersInLine()==4, "clubbers in line 2");
		
		
		try {
			result = d.nextClubber();
			expected = p1;
			if (result == null) {
				displayResults(false, "handled first clubber in line 1");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 1");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 1");
		}
		
		displayResults(d.clubbersInLine()==3, "clubbers in line 3");
		d.enterLine(p5);
		d.enterLine(p6);
		displayResults(d.clubbersInLine()==5, "clubbers in line 4");
		
		try {
			inserted = d.nextVIP(p7, 2);
			displayResults(inserted==true, "inserted into specific pos");
		} catch (Exception e) {
			displayResults(false, "inserted into specific pos");
		}
		
		try {
			result = d.nextClubber();
			expected = p2;
			if (result == null) {
				displayResults(false, "handled first clubber in line 2");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 2");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 2");
		}

		try {
			result = d.nextClubber();
			expected = p3;
			if (result == null) {
				displayResults(false, "handled first clubber in line 3");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 3");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 3");
		}

		try {
			result = d.nextClubber();
			expected = p7;
			if (result == null) {
				displayResults(false, "handled first clubber in line 4");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 4");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 4");
		}
		
		displayResults(d.clubbersInLine()==3, "clubbers in line 4");		
		d.enterLine(p7);
		d.enterLine(p1);
		d.enterLine(p2);
		d.enterLine(p3);		
		displayResults(d.clubbersInLine()==7, "clubbers in line 5");
		
		try {
			numRemoved = d.nextGroup(5);
			displayResults(numRemoved==5, "group of 5 enters dance club");
		} catch (Exception e) {
			displayResults(false, "group of 5 enters dance club");
		}
		displayResults(d.clubbersInLine()==2, "clubbers in line 6");
		
		try {
			numRemoved = d.nextGroup(5); // buy 5 tickets, but only 2 in line
			displayResults(numRemoved==2, "group of 2 enters dance club");
		} catch (Exception e) {
			displayResults(false, "group of 2 enters dance club");
		}
		displayResults(d.clubbersInLine()==0, "clubbers in line 7");
		
		try {
			numRemoved = d.nextGroup(5); // buy 5 tickets, but no one in line
			displayResults(numRemoved==0, "group of 0 enters dance club");
		} catch (Exception e) {
			displayResults(false, "group of 0 enters dance club");
		}
		displayResults(d.clubbersInLine()==0, "clubbers in line 8");		
		
		d.enterLine(p1);
		displayResults(d.clubbersInLine()==1, "clubbers in line 9");		

		try {
			inserted = d.nextVIP(p2, 0); 
			displayResults(inserted==true, "inserted into specific pos");
		} catch (Exception e) {
			displayResults(false, "inserted into specific pos");
		}
		displayResults(d.clubbersInLine()==2, "clubbers in line 10");	

		try {
			inserted = d.nextVIP(p3, 2); 
			displayResults(inserted==true, "inserted into specific pos");
		} catch (Exception e) {
			displayResults(false, "inserted into specific pos");
		}
		displayResults(d.clubbersInLine()==3, "clubbers in line 11");	

		try {
			inserted = d.nextVIP(p4, -2); 
			displayResults(inserted==false, "inserted into specific pos");
		} catch (Exception e) {
			displayResults(false, "inserted into specific pos");
		}
		displayResults(d.clubbersInLine()==3, "clubbers in line 12");	

		try {
			inserted = d.nextVIP(p5, 4); 
			displayResults(inserted==false, "inserted into specific pos");
		} catch (Exception e) {
			displayResults(false, "inserted into specific pos");
		}
		displayResults(d.clubbersInLine()==3, "clubbers in line 13");		

		try {
			result = d.nextClubber();
			expected = p2;
			if (result == null) {
				displayResults(false, "handled first clubber in line 5");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 5");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 5");
		}

		try {
			result = d.nextClubber();
			expected = p1;
			if (result == null) {
				displayResults(false, "handled first clubber in line 6");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 6");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 6");
		}

		try {
			result = d.nextClubber();
			expected = p3;
			if (result == null) {
				displayResults(false, "handled first clubber in line 7");
			} else {
				displayResults(result.equals(expected), "handled first clubber in line 7");
			}
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 7");
		}
		
		try {
			result = d.nextClubber();
			displayResults(result == null, "handled first clubber in line 8");
		} catch (Exception e) {
			System.out.println("exception thrown");
			displayResults(false, "handled first clubber in line 8");
		}
	}

	public static void displayResults (boolean passed, String testName) {
		testCount++;
		if (passed) {
			System.out.println ("Passed test: " + testCount);
			testPassCount++;
		} else {
			System.out.println ("Failed test: " + testName + " at line "
								+ Thread.currentThread().getStackTrace()[2].getLineNumber());
		}
	}
}