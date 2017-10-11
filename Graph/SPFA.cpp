#include <iostream>
#include <stdio.h>
#include <queue>
#include <cstring>
#define inf 0x3f3f3f3f
#define maxn 105
#define maxm 100005
using namespace std;

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];
int ps, pe, np, ne, sp[maxn], gsize, head[maxn];
bool inq[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

void SPFA() {
	queue<int> q;
	memset(sp, inf, sizeof sp); sp[ps] = 0;
	q.push(ps); inq[ps] = 1;
	while (!q.empty()) {
		int now = q.front();
		for (int i = head[now]; ~i; i = g[i].next)
			if (sp[g[i].to] > sp[now] + g[i].val) {
				sp[g[i].to] = sp[now] + g[i].val;
				if (!inq[g[i].to]) { 
					q.push(g[i].to); inq[g[i].to] = 1; 
				}
			}
		q.pop(); inq[now] = 0;
	}
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d %d %d", &np, &ne, &ps, &pe);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	SPFA();
	return 0;
}
