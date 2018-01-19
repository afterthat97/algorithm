#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 200005
#define maxm 200005
#define fst first
#define snd second
#define MAX_LGN 18
using namespace std;

pair<int, int> st[MAX_LGN][1 << MAX_LGN];

inline void init_st(int* a, int n) { // a[0 ~ n-1]
	for (int j = 0; j < n; j++) st[0][j] = { a[j], j };
	for (int i = 1; (1 << i) <= n; i++)
		for (int j = 0; j <= n - (1 << i); j++)
			st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
}

inline int query(int l, int r) { // l, r: [0, n-1]
	int i = 0;
	while ((1 << (i + 1)) <= r - l + 1) i++;
	return min(st[i][l], st[i][r - (1 << i) + 1]).snd;
}

struct edge {
	int to, next;
} g[maxm << 1];

int head[maxn], gsize, np, ne;

inline void add_edge(int from, int to) {
	g[gsize] = { to, head[from] };
	head[from] = gsize++;
}

int vs[maxn * 2], depth[maxn * 2], id[maxn];

void dfs(int v, int pre, int dep, int& k) {
	id[v] = k; vs[k] = v; depth[k++] = dep;
	for (int i = head[v]; ~i; i = g[i].next)
		if (g[i].to != pre) {
			dfs(g[i].to, v, dep + 1, k);
			vs[k] = v; depth[k++] = dep;
		}
}

void init_lca(int root) {
	int k = 0;
	dfs(root, -1, 0, k);
}

inline int lca(int u, int v) {
	return vs[query(min(id[u], id[v]), max(id[u], id[v]))];
}

int main() {
	while (scanf("%d", &np) == 1 && np) {
		np++; ne = np - 1;
		memset(head, -1, sizeof head);
		for (int i = 1; i < np; i++) {
			int par; scanf("%d", &par);
			add_edge(i, par);
			add_edge(par, i);
		}
		init_lca(0);
		init_st(depth, np * 2 - 1);
		int m; scanf("%d", &m);
		for (int i = 0, last = 0; i < m; i++) {
			int px, py; scanf("%d %d", &px, &py);
			last = lca(px ^ last, py ^ last);
			printf("%d\n", last);
		}
	}
	return 0;
}
