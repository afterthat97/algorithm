#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>
#define MAXN 10005
#define MAXM 50005
using namespace std;

struct edge {
	int to, next;
	bool psd;
	edge(int t = 0, int n = 0): to(t), next(n) { psd = 0; }
}g[MAXM * 2];

int np, ne, head[MAXN], gsize;
stack<int> way;

void dfs(int now) {
	for (int i = head[now]; ~i; i = g[i].next)
		if (!g[i].psd) {
			g[i].psd = true;
			dfs(g[i].to);
		}
	way.push(now);
}

inline void add_edge(int px, int py) {
	g[gsize] = edge(py, head[px]);
	head[px] = gsize++;
}

int main() {
	scanf("%d %d", &np, &ne);
	memset(head, -1, sizeof head);
	for (int i = 0; i < ne; i++) {
		int px, py;
		scanf("%d %d", &px, &py);
		add_edge(px, py);
		//add_edge(py, px);
	}
	dfs(1);
	while (!way.empty()) {
		printf("%d\n", way.top());
		way.pop();
	}
	return 0;
}
