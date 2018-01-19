//POJ 1273
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#define maxn 205
#define maxm 405
#define inf 0x3f3f3f3f
using namespace std;

struct node {
    int to, next, cap;
}g[maxm << 1];
int gsize, head[maxn];
bool used[maxn];

inline void add_edge(int from, int to, int cap) {
    g[gsize] = {to, head[from], cap};
    head[from] = gsize++;
	g[gsize] = {from, head[to], 0};
	head[to] = gsize++;
}

int dfs(int v, int pe, int flow) {
	if (v == pe) return flow;
	used[v] = 1;
	for (int i = head[v]; ~i; i = g[i].next)
		if (!used[g[i].to] && g[i].cap > 0) {
			int d = dfs(g[i].to, pe, min(flow, g[i].cap));
			if (d > 0) {
				g[i].cap -= d; g[i ^ 1].cap += d;
				return d;
			}
		}
	return 0;
}

int max_flow(int ps, int pe) {
	int ans = 0, f = 0;
	memset(used, 0, sizeof used);
	while (f = dfs(ps, pe, inf)) {
		memset(used, 0, sizeof used);
		ans += f;
	}
	return ans;
}

int main() {
	int np, ne;
	while (scanf("%d %d", &ne, &np) == 2) {
		memset(head, -1, sizeof head); gsize = 0;
		for (int i = 0; i < ne; i++) {
			int px, py, w;
			scanf("%d %d %d", &px, &py, &w);
			add_edge(px, py, w);
		}
		printf("%d\n", max_flow(1, np));
	}
	return 0;
}
