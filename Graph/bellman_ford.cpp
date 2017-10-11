#include <iostream>
#include <cstring>
#include <stdio.h>
#define inf 0x3f3f3f3f
#define maxn 1005
#define maxm 10005
using namespace std;

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];
int ne, np, ps, pe, sp[maxn], gsize, head[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

int bellman_ford() {
	memset(sp, inf, sizeof sp); sp[ps] = 0;
	for (int i = 1; i < np; i++)
		for (int j = 0; j < np; j++)
			for (int k = head[j]; ~k; k = g[k].next)
				if (sp[g[k].to] > sp[j] + g[k].val)
					sp[g[k].to] = sp[j] + g[k].val;
	for (int j = 0; j < np; j++)
		for (int k = head[j]; ~k; k = g[k].next)
			if (sp[g[k].to] > sp[j] + g[k].val) {
				printf("Found negative weight cycle.\n");
				return -1;
			}
	return sp[pe];
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d %d %d", &np, &ne, &ps, &pe);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	printf("%d\n", bellman_ford());
	return 0;
}
