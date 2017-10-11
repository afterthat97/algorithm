#include <iostream>
using namespace std;

typedef struct tag_node {
	int value;
	tag_node *next;
	tag_node(int v):value(v),next(NULL){}
}node;

void delete_all_duplicate_node(node *head) {
	node *p = head;
	while (p->next) {
		bool dup = false;
		node *q = p->next;
		while (q->next && q->next->value == q->value) {
			dup = true;
			q = q->next;
		}
		if (dup)
			p->next = q->next;
		else
			p = p->next;
	}
}

int main() {
	int dat[] = { 2,3,3,5,7,8,8,8,9,9,10 };
	node *head = new node(0);
	int size = sizeof(dat) / sizeof(int);
	for (int i = size - 1; i >= 0; i--) {
		node *p = new node(dat[i]);
		p->next = head->next;
		head->next = p;
	}
	delete_all_duplicate_node(head);
	return 0;
}