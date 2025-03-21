#include <iostream>
#include <ostream>
#include <vector>
#include <limits>
class Node {

	public:
		int data;
		Node *left;
		Node *right;
		Node(int data) {
			this->data = data;
			left = nullptr;
			right = nullptr;
	}

};
class BST {
		
	public:
		Node *root;
		BST(int data) {
			root = new Node(data);
		}
		Node *insert(Node* curr, int data) {
				
			if(curr == nullptr)
				return new Node(data);
			
			if(data < curr->data)
				curr->left = insert(curr->left, data);
			
			if(data > curr->data)
				curr->right = insert(curr->right, data);

			return curr;

		}

		void print_helper(Node *curr){
			if(curr == nullptr) return;
			
			print_helper(curr->left);
		    	std::cout << curr->data << " ";  // Removed unnecessary std::endl
		    	print_helper(curr->right);

		
		}
		void print(){
			print_helper(root);
			std::cout << std::endl;
		}

		Node *get(Node *curr, int data) {
			if(curr == nullptr)
				return nullptr;

			if(data < curr ->data)
				get(curr->left, data);
			if(data > curr ->data)
				get(curr->right, data);

			return curr;
		}
	
		int get_successor(Node *curr, int data){

			int successor =  std::numeric_limits<int>::max();

			while(curr != nullptr) {
				if(curr->data > data && curr->data < successor){
					successor = curr->data;
					curr = curr->left;
					continue;
				}
				
				curr = curr->right;
			}

			return successor;
		}
	
		void print_successor(int data) {
				
			//Node *successor = get_successor(root, data);
		}

};
int find_successor_of(std::vector<int> binary_tree, int target){
	//successor: when node.val > curr.val
	//case 1: current node has a right subtree
	//then successor is LEFTMOST node in the RIGHT subtree
	//
	//case 2: current node has no right child.
	//then ancestor must contain current node must in the left subtree,
	

	
	return 0;
}
int main() {
	std::vector<int> binary_tree = { 15, 7, 30, 2, 11, 20, 35, 1, 9, 12, 32, 40 };
	std::vector<int> targets = { 11, 20, 2, 40, 7 };
	//max used as sentinel value for none
     	std::vector<int> expected = { 12, 30, 7, std::numeric_limits<int>::max(), 9 };

	BST bst = BST(binary_tree[0]);
	for(auto i = 1; i < binary_tree.size(); i++)
		bst.insert(bst.root, binary_tree[i]);

	bst.print();

	for(auto i = 0; i < targets.size(); i++)
		bst.print_successor(targets[i]);	
	

	for(int i = 0; i < targets.size(); i++){
		int actual = bst.get_successor( bst.root, targets[i] );
		bool isCorrect = actual == expected.at(i);
		if(isCorrect)
			std::cout << "correct, expected " << targets.at(i) << ": " << expected.at(i) << " got " << targets.at(i) << ": " << actual << std::endl;
		else std::cout << "incorrect, expected " <<  targets.at(i) << ": " << expected.at(i) << " got " << targets.at(i) << ": " << actual << std::endl;
	}
	return 0;
}
