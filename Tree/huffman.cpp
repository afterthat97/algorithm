#include <iostream>
#include <queue>
#include <cstdio>
using namespace std;

struct node {
    int val;
    node *l, *r;
    node(int v = 0): val(v), l(0), r(0) {}
};

struct cmp { 
	bool operator() (node* a, node* b) { return a->val > b->val; }
};

priority_queue<node*, vector<node*>, cmp> q;

node* huffman() {
    node* cur = NULL;
    while (q.size() > 1) {
        cur = new node;
        cur->l = q.top(); q.pop();
        cur->r = q.top(); q.pop();
        cur->val = cur->l->val + cur->r->val;
        q.push(cur);
    }
    return q.top();
}

int main() {
    int n; scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int v; scanf("%d", &v);
        q.push(new node(v));
    }
    node *root = huffman();
    return 0;
}

