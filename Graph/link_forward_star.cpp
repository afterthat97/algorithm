#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 10005
#define maxm 100005
using namespace std;

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];
int np, ne, gsize, head[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	return 0;
}
