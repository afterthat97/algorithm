#include <iostream>
#include <stdio.h>
using namespace std;

struct Node {
	int val, height, size;
	Node *left, *right;
	Node(int v): val(v) {
		size = height = 1;
		left = right = NULL;
	}
};

int getHeight(Node* x) {
	return x ? x->height : 0;
}

int getSize(Node* x) {
	return x ? x->size : 0;
}

void rotate_left(Node *x) {
	Node *z = x->right, *t1 = x->left, *t23 = z->left, *t4 = z->right;
	x->left = z;  x->right = t4;
	z->left = t1; z->right = t23;
	swap(x->val, z->val);
	z->height = max(getHeight(z->left), getHeight(z->right)) + 1;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	z->size = getSize(z->left) + getSize(z->right) + 1;
	x->size = getSize(x->left) + getSize(x->right) + 1;
}

void rotate_right(Node *x) {
	Node *z = x->left, *t1 = z->left, *t23 = z->right, *t4 = x->right;
	x->left = t1;  x->right = z;
	z->left = t23; z->right = t4;
	swap(x->val, z->val);
	z->height = max(getHeight(z->left), getHeight(z->right)) + 1;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	z->size = getSize(z->left) + getSize(z->right) + 1;
	x->size = getSize(x->left) + getSize(x->right) + 1;
}

void maintain(Node* root) {
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	root->size = getSize(root->left) + getSize(root->right) + 1;
	if (getHeight(root->right) - getHeight(root->left) > 1) {
		Node* z = root->right;
		if (getHeight(z->right) - getHeight(z->left) < 0)
			rotate_right(z);
		rotate_left(root);
	} else if (getHeight(root->right) - getHeight(root->left) < -1) {
		Node* z = root->left;
		if (getHeight(z->right) - getHeight(z->left) > 0)
			rotate_left(z);
		rotate_right(root);
	}
}

void insert(Node*& root, int val) {
	if (root != NULL) {
		insert(val < root->val ? root->left : root->right, val);
		maintain(root);
	} else
		root = new Node(val);
}	

Node* query(Node* root, int val) {
	if (root == NULL) return NULL;
	if (val == root->val) return root;
	return val < root->val ? query(root->left, val) : query(root->right, val);
}

Node* getMin(Node* root) {
	if (root == NULL) return NULL;
	return root->left ? getMin(root->left) : root;
}

Node* getMax(Node* root) {
	if (root == NULL) return NULL;
	return root->right ? getMax(root->right) : root;
}

void del(Node*& root, int val) {
	if (root == NULL) return;
	if (val == root->val) {
		if (root->left == NULL && root->right == NULL) {
			delete root; root = NULL;
		} else if (root->left == NULL) {
			Node *tmp = root; root = root->right; delete tmp;
		} else if (root->right == NULL) {
			Node *tmp = root; root = root->left; delete tmp;
		} else {
			swap(root->val, getMin(root->right)->val);
			del(root->right, val);
			maintain(root);
		}
	} else {
		del(val < root->val ? root->left : root->right, val);
		maintain(root);
	}
}

int getLowerBound(Node* root, int val) { // numbers less than val
	if (root == NULL) return 0;
	if (val <= root->val) return getLowerBound(root->left, val);
	return getSize(root->left) + getLowerBound(root->right, val) + 1;
}

int getUpperBound(Node* root, int val) { // numbers not more than val
	if (root == NULL) return 0;
	if (val < root->val)  return getUpperBound(root->left, val);
	return getSize(root->left) + getUpperBound(root->right, val) + 1;
}

int getNum(Node* root, int k) { // k starts from 1
	if (root == NULL) return -1; // out of range
	if (root->left == NULL && root->right == NULL) return root->val;
	if (k == getSize(root->left) + 1) return root->val;
	if (k <= getSize(root->left))
		return getNum(root->left, k);
	else
		return getNum(root->right, k - getSize(root->left) - 1);
}

int main() {
	int n; cin >> n;
	Node* root = NULL;
	while (n--) {
		int opt, x;
		scanf("%d %d", &opt, &x);
		if (opt == 1) { // insert x
			insert(root, x);
		} else if (opt == 2) { // delete x
			del(root, x);
		} else if (opt == 3) { // get x's rank
			printf("%d\n", getLowerBound(root, x) + 1);
		} else if (opt == 4) { // get the element ranked x
			printf("%d\n", getNum(root, x));
		} else if (opt == 5) { // get x's predecessor
			int r = getLowerBound(root, x);
			printf("%d\n", getNum(root, r));
		} else { // get x's successor
			int r = getUpperBound(root, x);
			printf("%d\n", getNum(root, r + 1));
		}
	}
	return 0;
}
