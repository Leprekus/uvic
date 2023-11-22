import java.util.*;

import javax.swing.tree.TreeNode;

/*
 * An implementation of a binary search tree. This tree stores
 * both keys and values associated with those keys.
 *
 * More information about binary search trees can be found here:
 * http://en.wikipedia.org/wiki/Binary_search_tree
 */
class BinarySearchTree <K extends Comparable<K>, V>  {

	public static final int BST_PREORDER  = 1;
	public static final int BST_POSTORDER = 2;
	public static final int BST_INORDER   = 3;

	// These are package friendly for the TreeView class
	BSTNode<K,V> root;
	int	count;

	public BinarySearchTree () {
		root = null;
		count = 0;
	}

	
	/* Purpose: Insert a new key-value element into the tree.
	 *          If the key already exists in the tree, update
	 *          the value stored at that node with the new value.
	 * Parameters: K key - the key for which the BST is ordered
     *	 		   V value - the value to associate with the key
	 * Returns: nothing
	 * Pre-Conditions: the tree is a valid binary search tree
	 */
	private BSTNode<K, V> insert(BSTNode<K, V> current, K key, V value) {

        if(current == null) return new BSTNode<K, V>(key, value);

		int comparable = key.compareTo(current.key);
        if(comparable < 0)
            current.left = insert(current.left, key, value);
        
        if(comparable > 0)
            current.right = insert(current.right, key, value);
    
		if(comparable == 0){
			--count;
			current.value = value;
		}
		
		return current;
    }
	
	public void insert (K key, V value) {
		// TODO: implement this
		++count;
		if(root == null) {
			root = new BSTNode<K, V>(key, value);
			return;
		}
		insert(root, key, value);
	}

	/*
	 * Purpose: Get the value of the given key.
	 * Parameters: K key - the key to search for
	 * Returns: V - the value associated with the key
	 * Pre-conditions: the tree is a valid binary search tree
	 * Throws: KeyNotFoundException if key isn't in the tree
	 */
	private void findHelper(BSTNode<K, V>current, K key, V[] payload) {
        if(current == null) return;
		
		int comparable = current.key.compareTo(key);
        if(comparable == 0) {
            payload[0] = current.value;
			return;
		}
        
		if(comparable > 0)
			findHelper(current.left, key, payload);
		
		if(comparable < 0)
			findHelper(current.right, key, payload);
        // findHelper(current.left, key, payload);
        // findHelper(current.right, key, payload);
    }

    public V find(K key) throws KeyNotFoundException {
        if(root == null) throw new KeyNotFoundException();

        @SuppressWarnings("unchecked")
		V[] payload = (V[]) new Object[ 1 ];
	
        findHelper(root, key, payload);
		if(payload[0] == null) throw new KeyNotFoundException();
		
        return payload[ 0 ];
    }
	// public V find (K key) throws KeyNotFoundException {
	// 	// TODO: implement this
	// 	throw new KeyNotFoundException();
	// }

	/* 	
	 * Purpose: Get the number of nodes in the tree.
	 * Parameters: none
	 * Returns: int - the number of nodes in the tree. 
	 */
	public int size() {
		// TODO: implement this
		return count; // so it compiles
	}

	/*
	 * Purpose: Remove all nodes from the tree.
	 * Parameters: none
	 * Returns: nothing
	 */
	public void clear() {
		// TODO: implement this
		root = null;
	}

	/* 
	 * Purpose: Get the height of the tree. 
	 * Parameters: none
	 * Returns: int - the height of the tree
	 * Example: We define height as being the number of 
	 * arrows that need to be followed on the path from 
	 * the root to the deepest leaf node. This means that 
	 * a tree with one node (just the root) has height 0,
	 * and an empty tree (root is null) has height -1.
	 *
	 * See the assignment PDF and the test program for examples.
	 */
	private int heightHelper(BSTNode<K, V> current, int height) {
		if(current == null) return -1;

		int leftHeight = heightHelper(current.left, ++height);
		int rightHeight = heightHelper(current.right, ++height);

		return Math.max(leftHeight, rightHeight) + 1;
	}

	public int height() {
		if(root == null) return -1;

		return heightHelper(root, 0); // so it compiles
	}

