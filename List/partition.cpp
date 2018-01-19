#include <iostream>
using namespace std;

typedef struct tag_node {
	int value;
	tag_node *next;
	tag_node(int v):value(v), next(NULL) {}
}node;

node* partition(node *head, int x) {
	node *l_head = new node(0), *r_head = new node(0), *p = head;
	node *l_tail = l_head, *r_tail = r_head;
	while (p = p->next) {
		if (p->value < x) {
			l_tail->next = p;
			l_tail = p;
		} else {
			r_tail->next = p;
			r_tail = p;
		}
	}
	r_tail->next = NULL;
	l_tail->next = r_head->next;
	return l_head;
}

int main() {
	int x;
	cin >> x;
	int dat[] = { 5,2,7,9,1,5,9,3,5,8,3,1 };
	int size = sizeof(dat) / sizeof(int);
	node *head = new node(0);
	for (int i = size - 1; i >= 0; i--) {
		node *p = new node(dat[i]);
		p->next = head->next;
		head->next = p;
	}

	head = partition(head, x);
	return 0;
}