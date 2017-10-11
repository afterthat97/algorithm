#include <iostream>
#include <stdio.h>
using namespace std;

struct node {
	node *l, *r;
	int val;
	node(int v = 0):val(v), l(0), r(0) {}
};

node* null_node = 0;//Don't use it

void ins(node* &root, int val) {
	if (root == 0) { root = new node(val); return; }
	if (val < root->val) ins(root->l, val);
	if (val > root->val) ins(root->r, val);
}

node* &find(node* &root, int val) {
	if (root == 0) { return null_node;}
	if (val < root->val) return find(root->l, val);
	if (val > root->val) return find(root->r, val);
	return root;
}

node* &getl(node* &root) { return (root->l == 0 ? root : getl(root->l)); }

void del(node* &root) {
	if (root->l == 0 && root->r == 0) {
		delete root; root = 0;
	} else if (root->l == 0) {
		node* tmp = root; root = root->r; delete tmp;
	} else if (root->r == 0) {
		node* tmp = root; root = root->l; delete tmp;
	} else {
		node* &tmp = getl(root->r); root->val = tmp->val; del(tmp);
	}
}

void mid_order(node* &root) {
	if (root == 0) return;
	mid_order(root->l);
	cout << root->val << ' ';
	mid_order(root->r);
}

int main() {
	//Attention!!!
	//node* &ans = find(root, val);
	//must define like this
	return 0;
}
