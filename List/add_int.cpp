#include <iostream>
using namespace std;

typedef struct tag_node {
	int value;
	tag_node *next;
	tag_node(int v):value(v), next(NULL) {}
}node;

node* add(node* head1, node* head2) {
	node *sum = new node(0);
	node *tail = sum;
	node *p1 = head1->next;
	node *p2 = head2->next;
	node *cur;
	int carry = 0, value = 0;
	while (p1&&p2) {
		value = p1->value + p2->value + carry;
		carry = value / 10;
		value %= 10;
		cur = new node(value);
		tail->next = cur;
		tail = cur;
		p1 = p1->next;
		p2 = p2->next;
	}

	node *p = p1 ? p1 : p2;
	while (p) {
		value = p->value + carry;
		carry = value / 10;
		value %= 10;
		cur = new node(value);
		tail->next = cur;
		tail = cur;
		p = p->next;
	}

	if (carry)
		tail->next = new node(carry);
	return sum;
}

int main() {
	node *head1 = new node(0);
	for (int i = 0; i < 6; i++) {
		int n;
		cin >> n;
		node *p = new node(n);
		p->next = head1->next;
		head1->next = p;
	}
	node *head2 = new node(0);
	for (int i = 0; i < 8; i++) {
		int n;
		cin >> n;
		node *p = new node(n);
		p->next = head2->next;
		head2->next = p;
	}
	node *sum = add(head1, head2);
	cin.get(); cin.get();
	return 0;
}