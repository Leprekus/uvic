/*
 * ArrayBasedBinaryTree.java
 *
 * An array-based BinaryTree meant to store values of type Integer
 */
public class ArrayBasedBinaryTree implements BinaryTree {
    private static final int CAPACITY = 5;
    protected Integer[] data;
    protected int root;
    protected int size;
    
	public ArrayBasedBinaryTree() {
		
		// [ x ] allocate space for data array.
		// [ x ] What index are you choosing the root to be?
		// [ x ] initialize your size as the number of elements in the empty tree

		/**
		 * index of the left child = 2 * index of the current node + 1
		 * index of the right child = 2 * index of the current node + 2
		 */
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
		if(size == 0) { //insert root
			data[size++] = value;
			return;
		}
		if(size == data.length - 1) expandAndCopy();

		int idx = 0;
		Integer current = data[idx];
		while(idx < size) {
			if(value == current) return;
			if(value < current) {
				data[idx] = value;
				value = current;
				current = data[++idx];
				continue;
			}
			++idx;
		}
		data[size++] = value;
		return;

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
		return  2 * t + 1;
	}

	/*
	 * Purpose: calculates and returns the index of t's right child
	 * Parameters: int t - index of current element in this ArrayBasedBinaryTree
	 * Returns: int - index of right child
	 */
	protected int getRight(int t) {
		return 2 * t + 2;
	}

	private void traverse(int idx) {
		/*
		 * takes in a "root" node
		 * and traverses the tree
		 * printing the values
		 */
		if(idx >= size) return;

		traverse(getLeft(idx));
		System.out.println(data[idx]);
		traverse(getRight(idx));
	}
	public void inOrder(){
		// TODO...
		/**
		 * the left subtree to the root then to the right subtree.
		 */
		traverse(0);
	}

	public void preOrder(){
		// TODO...
		System.out.println(data[0]); //root node
		traverse(1);//left node
		traverse(2);//right node
	}

	public void postOrder(){
		// TODO...
		traverse(1);//left node
		traverse(2);//right node
		System.out.println(data[0]); //root node
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
		if (t >= size) {
            return 0;
        } 
		// TODO: complete the rest
		return (int) Math.floor((Math.log(t + 1) / Math.log(2)));
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
		String result = "{";
		for(int i = 0; i < size; ++i) {
			result += " " + data[i] + " ";
		}
		result += "}";
		return result;
	}

	public static void main(String[] args) {
		
		ArrayBasedBinaryTree myTree = new ArrayBasedBinaryTree();
		for(int i=2; i<8; i++) {
			myTree.insert(i);
		}
		System.out.println("in");
		myTree.inOrder();
		System.out.println("pre");
		myTree.preOrder();
		System.out.println("post");
		myTree.postOrder();
		
		System.out.println("toString\n" + myTree);
	}
    
}
