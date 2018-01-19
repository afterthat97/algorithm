#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 10050
#define maxm 50050
using namespace std;

struct edge {
	int to, next;
} g[maxm], rg[maxm];

int vis[maxn], order[maxn], bel[maxn], cnt;
int np, ne, head[maxn], rhead[maxn], gsize, rgsize;

inline void add_edge(int from, int to) {
	g[gsize] = { to, head[from] };
	rg[rgsize] = { from, rhead[to] };
	head[from] = gsize++;
	rhead[to] = rgsize++;
}

void dfs(int v) {
	vis[v] = 1;
	for (int i = head[v]; ~i; i = g[i].next)
		if (!vis[g[i].to]) dfs(g[i].to);
	order[cnt++] = v;
}

void rdfs(int v, int k) {
	vis[v] = 1; bel[v] = k;
	for (int i = rhead[v]; ~i; i = rg[i].next)
		if (!vis[rg[i].to]) rdfs(rg[i].to, k);
}

int scc() {
	memset(vis, 0, sizeof vis); cnt = 0;
	for (int i = 0; i < np; i++)
		if (!vis[i]) dfs(i);
	memset(vis, 0, sizeof vis);
	int k = 0;
	for (int i = cnt - 1; ~i; i--)
		if (!vis[order[i]]) rdfs(order[i], k++);
	return k;
}

int main() {
	while (scanf("%d %d", &np, &ne) == 2) {
		memset(head, -1, sizeof head);
		memset(rhead, -1, sizeof rhead);
		gsize = rgsize = 0;
		for (int i = 0; i < ne; i++) {
			int px, py;
			scanf("%d %d", &px, &py);
			add_edge(px - 1, py - 1);
		}
		int ans = scc();
		printf("%d\n", ans);
	}
	return 0;
}
