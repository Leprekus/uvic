import java.lang.Math;
/*
 * RefBasedBinarySearchTree.java
 *
 * A ref-based BinaryTree meant to store values of type Integer
 */
public class RefBasedBinarySearchTree extends RefBasedBinaryTree {


    private TreeNode insert(TreeNode current, int value) {
        if(current == null) return new TreeNode(value);

        if(value < current.getValue())
            current.left = insert(current.getLeft(), value);
        
        if(value > current.getValue())
            current.right = insert(current.getRight(), value);
        
        return current;
    }

    public void insert(int n) {
        if(root == null) {
            root = new TreeNode(n);
            return;
        }
        insert(root, n);
    }

    private void calculateSum(TreeNode current, int[] sum) {
        if(current == null) return;
        sum[0] += current.getValue();
        calculateSum(current.left, sum);
        calculateSum(current.right, sum);
    }
    public int sum() {
        if(root == null) return 0;
        int[] sum = { 0 };
        calculateSum(root, sum);
        return sum[0];
    }

    private void findVal(int val, boolean[] found, TreeNode current) {
        if(current == null) return;
        if(current.getValue() == val) {
            found[0] = true;
            return;
        }
        findVal(val, found, current.left);
        findVal(val, found, current.right);
    }

    public boolean find(int val) {
        if(root == null) return false;

        boolean[] found = { false };
        findVal(val, found, root);
        return found[0];
    }

    private void getMaxHelper(TreeNode current, int[] max) {
        if(current == null) return;
        if(current.getValue() > max[0]) max[0] = current.getValue();
        getMaxHelper(current.left, max);
        getMaxHelper(current.right, max);
    }
    private int _getMax() throws TreeEmptyException {
        if(root == null) throw new TreeEmptyException();
        int[] max = { 0 };
        getMaxHelper(root, max);
        return max[0];

    }

    public int getMax() {
        try {
            return _getMax();
        } catch(TreeEmptyException e) {
            return 0;
        }
    }



    public static void main(String[] args) {
        // use these trees to test the methods you will implement
        RefBasedBinarySearchTree emptyTree = new RefBasedBinarySearchTree();
        RefBasedBinarySearchTree myTree = new RefBasedBinarySearchTree();
        myTree.insert(2);
        myTree.insert(1);
        myTree.insert(5);
        myTree.insert(7);
        myTree.insert(0);
        myTree.insert(4);
        myTree.insert(6);
        
        System.out.println("in");
        myTree.inOrder();
        System.out.println("pre");
        myTree.preOrder();
        System.out.println("post");
        myTree.postOrder();

        System.out.println("sum " + myTree.sum());
        System.out.println("max " + myTree.getMax());
        System.out.println("find " + myTree.find(0));
        
        System.out.println("toString\n" + myTree);
    }
}
