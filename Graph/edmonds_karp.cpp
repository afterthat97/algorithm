#include <iostream>
#include <stdio.h>
#include <queue>
#include <cstring>
#define min(a, b) ((a) > (b) ? (b) : (a))
#define INF 0x3f3f3f3f;
#define MAXN 105
using namespace std;

int np, ne, g[MAXN][MAXN];

int edmonds_karp(int ps, int pe) {
	int max_flow = 0, flow[MAXN] = {}, path[MAXN] = {};
	queue<int> q;
	while (1) {
		memset(flow, 0, sizeof flow); flow[ps] = INF;
		q.push(ps);
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (int v = 1; v <= np; v++)
				if (!flow[v] && g[u][v]) {
					path[v] = u;
					q.push(v);
					flow[v] = min(flow[u], g[u][v]);
				}
		}
		if (flow[pe] == 0) return max_flow;
		for (int p = pe; p != ps; p = path[p]) {
			g[path[p]][p] -= flow[pe];
			g[p][path[p]] += flow[pe];
		}
		max_flow += flow[pe];
	}
}

int main() {
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		g[px][py] += w;
	}
	printf("%d\n", edmonds_karp(1, np));
	return 0;
}
