#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <queue>
#define inf 0x3f3f3f3f
#define maxn 1005
#define maxm 10005
using namespace std;

struct edge {
	int to, next, cap, flow;
}g[maxm];

int gsize, np, ne, d[maxn], cur[maxn], head[maxn]; 
bool vis[maxn];

void add_edge(int from, int to, int cap) {
	g[gsize] = (edge){ to, head[from], cap, 0 };
	head[from] = gsize++;
	g[gsize] = (edge){ from, head[to], 0, 0 };
	head[to] = gsize++;
}

bool BFS(int ps, int pe) {
	memset(vis, 0, sizeof vis);
	queue<int> q; q.push(ps);
	d[ps] = 0; vis[ps] = 1;
	while (!q.empty()) {
		int now = q.front(); q.pop();
		for (int i = head[now]; ~i; i = g[i].next)
			if (!vis[g[i].to] && g[i].cap > g[i].flow) {
				vis[g[i].to] = 1;
				d[g[i].to] = d[now] + 1;
				q.push(g[i].to);
			}
	}
	return vis[pe];
}

int DFS(int x, int a, int pe) {
	if (x == pe || a == 0) return a;
	int flow = 0;
	for (int &i = cur[x]; ~i; i = g[i].next) {
		if (d[x] + 1 == d[g[i].to]) {
			int f = DFS(g[i].to, min(a, g[i].cap - g[i].flow), pe);
			if (f > 0) {
				g[i].flow += f; g[i ^ 1].flow -= f;
				flow += f; a -= f;
				if (a == 0) break;
			}
		}
	}
	return flow;
}

int max_flow(int ps, int pe) {
	int flow = 0;
	while (BFS(ps, pe)) {
		for (int i = 0; i < np; i++)
			cur[i] = head[i];
		flow += DFS(ps, inf, pe);
	}
	return flow;
}

int main() {
	while (scanf("%d %d", &ne, &np) == 2) {
		memset(head, -1, sizeof head); gsize = 0;
		for (int i = 0; i < ne; i++) {
			int x, y, c;
			scanf("%d %d %d", &x, &y, &c);
			add_edge(x, y, c);
		}
		cout << max_flow(1, np) << endl;
	}
	return 0;
}
