#include <iostream>
using namespace std;

typedef struct tag_node {
	int value;
	tag_node *next;
	tag_node(int v):value(v),next(NULL){}
}node;

void reverse(node *head, int from, int to) {
	node *cur = head->next;
	int i;
	for (i = 0; i < from - 1; i++) {
		head = cur;
		cur = cur->next;
	}
	node *pre = cur;
	cur = cur->next;
	to--;
	node *next;
	for (; i < to; i++) {
		next = cur->next;
		cur->next = head->next;
		head->next = cur;
		pre->next = next;
		cur = next;
	}
}

int main() {
	node *head = new node(0);
	for (int i = 0; i < 10; i++) {
		int n;
		cin >> n;
		node *p = new node(n);
		p->next = head->next;
		head->next = p;
	}
	reverse(head, 4, 8);
	return 0;
}