	/* 
	 * Purpose: Return a list of all the key-value Entry elements 
	 *          stored in the tree using a level-order traversal.
	 * Parameters: None
	 * Returns: List<Entry<K,V>> - a list of key-value entries
	 *
	 * Example: A level order traversal of a tree cannot be done 
	 *          without the help of a secondary data structure.
	 *
	 *          It is commonly implemented using a queue of nodes 
	 *          as the secondary data structure. You will still be 
	 *          adding the "visited" elements to l as you do in the 
	 *          other traversal methods but you will create an 
	 *          additional q to hold nodes still to visit. This is
	 *          similar to what we did in the worksheet on tree traversals.
	 *
	 * From wikipedia (they call it breadth-first), the algorithm 
	 * for level order is:
	 *
	 *  levelorder()
	 *      q = empty queue
	 *      q.enqueue(root)
	 *      while not q.empty do
	 *          node := q.dequeue()
	 *          visit(node)
	 *          if node.left != null then
	 *                q.enqueue(node.left)
	 *          if node.right != null then
	 *                q.enqueue(node.right)
	 *
	 * Note that you can use the Java LinkedList as a Queue
	 * and then use only the removeFirst() and addLast() methods on q
	 */
	public List<Entry<K,V>>	entryList() {
		// list to add all the nodes to
		List<Entry<K,V> > l = new LinkedList<Entry<K,V>>();
		
		// queue of nodes that need to be added
		LinkedList<BSTNode<K,V>> q = new LinkedList<BSTNode<K,V> >();
		
		q.addLast(root);
		
		// TODO: implement this
		
		while(!q.isEmpty()) {
			BSTNode<K, V> current = q.removeFirst();
			if(current.left != null)
				q.addLast(current.left);

			if(current.right != null)
				q.addLast(current.right);

			Entry<K, V> entry = new Entry<K, V>(current.key, current.value);
			l.add(entry);
		}
		
		return l;
	}

	/* 	
	 * Purpose: Get a list of all the key-value entries stored in the tree
	 * Parameters: int whichTraversal - the type of traversal to perform:
	 * Returns: List<Entry<K,V>> - a list of key-value entries
	 * Example: The list will be constructed by performing a traversal
	 * specified by the parameter whichTraversal.
	 * 
	 * If whichTraversal is:
	 *	 BST_PREORDER	perform a pre-order traversal
	 *	 BST_POSTORDER	perform a post-order traversal
	 *	 BST_INORDER	perform an in-order traversal
	 */
	public List<Entry<K,V> > entryList (int which) {
		List<Entry<K,V> > entries = new LinkedList<Entry<K,V> >();

		if (which == BST_PREORDER) {
			preOrderRec(root, entries);
		}
		else if (which == BST_INORDER) {
			inOrderRec(root, entries);
		}
		else if (which == BST_POSTORDER) {
			postOrderRec(root, entries);
		}
		// switch(which) {
		// 	case BST_PREORDER:
		// 	preOrderRec(root, entries);

		// 	case BST_INORDER:
		// 	inOrderRec(root, entries);

		// 	case BST_POSTORDER:
		// 	postOrderRec(root, entries);
			
		// }
		return entries;
	}
	
	
	private void inOrderRec (BSTNode<K,V> n, List <Entry<K,V>> entries) {
		// TODO: implement this so it adds all of the elements in
		// the tree to the entries list with an in-order traversal
		if(n == null) return;
		Entry<K, V> e = new Entry<K, V>(n.key, n.value);

		inOrderRec(n.left, entries);
		entries.add(e);
		inOrderRec(n.right, entries);

	}
	

	private void preOrderRec (BSTNode<K,V> n, List <Entry<K,V>> entries) {
		// TODO: implement this so it adds all of the elements in
		// the tree to the entries list with a pre-order traversal
		if(n == null) return;
		Entry<K, V> e = new Entry<K, V>(n.key, n.value);
		
		entries.add(e);
		inOrderRec(n.left, entries);
		inOrderRec(n.right, entries);
	}

	private void postOrderRec (BSTNode<K,V> n, List <Entry<K,V>> entries) {
		//TODO: implement this so it adds all of the elements in
		if(n == null) return;
		Entry<K, V> e = new Entry<K, V>(n.key, n.value);
		
		inOrderRec(n.left, entries);
		inOrderRec(n.right, entries);
		entries.add(e);
	}
}