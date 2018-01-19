#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#define maxn 1005
#define maxm 20005
using namespace std;

struct edge {
	int to, next;
}g[maxm];

int gsize, np, ne, head[maxn], match[maxn];
bool vis[maxn];

inline void add_edge(int from, int to) {
	g[gsize] = (edge){ to, head[from] };
	head[from] = gsize++;
}

bool dfs(int v) {
	if (vis[v]) return 0; else vis[v] = 1;
	for (int i = head[v]; ~i; i = g[i].next) {
		int t = g[i].to, w = match[t];
		if (w == -1 || dfs(w)) {
			match[v] = t; match[t] = v;
			return 1;
		}
	}
	return 0;
}

int bipartite_match() {
	int ans = 0;
	memset(match, -1, sizeof match);
	for (int i = 0; i <= np; i++)
		if (match[i] == -1) {
			memset(vis, 0, sizeof vis);
			if (dfs(i)) ans++;
		}
	return ans;
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py;
		scanf("%d %d", &px, &py);
		add_edge(px, py + np);
		add_edge(py + np, px);
	}
	np <<= 1;
	printf("%d\n", bipartite_match());
	return 0;
}
