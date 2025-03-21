#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//polynomial rolling hash function:
//where s = string, p = prime, m = modulo
//hash is equal to the sum of:
// ( s[0]*p^0 + s[1]*p^1 + s[2]*p^2 + ... + p[n-1]*p^(n-1) ) % m
long long hash(const char *string){
	//prime rougly equal to the number of inputs our hash may receive:
	//a-z
	const int p = 31;
	//modulo (a large prime), hash range is from [0, m)
	const int m =  1e9 + 9;
	long long value = 0;
	long long p_pow = 1;

	for(int i = 0; i < strlen(string); i++){
		//map our char to a numerical value starting at 1
		value = (value + (string[i] - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return value;
}


enum Color  {
	RED = 0x02,
	BLACK = 0x01
};
struct RBNode {
	struct RBNode *parent;
	struct RBNode *left;
	struct RBNode *right;

	const long long key;	
	const char *value;
	enum Color color;

};

 struct RBTree{
	struct RBNode *root;
};

struct RBNode *RBTree_node(const long long key, const char *value) {
	struct RBNode *x = (struct RBNode *)malloc(sizeof(struct RBNode));

	if(x == NULL){
		printf("RBNode init()");
		exit(1);
	}

	x->parent = NULL;
	x->left = NULL;
	x->right = NULL;
	x->value = value;
	x->color = RED;

	return x;
}

void RBTree_rotate_left(struct RBTree *T, struct RBNode *x){
	if(x->right == NULL){
		printf("RBTRee_rotate_left()");
		exit(1);
	}
	
	struct RBNode *y = x->right;
	x->right = y->left;

	if(y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if(x->parent == NULL)
		T->root = y;
	else if(x == x->parent->left)
			x->parent->left = y;
	else x->parent->right = y;

	y->left = x;
	x->parent = y;
	
}
void RBTree_rotate_right(struct RBTree *T, struct RBNode *y){
	if(y->left == NULL){
		printf("RBTRee_rotate_right()");
		exit(1);
	}
	struct RBNode *x = y->left;
	y->left = x->right;
	//turn x's into y's left subtree
	if(x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;
	if(y->parent == NULL)
		T->root = x;
	else if (y == y->parent->left)
			y->parent->left = x;
	else  y->parent->right = x;

	x->right = y;
	y->parent = x;

}
void RBTree_search(){}
void RBTree_insert_fix(struct RBTree *T, struct RBNode *n){

	struct RBNode *parent = n->parent;
	while(parent != NULL && parent->color == RED){
		struct RBNode *grandparent = parent->parent;
		
		if(grandparent == NULL) break;

		//if parent is a left child
		if(grandparent->left == parent){
			struct RBNode *uncle = grandparent->right;			

			//case 1: parent and uncle are RED, make both black, and grandparent RED
			if(uncle != NULL && uncle->color == RED){
				parent->color = BLACK;
				uncle->color = BLACK;	
				grandparent->color = RED;
				n = grandparent;

			//case 2: parent is RED and uncle is BLACK, and n is a right child, 
			} else if(n == parent->right){
				n = parent;
				RBTree_rotate_left(T, n);
			} 
			//case 3: note: cases 1 and 2 fall through to 3
			parent->color = BLACK;
			grandparent->color = RED;
			RBTree_rotate_right(T, grandparent);
		
		//same with right and left exchanged
		} else {
			struct RBNode *uncle = grandparent->left;			

			if(uncle != NULL && uncle->color == RED){
				parent->color = BLACK;
				uncle->color = BLACK;	
				grandparent->color = RED;
				n = grandparent;

			} else if(n == parent->left){
				n = parent;
				RBTree_rotate_left(T, n);
			} 
			parent->color = BLACK;
			grandparent->color = RED;
			RBTree_rotate_left(T, grandparent);
		
		}
	}
	T->root->color = BLACK;

}
void RBTRee_insert(struct RBTree *T, const char *s){

	long long key = hash(s);
	struct RBNode *n = RBTree_node(key, s);

	struct RBNode *curr = NULL;
	struct RBNode *next = T->root;
	while(next != NULL){
		curr = next;
		if(key < next->key)
			next = next->left;
		else
			next = next->right;
	}
	n->parent = curr;
	if(curr == NULL)
		T->root = n;

	else if(key < curr->key)
		curr->left = n;

	else curr->right = n;

	//setting n's left, right, and color values to NULL, NULL, RED respectively
	//is handled in RBTRee_node()
	RBTree_insert_fix(T, n);

}
void RBTRee_delete(){}

void RBTree_init(struct RBTree *T){
	T->root = NULL;
};
int main() {
	struct RBTree T;
	RBTree_init(&T);

	
	return 0;
}
