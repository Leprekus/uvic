public class ArrayBasedBinarySearchTree extends ArrayBasedBinaryTree{


    private void insertHelper(int idx, int value) {
        Integer item = data[idx];

        if(item == null) {
            data[idx] = value;
            return;
        }

        if(value < item) {
            idx = getLeft(idx);
            insertHelper(idx, value);
        }
        if(value > item) {
            idx = getRight(idx);
            insertHelper(idx, value);
        }
    }

    public void insert(Integer value) {
        int idx = 0;
        insertHelper(idx, value);
        size++;
        return;
    }

    public static void main(String[] args) {
        ArrayBasedBinarySearchTree emptyTree = new ArrayBasedBinarySearchTree();
        
        ArrayBasedBinarySearchTree myTree = new ArrayBasedBinarySearchTree();
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
        
        System.out.println("toString\n" + myTree);
    }

}
