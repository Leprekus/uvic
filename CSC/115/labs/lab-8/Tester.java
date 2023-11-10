public class Tester {
    static int testCount = 0;
    static int testPassCount = 0;

    public static void main(String[] args) {
        testArrayBasedBinaryTree();
        //testRefBasedBinaryTree();
        System.out.println("Tests passed: " + testPassCount + "/" + testCount);
    }

    public static void testRefBasedBinaryTree(){
        RefBasedBinaryTree tree = new RefBasedBinaryTree();
        tree.insert(0);
        tree.insert(1);
        tree.insert(2);
        
        
        
    }
    public static void testArrayBasedBinaryTree() {
        System.out.println("Test Array Based Binary Tree");

        ArrayBasedBinaryTree tree = new ArrayBasedBinaryTree();

        Integer expected = 0;
        Integer actual = tree.size;
        boolean result = actual.equals(expected);
    
        displayResults(result, "Initial data is Empty");

        
        tree.insert(2);
        tree.insert(4);
        tree.insert(6);
        tree.insert(8);
        tree.insert(10);

        Integer[] expected_1 = { 2, 4, 6, 8, 10 };
        result = stringify(expected_1).equals(tree.stringify());
        displayResults(result, "Inserts nodes");
        
        tree.insert(1);
        Integer[] expected_2 = { 1, 2, 4, 6, 8, 10 };
        result = stringify(expected_2).equals(tree.stringify());
        displayResults(result, "Expands array and inserts node in correct order");
        System.out.println("actual " + tree.stringify() + " expected " + stringify(expected_2));
        
        tree.insert(11);
        Integer[] expected_3 = { 1, 2, 4, 6, 8, 10, 11 };
        result = stringify(expected_3).equals(tree.stringify());
        displayResults(result, "Inserts at end");
        System.out.println("actual " + tree.stringify() + " expected " + stringify(expected_3));
        
        tree.insert(1);
        Integer[] expected_4 = { 1, 2, 4, 6, 8, 10, 11 };
        result = stringify(expected_4).equals(tree.stringify());
        displayResults(result, "Does not insert duplicate");
        System.out.println("actual " + tree.stringify() + " expected " + stringify(expected_4));

        ArrayBasedBinaryTree tree2 = new ArrayBasedBinaryTree();
        tree2.insert(0);
        tree2.insert(1);
        tree2.insert(2);
        tree2.insert(3);
        tree2.insert(4);
        tree2.insert(5);
        tree.preOrder();
        
        


    }

    public static String stringify(Integer[] data) {
        String result = "{";
        for(int e : data) result += " " + e + " ";
        result += "}";
        
        return result;
    }
    public static void displayResults (boolean passed, String testName) {
		testCount++;
		if (passed) {
			System.out.println ("Passed test: " + testCount + " " + testName);
			testPassCount++;
		} else {
			System.out.println ("Failed test: " + testName + " at line "
								+ Thread.currentThread().getStackTrace()[2].getLineNumber());
                                
		}
	}
    
}
