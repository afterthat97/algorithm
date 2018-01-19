#include <iostream>
#include <stdio.h>
#include <queue>
#include <cstring>
#define maxn 10005
#define maxm 100005
#define INF 0x3f3f3f3f
#define fst first
#define snd second
using namespace std;

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0): to(t), next(n), val(v) {}
}g[maxm * 2];
typedef pair<int, int> node;
int np, ne, gsize, head[maxn], minc[maxn];
bool vis[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

int prim() {
	int all_cost = 0;
	memset(minc, INF, sizeof minc); minc[1] = 0;
	priority_queue<node, vector<node>, greater<node> > q;
	q.push(node(0, 1));//start from point 1
	while (!q.empty()) {
		node now = q.top(); q.pop();
		if (vis[now.snd] || now.fst > minc[now.snd]) continue;
		vis[now.snd] = 1; all_cost += minc[now.snd];
		for (int i = head[now.snd]; ~i; i = g[i].next)
			if (minc[g[i].to] > g[i].val) {
				minc[g[i].to] = g[i].val;
				q.push(node(g[i].val, g[i].to));
			}
	}
	return all_cost;
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	printf("%d\n", prim());
	return 0;
}
