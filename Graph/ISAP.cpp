#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <queue>
#define ll long long
#define INF 0x3f3f3f3f
#define MAXN 1005
#define min(a, b) ((a) > (b) ? (b) : (a))
using namespace std;

struct edge {
	int from, to, cap, flow;
	edge(int a, int b, int c, int d) {
		from = a; to = b; cap = c; flow = d;
	}
};
vector<edge> edges;
vector<int> g[MAXN];
int np, ne, ps, pe;
int d[MAXN], cur[MAXN], p[MAXN], num[MAXN];
//d[i]: 起点到i的距离; cur[i]: 当前弧下标; p[i]: 可增广路径上的一条弧; num[i]: 距离标号计数
bool vis[MAXN];

inline void add_edge(int from, int to, int cap) {
	edges.push_back(edge(from, to, cap, 0));
	edges.push_back(edge(to, from, 0, 0));
	int m = edges.size();
	g[from].push_back(m - 2);
	g[to].push_back(m - 1);
}

inline bool BFS() {
	memset(vis, 0, sizeof vis);
	queue<int> q;
	q.push(pe);
	d[pe] = 0;
	vis[pe] = 1;
	while (!q.empty()) {
		int now = q.front();
		q.pop();
		for (int i = 0; i < g[now].size(); i++) {
			edge& e = edges[g[now][i]];
			if (!vis[e.to] && int(e.cap) > e.flow) {
				vis[e.to] = 1;
				d[e.to] = d[now] + 1;
				q.push(e.to);
			}
		}
	}
	return vis[pe];
}

int augment() {
	int x = pe, a = INF;
	for (; x != ps; x = edges[p[x]].from) {
		edge& e = edges[p[x]];
		a = min(a, e.cap - e.flow);
	}
	for (x = pe; x != ps; x = edges[p[x]].from) {
		edges[p[x]].flow += a;
		edges[p[x] ^ 1].flow -= a;
	}
	return a;
}

inline int max_flow() {
	int flow = 0, x = ps;
	BFS();
	memset(num, 0, sizeof num);
	memset(cur, 0, sizeof cur);
	for (int i = 1; i <= np; i++) num[d[i]]++;
	while (d[ps] < np) {
		if (x == pe) {
			flow += augment();
			x = ps;
		}
		bool ok = 0;
		for (int& i = cur[x]; i < g[x].size(); i++) {
			edge& e = edges[g[x][i]];
			if (e.cap > e.flow && d[x] == d[e.to] + 1) {
				ok = 1;
				p[e.to] = g[x][i];
				x = e.to;
				break;
			}
		}
		if (!ok) {
			int m = np - 1;
			for (int i = 0; i < g[x].size(); i++) {
				edge& e = edges[g[x][i]];
				if (e.cap > e.flow)
					m = min(m, d[e.to]);
			}
			if (--num[d[x]] == 0) break; //gap
			num[d[x] = m + 1]++;
			cur[x] = 0;
			if (x != ps)x = edges[p[x]].from;
		}
	}
	return flow;
}

int main() {
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int x, y, c;
		scanf("%d %d %d", &x, &y ,&c);
		add_edge(x, y, c);
	}
	ps = 1;
	pe = np;
	printf("%d\n", max_flow());
	return 0;
}
