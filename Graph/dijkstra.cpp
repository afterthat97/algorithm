#include <stdio.h>
#include <cstring>
#include <iostream>
#include <queue>
#define inf 0x3f3f3f3f
#define maxn 1005
#define maxm 10005
#define fst first
#define snd second
using namespace std;

int np, ne, ps, gsize, head[maxn], sp[maxn];
typedef pair<int, int> node;
struct edge {
    int to, cost, next;
    edge(int t = 0, int c = 0, int n = 0): 
		to(t), cost(c), next(n) {}
}g[maxm];

inline void add_edge(int px, int py, int d) {
    g[gsize] = edge(py, d, head[px]);
    head[px] = gsize++;
}

void dijkstra() {
    priority_queue<node, vector<node>, greater<node> > q;
    memset(sp, inf, sizeof sp); sp[ps] = 0;
    q.push(node(0, ps));
    while (!q.empty()) {
		node p = q.top(); q.pop();
		if (sp[p.snd] < p.fst) continue;
		for (int cnt = p.snd, i = head[p.snd]; ~i; i = g[i].next) 
			if (sp[g[i].to] > sp[cnt] + g[i].cost) {
				sp[g[i].to] = sp[cnt] + g[i].cost;
				q.push(node(sp[g[i].to], g[i].to));
			}
    }
}

int main() {
    memset(head, -1, sizeof head);
	scanf("%d %d %d", &np, &ne, &ps);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
		//add_edge(py, px, w);
	}
	dijkstra();
	return 0;
}
