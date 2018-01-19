#include <iostream>
using namespace std;

typedef struct tag_node {
	int value;
	tag_node *next;
	tag_node(int v):value(v), next(NULL) {}
}node;

void delete_duplicate_node(node *head) {
	node *p = head->next;
	while (p) {
		if (p->next && p->next->next && p->next->value == p->next->next->value)
			p->next = p->next->next;
		else
			p = p->next;
	}
}

int main() {
	node *head = new node(0);
	int dat[] = { 2,3,3,5,7,8,9,9,30,8,8,8,8 };
	int size = sizeof(dat) / sizeof(int);
	for (int i = size - 1; i >= 0; i--) {
		node *p = new node(dat[i]);
		p->next = head->next;
		head->next = p;
	}
	delete_duplicate_node(head);
	node *p = head->next;
	while (p != NULL) {
		cout << p->value << ' ';
		p = p->next;
	}
	return 0;
}
