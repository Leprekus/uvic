
public class A4Tester {

	private static int testPassCount = 0;
	private static int testCount = 0;
	
	public static void main(String[] args) {
		testStackOperations();
		testStackIsGeneric();
		testStackedCorrectly();
		testInsertPlate();
		
		System.out.println("Passed " + testPassCount + " / " + testCount + " tests");
	}
	
	public static void testStackOperations() {
		System.out.println("Stack Operations Tests:");
		A4Stack<Integer> testStack = new A4Stack<Integer>();
		int result = 0;
		
		displayResults(testStack.isEmpty(), "stack initially empty");
				
		testStack.push(2);
		result = testStack.top();
		displayResults(!testStack.isEmpty(), "stack no longer empty");
		displayResults(result==2, "top works after initial push");
		
		//TODO: add more tests here
		 A4Stack<Integer> myStack = new A4Stack<Integer>();
		 myStack.push(1);
		 myStack.push(1);
		 myStack.push(1);
		 myStack.push(1);

		 myStack.pop();
		 myStack.pop();
		 myStack.pop();
		 myStack.pop();

		 displayResults(myStack.isEmpty() == true, "test is emptied with pop");

		 displayResults(myStack.pop() == null, "pop on empty returns null");


				
		System.out.println();
	}
	
	public static void testStackIsGeneric() {
		System.out.println("Stack Generics Tests:");
		A4Stack<Integer> s1 = new A4Stack<Integer>();
		A4Stack<String> s2 = new A4Stack<String>();
		A4Stack<Double> s3 = new A4Stack<Double>();
		
		int result1;
		String result2;
		double result3;
		
		s1.push(3);
		s1.push(8);
		s2.push("CSC");
		s2.push("ENGR");
		s3.push(5.5);
		s3.push(9.1);
		
		result1 = s1.pop();
		result2 = s2.pop();
		result3 = s3.pop();
		
		displayResults(result1==8, "Integer Stack");
		displayResults(result2.equals("ENGR"), "String Stack");
		displayResults(result3==9.1, "Double Stack");		
		
		result1 = s1.top();
		result2 = s2.top();
		result3 = s3.top();
		
		displayResults(result1==3, "Integer Stack");
		displayResults(result2.equals("CSC"), "String Stack");
		displayResults(result3==5.5, "Double Stack");
		
		//TODO: add more tests here
		
		System.out.println();
	}

	public static void testStackedCorrectly() {
		System.out.println("Testing stackedCorrectly");
		Plate p1 = new Plate(5);
		Plate p2 = new Plate(12);
		Plate p3 = new Plate(1);
		Plate p4 = new Plate(8);
		Plate p5 = new Plate(3);
		
		A4Stack<Plate> s1 = new A4Stack<Plate>();
		A4Stack<Plate> s2 = new A4Stack<Plate>();
		
		s1.push(p2); //12
		s1.push(p4); //8
		s1.push(p1); //5
		s1.push(p5); //3
		s1.push(p3); //1
		
		s2.push(p1); //5
		s2.push(p4); //8
		
		boolean result = false;
		
		result = A4Exercises.stackedCorrectly(s1);
		displayResults(result, "s1 stacked correctly");
		
		result = A4Exercises.stackedCorrectly(s2);
		displayResults(!result, "s2 stacked correctly");
		
		//TODO: add more tests here
		
		System.out.println();
	}

	public static void testInsertPlate() {
		System.out.println("Testing insertPlate");
		Plate p1 = new Plate(5);
		Plate p2 = new Plate(12);
		Plate p3 = new Plate(1);
		Plate p4 = new Plate(8);
		Plate p5 = new Plate(3);

		
		Plate insert = new Plate(6);
		
		A4Stack<Plate> s1 = new A4Stack<Plate>();
		
		
		s1.push(p2); //12
		s1.push(p4); //8
		s1.push(p1); //5
		s1.push(p5); //3
		s1.push(p3); //1

		System.out.println(s1.toString());
		A4Exercises.insertPlate(s1, insert);
		displayResults(A4Exercises.stackedCorrectly(s1)==true, "insert into empty stack");
		
		Plate p13 = new Plate(13);
		s1.push(p2); //12
		s1.push(p4); //8
		s1.push(p1); //5
		s1.push(p5); //3
		s1.push(p3); //1

		System.out.println(s1.toString());
		A4Exercises.insertPlate(s1, p13);
		
		displayResults(A4Exercises.stackedCorrectly(s1)==true, "Insert at bottom of stack");
		
		s1.push(p2); //12
		s1.push(p4); //8
		s1.push(p1); //5
		s1.push(p5); //3
		s1.push(p3); //1
		
		Plate p0 = new Plate(0);
		A4Exercises.insertPlate(s1, p0);

		displayResults(A4Exercises.stackedCorrectly(s1)==true, "Insert at top of stack");
		
		
		A4Exercises.insertPlate(s1, p0);
		System.out.println(s1.toString());
		displayResults(A4Exercises.stackedCorrectly(s1)==true, "Insert on empty stack");

		//TODO: add more tests here
	}

	public static void displayResults (boolean passed, String testName) {
       /* There is some magic going on here getting the line number
        * Borrowed from:
        * http://blog.taragana.com/index.php/archive/core-java-how-to-get-java-source-code-line-number-file-name-in-code/
        */
        
        testCount++;
        if (passed)
        {
            System.out.println ("Passed test: " + testName);
            testPassCount++;
        }
        else
        {
            System.out.println ("Failed test: " + testName + " at line "
                                + Thread.currentThread().getStackTrace()[2].getLineNumber());
        }
    }
	
}