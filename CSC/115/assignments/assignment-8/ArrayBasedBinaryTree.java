/*
 * ArrayBasedBinaryTree.java
 *
 * An array-based BinaryTree meant to store values of type Integer
 */

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.lang.Math;
public class ArrayBasedBinaryTree implements BinaryTree {
    private static final int CAPACITY = 5;
    protected Integer[] data;
    protected int root;
    protected int size;

	static int testCount;
	static int testsPassed;
    
	public ArrayBasedBinaryTree() {
		// TODO...
		// allocate space for data array.
		// What index are you choosing the root to be?
		// initialize your size as the number of elements in the empty tree
		data = new Integer[ CAPACITY ];
		size = 0;
	
	}

	/*
	 * Purpose: inserts the given value into data at next available
	 *  position in a level-order traversal
	 *  The tree remains complete after insertion.
	 * Parameters: Integer value - value to insert
	 * Returns: nothing
	 */
	public void insert(Integer value) {
		// TODO...
		// NOTE: The underlying data structure is an array,
		//  but we are just USING the array to store the data.
		//  The way we traverse the data will expose its binary tree structure

		if(size == 0) {

			data[size++] = value;
			return;
		}
		if(size >= data.length) expandAndCopy();
		data[size++] = value;

	}

	protected void expandAndCopy() {
		Integer[] newData = new Integer[data.length*2];
		for(int i=0; i<data.length; i++) {
			newData[i] = data[i];
		}
		data = newData;
	}

	/*
	 * Purpose: calculates and returns the index of t's left child
	 * Parameters: int t - index of current element in this ArrayBasedBinaryTree
	 * Returns: int - index of left child
	 */
	protected int getLeft(int t) {
		// TODO...
		return 2 * t + 1;
	}

	/*
	 * Purpose: calculates and returns the index of t's right child
	 * Parameters: int t - index of current element in this ArrayBasedBinaryTree
	 * Returns: int - index of right child
	 */
	protected int getRight(int t) {
		// TODO...
		return 2 * t + 2;
	}


	private void traverseInOrder(int idx) {

		if(idx >= size) return;
		traverseInOrder(getLeft(idx));
		System.out.print(" " + data[idx] + " ");
		traverseInOrder(getRight(idx));
	}
	public void inOrder(){
		// TODO...
		traverseInOrder(0);
		System.out.println("");
	}


	private void traversePreOrder(int idx) {

		if(idx >= size) return;
		System.out.print(" " + data[idx] + " ");
		traversePreOrder(getLeft(idx));
		traversePreOrder(getRight(idx));
	}
	public void preOrder(){
		// TODO...
		traversePreOrder(0);
		System.out.println("");
	}

	private void traversePostOrder(int idx) {

		if(idx >= size) return;
		traversePostOrder(getLeft(idx));
		traversePostOrder(getRight(idx));
		System.out.print(" " + data[idx] + " ");
	}
	public void postOrder(){
		// TODO...
		traversePostOrder(0);
		System.out.println("");
	}

	public int height() {
		return height(root);
	}

	/*
	 * Purpose: computes and returns the height of a 
	 *          binary tree rooted at index t 
	 * Parameters: TreeNode t - the BinaryTree
	 * Returns: int - the height
	 * NOTE: a BinaryTree with no nodes is height -1
	 *       a BinaryTree with just a root is height 0
	 *
	 *       the height of a node in a tree is equal to 
	 *       1 + the height of its largest subtree
	 */
	protected int height(int t) {
		if (t > size) {
            return 0;
        }
		//traverse right
		//traverse left
		//return biggest int
		int left = height(getLeft(t));
		int right = height(getRight(t));
		
		
		// TODO: complete the rest
		return Math.max(left, right) + 1; // so it compiles
	}
	

	/*
	 * Purpose: returns a String reprensentation of a in-order traversal
	 *     of this ArrayBasedBinaryTree
	 * Parameters: none
	 * Returns: String - the representation
	 */
	// written for you - do not change
	// NOTICE: we use the helper methods to get the index of the left/right
	//   children of the current position in the tree.
	// This method will not work until you have completed those methods correctly.
	public String toString() {
		return toString(root);
	}

	private String toString(int t) {
        if (t >= size) {
            return "";
        } 
        String s = "";
        s += toString(getLeft(t));
        s += data[t] + " ";
        s += toString(getRight(t));

        return s;
	}

	public String stringify() {
		String result = "";
		for(int i = 0; i < size; ++i)
			result += " " + data[i] + " ";
		return result;
	}
	
	public static void verifyOutput(Runnable code, String expectedMessage) {
        // Save the current System.out
        PrintStream originalOut = System.out;
        // Create a ByteArrayOutputStream to redirect output
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        PrintStream newOut = new PrintStream(outputStream);
        // Redirect System.out to the new stream
        System.setOut(newOut);
        // Execute the provided code
        code.run();
        // Reset System.out to the original stream
        System.setOut(originalOut);
        // Convert the output stream to a string for comparison
        String outputString = outputStream.toString();
        // Verify the output
		String successMessage;
		if(outputString.trim().equals(expectedMessage.trim())) {
			successMessage = "Passed";
			++testsPassed;
		} else {
			successMessage = "Failed";
		}
		++testCount;

		System.out.println(successMessage);
		System.out.println("expected: " + expectedMessage);
		System.out.println("  actual: " + outputString);
    }

	private static void tester() {
		ArrayBasedBinaryTree tree = new ArrayBasedBinaryTree();
		for(int i= 0; i < 7; ++i)
			tree.insert(i);

		
		////////////////////////////////////////////////////////////////////////
		String expected = " 3  1  4  0  5  2  6 ";
		System.out.println("InOrder\n");
		verifyOutput(() ->tree.inOrder(), expected);

		////////////////////////////////////////////////////////////////////////
		
        // Your code that uses System.out.println
		System.out.println("PreOrder\n");
		expected = " 0  1  3  4  2  5  6 ";
		verifyOutput(() ->tree.preOrder(), expected);

		////////////////////////////////////////////////////////////////////////
		System.out.println("PostOrder\n");
		expected = " 3  4  1  5  6  2  0 ";
		verifyOutput(() ->tree.postOrder(), expected);
	}

	public static void main(String[] args) {
		
		tester();
		System.out.println("Tests passed: " + testsPassed + "/" + testCount);

		ArrayBasedBinaryTree myTree = new ArrayBasedBinaryTree();
		for(int i=2; i<8; i++) {
			myTree.insert(i);
		}
		System.out.println("in:");
		myTree.inOrder();
		System.out.println("pre:");
		myTree.preOrder();
		System.out.println("post:");
		myTree.postOrder();
		
		System.out.println("toString\n" + myTree);
	}
    
}
