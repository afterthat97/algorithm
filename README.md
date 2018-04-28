<style type="text/css">
p, blockquote, ul, ol, dl, li, table {
font-family: Menlo;
font-size: 12px;
line-height: 1.5;
}
pre {
line-height: 1.3
}
@media print {
	table, pre {
		page-break-inside: auto;
	}
	pre {
		word-wrap: break-word;
	}
}
code[class*="language-"] {
	text-shadow: 0 1px white;
	font-family: Menlo;
	font-size: 11px;
}
</style>

<center>
# 2017 ACM-ICPC 区域赛
## 标准算法模板库

<img src = "/Users/afterthat97/Pictures/icpc.jpg" width = 80%>

</center>

<div style="page-break-after: always;"></div>

## 图论

* 存储结构
 * 链式向前星
* 单源最短路
 * Bellman-Ford
 * Dijkstra
 * SPFA
* 最小生成树
 * Kruskal
 * Prim
* 并查集
* 拓扑排序
* 网络流
 * Ford-fulkerson
 * Edmonds-Karp
 * Dinic
 * ISAP
* 费用流
* 二分图匹配
* 强连通分量
 * Tarjan
 * kosaraju (scc)
* 欧拉路

<div style="page-break-after: always;"></div>

### 存储结构 - 链式向前星

-----

定义：

```cpp
#define maxn 10005
#define maxm 100005

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];

int np, ne, gsize, head[maxn];
```

添加边：

```cpp
inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}
```

初始化：

```
memset(head, -1, sizeof head);
```

访问以`p`为起点的所有边：

```
for (int i = head[p]; ~i; i = g[i].next)
```

<div style="page-break-after: always;"></div>

### 最短路 - Bellman-Ford

-------

数据结构使用链式向前星，时间复杂度：$O(NE)$，可以检测负环。

```cpp
#define inf 0x3f3f3f3f
#define maxn 1005
#define maxm 10005

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];
int ne, np, ps, pe, sp[maxn], gsize, head[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

int bellman_ford() {
	memset(sp, inf, sizeof sp); sp[ps] = 0;
	for (int i = 1; i < np; i++)
		for (int j = 0; j < np; j++)
			for (int k = head[j]; ~k; k = g[k].next)
				if (sp[g[k].to] > sp[j] + g[k].val)
					sp[g[k].to] = sp[j] + g[k].val;
	for (int j = 0; j < np; j++)
		for (int k = head[j]; ~k; k = g[k].next)
			if (sp[g[k].to] > sp[j] + g[k].val) {
				printf("Found negative weight cycle.\n");
				return -1;
			}
	return sp[pe];
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d %d %d", &np, &ne, &ps, &pe);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	printf("%d\n", bellman_ford());
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 最短路 - Dijkstra

--------

数据结构使用链式向前星，时间复杂度：$O(ElogN)$

```cpp
#define inf 0x3f3f3f3f
#define maxn 1005
#define maxm 10005
#define fst first
#define snd second

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
```

<div style="page-break-after: always;"></div>

### 最短路 - SPFA

-------

数据结构使用链式向前星，时间复杂度：未知

```cpp
#define inf 0x3f3f3f3f
#define maxn 105
#define maxm 100005

struct edge {
	int to, next, val;
	edge(int t = 0, int n = 0, int v = 0):
		to(t), next(n), val(v) {}
}g[maxm * 2];
int ps, pe, np, ne, sp[maxn], gsize, head[maxn];
bool inq[maxn];

inline void add_edge(int from, int to, int val) {
	g[gsize] = edge(to, head[from], val);
	head[from] = gsize++;
}

void SPFA() {
	queue<int> q;
	memset(sp, inf, sizeof sp); sp[ps] = 0;
	q.push(ps); inq[ps] = 1;
	while (!q.empty()) {
		int now = q.front();
		for (int i = head[now]; ~i; i = g[i].next)
			if (sp[g[i].to] > sp[now] + g[i].val) {
				sp[g[i].to] = sp[now] + g[i].val;
				if (!inq[g[i].to]) { 
					q.push(g[i].to); inq[g[i].to] = 1; 
				}
			}
		q.pop(); inq[now] = 0;
	}
}

int main() {
	memset(head, -1, sizeof head);
	scanf("%d %d %d %d", &np, &ne, &ps, &pe);
	for (int i = 0; i < ne; i++) {
		int px, py, w;
		scanf("%d %d %d", &px, &py, &w);
		add_edge(px, py, w);
	}
	SPFA();
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 最小生成树 - Kurskal

------

时间复杂度：$O(ElogE)$，并查集已加入状态压缩和`rank`优化。

```cpp
struct edge {
	int x, y, w;
}g[maxm];
int np, ne, par[maxn], rk[maxn];

bool cmp(edge& a, edge& b) { return a.w < b.w; }

inline int getfa(int x) {
	int fx = x, tmp;
	while (fx != par[fx]) fx = par[fx];
	while (x != fx) {//compress condition
		tmp = par[x];
		par[x] = fx;
		x = tmp;
	}
	return fx;
}

inline void combine(int x, int y) {
	if (rk[x = getfa(x)] > rk[y = getfa(y)])
		par[y] = x;
	else {
		par[x] = y;
		if (rk[x] == rk[y]) rk[y]++;
	}
}

int main() {
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < np; i++) par[i] = i;
	for (int i = 0; i < ne; i++)
		scanf("%d %d %d", &g[i].x, &g[i].y, &g[i].w);
	sort(g, g + ne, cmp);
	int cnt = 0, all_cost = 0;
	for (int i = 0; i < ne && cnt < np - 1; i++) {
		int fx = getfa(g[i].x), fy = getfa(g[i].y);
		if (fx != fy) { //Add edge (xp, yp)
			combine(fx, fy);
			cnt++;
			all_cost += g[i].w;
		}
	}
	printf("%d\n", all_cost);
	return 0;
}
```

<div style="page-break-after: always;"></div>


### 最小生成树 - Prim

------

数据结构使用链式向前星，时间复杂度：$O(ElogN)$

```cpp
#define INF 0x3f3f3f3f
#define fst first
#define snd second

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
	printf("%d\n", prim()); return 0;
}
```

<div style="page-break-after: always;"></div>

### 并查集

-------

状态压缩 + `rank`优化

```cpp
int par[maxn], rk[maxn];

void init() {
	memset(rk, 0, sizeof rk);
	for (int i = 0; i < maxn; i++) par[i] = i;
}

inline int getfa(int x) {
	int fx = x, tmp;
	while (fx != par[fx]) fx = par[fx];
	while (x != fx) {//compress condition
		tmp = par[x];
		par[x] = fx;
		x = tmp;
	}
	return fx;
}

inline void combine(int x, int y) {
	if (rk[x = getfa(x)] > rk[y = getfa(y)])
		par[y] = x;
	else {
		par[x] = y;
		if (rk[x] == rk[y]) rk[y]++;
	}
}
```


<div style="page-break-after: always;"></div>

### 拓扑排序

-----

时间复杂度：$O(N + E)$

```cpp
vector<int> g[maxn];
int cnt, np, ne, deg[maxn], order[maxn];

void topologic() {
	queue<int> q;
	for (int i = 0; i < np; i++)
		if (deg[i] == 0) q.push(i);
	while (!q.empty()) {
		int now = q.front(); q.pop();
		order[cnt++] = now;
		for (int i = 0; i < g[now].size(); i++)
			if ((--deg[g[now][i]]) == 0) 
				q.push(g[now][i]);
	}
	if (cnt < np) printf("Failed.\n");
}

int main() {
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py;
		scanf("%d %d", &px, &py);
		g[px].push_back(py);
		deg[py]++;
	}
	topologic();
	for (int i = 0; i < np; i++)
		printf("%d ", order[i]);
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 网络流 - Ford-fulkerson

------

```cpp
#define maxn 205
#define maxm 405
#define inf 0x3f3f3f3f

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
```

<div style="page-break-after: always;"></div>


### 网络流 - Edmonds-Karp

------

时间复杂度： $O(NE^2)$

```cpp
int np, ne, g[maxn][maxn], flow[maxn], path[maxn];

int edmonds_karp(int ps, int pe) {
	int max_flow = 0; queue<int> q;
	while (1) {
		memset(flow, 0, sizeof flow); 
		flow[ps] = INF; q.push(ps);
		while (!q.empty()) {
			int u = q.front(); q.pop();
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
	printf("%d\n", edmonds_karp(1, np));
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 网络流 - Dinic

-------

时间复杂度：$O(EN^2)$

```cpp
struct edge {
	int from, to, cap, flow;
	edge(int a, int b, int c, int d) {
		from = a; to = b; cap = c; flow = d;
	}
};
vector<edge> edges;
vector<int> g[maxn];
int np, ne, ps, pe, d[maxn], cur[maxn];
//d[i]: 起点到i的距离; cur[i]: 当前弧下标.
bool vis[maxn];

void add_edge(int from ,int to, int cap) {
	edges.push_back(edge(from, to, cap, 0));
	edges.push_back(edge(to, from, 0, 0));
	int m = edges.size();
	g[from].push_back(m - 2);
	g[to].push_back(m - 1);
}

bool BFS() {
	memset(vis, 0, sizeof vis);
	queue<int> q;
	q.push(ps); d[ps] = 0; vis[ps] = 1;
	while (!q.empty()) {
		int now = q.front(); q.pop();
		for (int i = 0; i < g[now].size(); i++) {
			edge& e = edges[g[now][i]];
			if (!vis[e.to] && int(e.cap) > e.flow) {
				q.push(e.to); vis[e.to] = 1;
				d[e.to] = d[now] + 1;
			}
		}
	}
	return vis[pe];
}

int DFS(int x, int a) {
	if (x == pe || a == 0) return a;
	int flow = 0, f;
	for (int &i = cur[x]; i < g[x].size(); i++) {
		edge& e = edges[g[x][i]];
		if (d[x] + 1 == d[e.to] && 
		(f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
			e.flow += f;
			edges[g[x][i] ^ 1].flow -= f;
			flow += f; a -= f;
			if (a == 0) break;
		}
	}
	return flow;
}

int max_flow() {
	int flow = 0;
	while (BFS()) {
		memset(cur, 0, sizeof cur);
		flow += DFS(ps, INF);
	}
	return flow;
}

int main() {
	//add_edge(x, y, c);
	ps = 1; pe = np;
	cout << max_flow() << endl;
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 网络流 - ISAP

-------

时间复杂度：$O(EN^2)$

```cpp
struct edge {
	int from, to, cap, flow;
	edge(int a, int b, int c, int d) {
		from = a; to = b; cap = c; flow = d;
	}
};
vector<edge> edges;
vector<int> g[maxn];
int np, ne, ps, pe;
int d[maxn], cur[maxn], p[maxn], num[maxn];
//d[i]: 起点到i的距离; cur[i]: 当前弧下标; 
//p[i]: 可增广路径上的一条弧; num[i]: 距离标号计数
bool vis[maxn];

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
	q.push(pe); d[pe] = 0; vis[pe] = 1;
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
		if (x == pe) { flow += augment(); x = ps; }
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
	//add_edge(x, y, c);
	ps = 1; pe = np;
	printf("%d\n", max_flow());
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 费用流

------

```cpp
#define maxv 500050
#define inf 0x3f3f3f3f

typedef pair<int, int> P;

struct edge {
	int to, next, cap, cost;
}g[maxv << 1];

int h[maxv], dis[maxv], prevv[maxv], preve[maxv], head[maxv], val[maxv];
int gsize, V, np, ne, ST, ED;

inline void add_edge(int from, int to, int cap, int cost) {
    g[gsize] = {to, head[from], cap, cost};
    head[from] = gsize++;
	g[gsize] = {from, head[to], 0, -cost};
	head[to] = gsize++;
}

int min_cost_flow(int s, int t, int f) {
    int res = 0;
    memset(h, 0, sizeof h);
    while (f > 0) {
        priority_queue<P, vector<P>, greater<P> > que;
        memset(dis, 0x3f, sizeof dis);
        dis[s] = 0;
        que.push(P(0, s));
        while (!que.empty()) {
            P p = que.top(); que.pop();
            int v = p.second;
            if (dis[v] < p.first) continue;
            for (int i = head[v]; ~i; i = g[i].next)
                if (g[i].cap > 0 && dis[g[i].to] > dis[v] + g[i].cost + h[v] - h[g[i].to]) {
                    dis[g[i].to] = dis[v] + g[i].cost + h[v] - h[g[i].to];
                    prevv[g[i].to] = v;
                    preve[g[i].to] = i;
                    que.push(P(dis[g[i].to], g[i].to));
                }
        }
        if (dis[t] == inf) return -1;
        for (int v = 0; v < V; v++) h[v] += dis[v];
        int d = f;
        for (int v = t; v != s; v = prevv[v])
            d = min(d, g[preve[v]].cap);
        f -= d; res += d * h[t];
        for (int v = t; v != s; v = prevv[v]) {
            g[preve[v]].cap -= d;
            g[preve[v] ^ 1].cap += d;
        }
    }
    return res;
}

int main() {
    int T; cin >> T;
    while (T--) {
		gsize = 0;
        memset(h, 0, sizeof h);
        memset(prevv, 0, sizeof prevv);
        memset(preve, 0, sizeof preve);
		memset(head, -1, sizeof head);
		scanf("%d", &np); ne = np - 1;
		for (int i = 1; i <= np; i++)
			scanf("%d", &val[i]);
        for (int i = 0; i < ne; i++) {
			int px, py, w;
			scanf("%d %d %d", &px, &py, &w);
			add_edge(px, py, 1, w);
			add_edge(py, px, 1, w);
        }
		for (int i = 1; i <= np; i++) {
			add_edge(ST, i, 1, val[i]);
			add_edge(i, ED, 1, 10000 - val[i]);
		}
        V = ED + 1;
        cout << 10000 - min_cost_flow(ST, ED, 1) << endl;
    }
    return 0;
}
```

<div style="page-break-after: always;"></div>

### 二分图匹配

------

```cpp
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
```

<div style="page-break-after: always;"></div>

### 强连通分量 - Tarjan

------

时间复杂度：$O(N + E)$

```cpp
stack<int> s;
vector<int> g[maxn], gnew[maxn];
bool visited[maxn], instack[maxn];
int DFN[maxn], low[maxn], belong[maxn], ne, np, idx, cnt;

void tarjan(int p) {
	idx++;
	DFN[p] = low[p] = idx;
	s.push(p); visited[p] = instack[p] = 1;

	for (int i = 0; i < g[p].size(); i++)
			if (!visited[g[p][i]]) {
				tarjan(g[p][i]);
				low[p] = min(low[p], low[g[p][i]]);
			} else if (instack[g[p][i]])
				low[p] = min(low[p], DFN[g[p][i]]);

	if (DFN[p] == low[p]) {
		cnt++; int j;
		do {
			j = s.top(); s.pop();
			belong[j] = cnt;
			instack[j] = 0;
		} while (j != p);
	}
}

int main() {
	scanf("%d %d", &np, &ne);
	for (int i = 0; i < ne; i++) {
		int px, py;
		scanf("%d %d", &px, &py);
		g[px].push_back(py);
	}
	
	for (int i = 0; i < np; i++)
		if (!visited[i]) tarjan(i);
		
	for (int i = 0; i < np; i++)
		for (int j = 0; j < g[i].size(); j++)
			if (belong[i] != belong[g[i][j]])
				gnew[belong[i]].push_back(belong[g[i][j]]);
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 强连通分量 - kosaraju

------

```cpp
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
```

<div style="page-break-after: always;"></div>


### 欧拉路

-------

数据结构使用链式向前星，需要添加`bool psd;`属性。时间复杂度：$O(E)$

```cpp
int np, ne, head[maxn], gsize;
stack<int> way;

void dfs(int now) {
	for (int i = head[now]; ~i; i = g[i].next)
		if (!g[i].psd) {
			g[i].psd = true;
			dfs(g[i].to);
		}
	way.push(now);
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
		printf("%d ", way.top()); way.pop();
	}
	return 0;
}
```


<div style="page-break-after: always;"></div>


## 数据结构

* 建立二叉树
* 二叉搜索树
* 二叉堆
* 哈夫曼树
* RMQ & RSQ 问题
 * 稀疏表
 * 平方分割
 * 树状数组BIT
 * 线段树
* LCA
* K 元逆序对
* 主席树
* 归并树

<div style="page-break-after: always;"></div>

### 建立二叉树

-----

根据前序和中序遍历，或者中序和后序遍历来建立二叉树，时间复杂度 $O(NlogN)$。

**注意**：每次建树之前，需要初始化`l`, `r`, 和`cnt`。

```cpp
int pre[maxn], mid[maxn], suc[maxn];
int tree[maxn], l[maxn], r[maxn];
int cnt, n, temp;

void build1(int a, int b, int &idx) {
	tree[++idx] = pre[++cnt];
	int now = idx, t = -1;
	if (a == b) {
		if (mid[a] != tree[idx]) {/*Build failed*/}
		return;
	}
	for (int i = a; i <= b; i++)
		if (mid[i] == tree[idx]) { 
			t = i; break; 
		}
	if (t == -1) {/*Build failed*/}
	if (t > a) { 
		l[now] = idx + 1; 
		build1(a, t - 1, idx); 
	}
	if (t < b) { 
		r[now] = idx + 1; 
		build1(t + 1, b, idx); 
	}
}

void build2(int a, int b, int &idx) {
	tree[++idx] = suc[--cnt];
	int now = idx, t = -1;
	if (a == b) {
		if (mid[a] != tree[idx]) {/*Build failed*/}
		return;
	}
	for (int i = a; i <= b; i++)
		if (mid[i] == tree[idx]) { 
			t = i; break; 
		}
	if (t == -1) {/*Build failed*/}
	if (t < b) { 
		r[now] = idx + 1; 
		build2(t + 1, b, idx); 
	}
	if (t > a) { 
		l[now] = idx + 1; 
		build2(a, t - 1, idx); 
	}
}

int main() {
	memset(l, -1, sizeof l); 
	memset(r, -1, sizeof r);
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) 
		scanf("%d", &suc[i]);
	for (int i = 1; i <= n; i++) 
		scanf("%d", &mid[i]);
	
	cnt = 0;
	build1(1, n, temp = 0);
	
	cnt = n + 1;
	build2(1, n, temp = 0);
	
	return 0;
}
```


<div style="page-break-after: always;"></div>

### 二叉搜索树

------


插入、查找、删除操作时间复杂度均为$O(logN)$

**注意：**查找操作必须如此调用：

```
node* &ans = find(root, val);
```

```cpp
struct node {
	node *l, *r;
	int val;
	node(int v = 0):val(v), l(0), r(0) {}
};
node* null_node = 0;//Don't use it

void ins(node* &root, int val) {
	if (root == 0) { root = new node(val); return; }
	if (val < root->val) ins(root->l, val);
	if (val > root->val) ins(root->r, val);
}

node* &find(node* &root, int val) {
	if (root == 0) { return null_node;}
	if (val < root->val) return find(root->l, val);
	if (val > root->val) return find(root->r, val);
	return root;
}

node* &getl(node* &root) { 
	return (root->l == 0 ? root : getl(root->l));
}

void del(node* &root) {
	if (root->l == 0 && root->r == 0) {
		delete root; root = 0;
	} else if (root->l == 0) {
		node* tmp = root; root = root->r; delete tmp;
	} else if (root->r == 0) {
		node* tmp = root; root = root->l; delete tmp;
	} else {
		node* &tmp = getl(root->r); 
		root->val = tmp->val; 
		del(tmp);
	}
}
```

<div style="page-break-after: always;"></div>

### 二叉堆

-------

大根堆，插入、删除操作时间复杂度均为$O(logN)$。

```cpp
int heap[maxn + 1], size, n;//heap[]: [1, size]

inline void up(int x) {
	int fa = x >> 1, tmp = heap[x];
	while (fa) {
		if (tmp > heap[fa]) heap[x] = heap[fa];//cmp
		else break;
		x = fa; fa = x >> 1;
	}
	heap[x] = tmp;
}

inline void down(int x) {
	int ch = x << 1, tmp = heap[x];
	while (ch <= size) {
		if (ch < size && heap[ch + 1] > heap[ch]) ch++;//cmp
		if (heap[ch] > tmp) heap[x] = heap[ch];//cmp
		else break;
		x = ch; ch = x << 1;
	}
	heap[x] = tmp;
}

inline void push(int val) { heap[++size] = val; up(size); }
inline int top() { return heap[1]; }
inline void pop() { heap[1] = heap[size--]; down(1); }
void build() {
	size = n;
	for (int i = n; i > 0; i--) down(i);
}
```

<div style="page-break-after: always;"></div>

### 哈夫曼树

------

建树时间复杂度：$O(NlogN)$

```cpp
struct node {
    int val;
    node *l, *r;
    node(int v = 0): val(v), l(0), r(0) {}
};
struct cmp { 
	bool operator() (node* a, node* b) { 
		return a->val > b->val; 
	}
};
priority_queue<node*, vector<node*>, cmp> q;

node* huffman() {
    node* cur = NULL;
    while (q.size() > 1) {
        cur = new node;
        cur->l = q.top(); q.pop();
        cur->r = q.top(); q.pop();
        cur->val = cur->l->val + cur->r->val;
        q.push(cur);
    }
    return q.top();
}

int main() {
    //q.push(new node(v));
    node *root = huffman();
    return 0;
}
```


<div style="page-break-after: always;"></div>

### RMQ & RSQ - 平方分割

-----

| 空间   | 预处理 | 查询 | 更新 | 
|---------|-------|-------|------|
| $O(N)$ | $O(N)$| $O(\sqrt{N})$|$O(\sqrt{N})$|

示例：区间加，区间求和。

```cpp
ll sum[450], addv[450], a[200005];
int size, n, m;

inline void maintain(int idx, int k, int v) {
	a[k] += v;
	sum[idx] += v;
}

inline void add(int l, int r, int v) {//0 <= l, r < n
    int idx1 = l / size, idx2 = r / size, k;
    if (idx1 == idx2) {
        for (k = l; k <= r; ++k) maintain(idx1, k, v);
        return;
    }
    for (k = idx1 + 1; k < idx2; ++k) addv[k] += v;
    for (k = l; k < (idx1 + 1) * size; ++k) 
    	maintain(idx1, k, v);
    for (k = idx2 * size; k <= r; ++k) 
    	maintain(idx2, k, v);
}

inline ll query(int l, int r) {//0 <= l, r < n
    int idx1 = l / size, idx2 = r / size, k;
	ll ans = 0;
    if (idx1 == idx2) {
        for (k = l; k <= r; ++k) ans += a[k] + addv[idx1];
		return ans;
    }
    for (k = idx1 + 1; k < idx2; ++k) 
    	ans += addv[k] * size + sum[k];
    for (k = l; k < (idx1 + 1) * size; ++k) 
    	ans += a[k] + addv[idx1];	
	for (k = idx2 * size; k <= r; ++k) 
		ans += a[k] + addv[idx2];
	return ans;
}
```

<div style="page-break-after: always;"></div>

#### RMQ & RSQ - 稀疏表

-----

| 空间   | 预处理 | 查询 | 更新 | 
|---------|-------|-------|------|
| $O(N\log{N})$| $O(N\log{N})$ | $O(1)$| $O(N\log{N})$|

```cpp
#define MAX_LGN 15
int st[MAX_LGN][1 << MAX_LGN], a[maxn], n, m;

void init() {
	for (int j = 0; j < n; j++) st[0][j] = a[j];
	for (int i = 1; (1 << i) <= n; i++)
		for (int j = 0; j <= n - (1 << i); j++)
			st[i][j] = min(st[i - 1][j], 
				st[i - 1][j + (1 << (i - 1))]);
}

inline int query(int l, int r) {
	int i = 0;
	while ((1 << (i + 1)) <= r - l + 1) i++;
	return min(st[i][l], st[i][r - (1 << i) + 1]);
}
```

<div style="page-break-after: always;"></div>

#### RMQ & RSQ - 树状数组

------

| 空间   | 预处理 | 查询 | 更新 | 
|---------|-------|-------|------|
| $O(N)$| $O(N\log{N})$| $O(\log{N})$| $O(\log{N})$|

```cpp
int tree[maxn], n;

inline int sum(int x) {
	int ans = 0;
	for (; x > 0; x -= (x & -x))
		ans += tree[x];
	return ans;
}

inline void add(int x, int val) {
	for (; x <= n; x += (x & -x))
		tree[x] += val;
}
```

<div style="page-break-after: always;"></div>

### RMQ & RSQ - 线段树

------

| 空间   | 预处理 | 查询 | 更新 | 
|---------|-------|-------|------| 
|$O(N)$|$O(N)$|$O(\log{N})$|$O(\log{N})$| 

* 对于建树，首先需要找到一个最小的正整数$n'$，满足：$n' >= n$和$n'$为2的幂。于是$2n'-1$即为整个线段树的结点数量。其中1号结点为`root`，$[1, n')$为非叶子结点，$[n', 2n')$为叶子结点。
实际有效的叶子结点在$[n', n' + n)$中，$[n' + n, 2n')$为无效结点。这些结点的维护信息不能影响到有效结点，比如应该置其`sumv = 0`, `maxv = -INF`, `minv = INF`。建树时间复杂度$O(n)$。
* 仅在`flag()`和`push_down()`操作中可以对结点的标记进行修改。如果是双标记或者是多标记线段树，则所有标记都需要考虑在内。`push_down()`操作还需要考虑两个子树。
* 仅在`maintain()`中更新结点维护的值（比如`sumv`,`maxv`,`minv`等），维护的信息要能在接近$O(1)$时间内维护完成。要求先将旧值清零，再根据左右子树的维护的值和自身的标记来更新自身维护的值。
* 对于`update()`和`query()`操作，其维护的区间从0开始还是从1开始并不重要，但`root`结点必须为1号点，`query`操作同理。
 * 若$[a, b]$从0开始，则调用`update(1, 0, _n - 1, a, b ,v)`
 * 若$[a, b]$从1开始，则调用`update(1, 1, _n, a, b ,v)`

```cpp
struct node {
	ll setv, sumv;
	node(): sumv(0), setv(-INF) {}
};

int n, m, _n;
node* tree;

void init() {
	_n = 1;
	while (_n < n) _n <<= 1;
	tree = new node[_n << 1];
	for (int i = _n; i < _n + n; i++) {/*read tree[i]*/}
	for (int p = _n >> 1; p; p >>= 1)
		for (int i = p; i < (p << 1); i++) {
			int lt = i << 1, rt = (i << 1) + 1;
			//tree[i].sumv = tree[lt].sumv + tree[rt].sumv;
		}
}

inline void maintain(int k, int l, int r) {}
inline void push_down(int k) {}
inline void flag(int k, int v) {}

inline void update(int k, int l, int r, int a, int b, int v) {
	int lt = k << 1, rt = (k << 1) + 1;
	if (a <= l && r <= b)
		flag(k, v);
	else {
		push_down(k);
		int mid = (l + r) >> 1;
		if (a <= mid) update(lt, l, mid, a, b, v); 
			else maintain(lt, l, mid);
		if (mid < b) update(rt, mid + 1, r, a, b, v); 
			else maintain(rt, mid + 1, r);
	}
	maintain(k, l, r);
}

inline int query(int k, int l, int r, int a, int b) {
	if (r < a || l > b) return 0;//or INF, -INF
	//if flag...(e.g. setv)
	if (a <= l && r <= b) {}
	int mid = (r + l) >> 1, lt = k << 1, rt = (k << 1) + 1;
	return query(lt, l, mid, ans) + query(rt, mid + 1, r, ans);
}
```

<div style="page-break-after: always;"></div>


### LCA

------

```cpp
#define maxn 200005
#define maxm 200005
#define fst first
#define snd second
#define MAX_LGN 18

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
```

<div style="page-break-after: always;"></div>

### K 元逆序对

------

```cpp
#define maxn 20005
#define mod 1000000007

int tree[maxn], n, k;
int a[maxn], pre[maxn], cnt[maxn];

inline int sum(int x) {
	int ans = 0;
	for (; x > 0; x -= (x & -x))
		ans = (ans + tree[x]) % mod;
	return ans;
}

inline void add(int x, int val) {
	for (; x <= n; x += (x & -x))
		tree[x] = (tree[x] + val) % mod;
}

int main() {
	int T; cin >> T;
	for (int idx = 0; idx < T; idx++) {
		scanf("%d %d", &n, &k);
		for (int i = 0; i < n; i++) {
			scanf("%d", &a[i]);
			cnt[i] = 1;
		}
		for (int j = 0; j < k - 1; j++) {//des: cnt
			memset(tree, 0, sizeof tree);
			memcpy(pre, cnt, sizeof cnt);
			for (int i = n - 1; ~i; i--) {
				cnt[i] = sum(a[i]);
				add(a[i], pre[i]);
			}
		}
		int ans = 0;
		for (int i = 0; i < n; i++) 
			ans = (ans + cnt[i]) % mod;
		printf("%d\n", ans);
	}
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 主席树

------

```cpp
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#define fst first
#define snd second
#define MAXN 100005
#define MAXM 50005
using namespace std;

struct node {
	int l, r, num;
	node() {}
	node(int L, int R, int NUM) {
		l = L; r = R; num = NUM;
	}
}tree[MAXN << 5];

pair<int, int> raw[MAXN];
int root[MAXN], a[MAXN], n, m, cnt;

int ins(int pre, int pos) {
	int now = ++cnt;
	int l = 1, r = n, ret = now;
	while (l < r) {
		int mid = (l + r) >> 1;
		if (pos <= mid) {
			tree[now] = node(++cnt, tree[pre].r, tree[pre].num + 1);
			pre = tree[pre].l; r = mid;
		} else {
			tree[now] = node(tree[pre].l, ++cnt, tree[pre].num + 1);
			pre = tree[pre].r; l = mid + 1;
		}
		now = cnt;
	}
	tree[now] = node(0, 0, 1);
	return ret;
}

int query(int ql, int qr, int k) {
	int l = 1, r = n;
	while (l < r) {
		int mid = (l + r) >> 1;
		int num = tree[tree[qr].l].num - tree[tree[ql].l].num;
		if (k <= num) {
			ql = tree[ql].l; qr = tree[qr].l;
			r = mid;
		} else {
			ql = tree[ql].r; qr = tree[qr].r;
			l = mid + 1; k -= num;
		}
	}
	return l;
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &raw[i].fst);
		raw[i].snd = i;
	}
	sort(raw + 1, raw + n + 1);
	for (int i = 1; i <= n; i++)
		a[raw[i].snd] = i;
	for (int i = 1; i <= n; i++)
		root[i] = ins(root[i - 1], a[i]);
	for (int i = 0; i < m; i++) {
		int l, r, k;
		scanf("%d %d %d", &l, &r, &k);
		printf("%d\n", raw[query(root[l - 1], root[r], k)].fst);
	}
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 归并树

------

```cpp
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstring>
#define MAXN 100005
using namespace std;

int n, m, a[MAXN], xx, yy, cc, kth, flr[20][MAXN];
struct node {
	int l, r, f;
}tree[(1 << 18)];

void init(int k, int l, int r, int f) {
	if (r - l == 1)
		flr[f][l] = a[l];
	else {
		int lc = k << 1, rc = (k << 1) | 1, mid = (l + r) >> 1;
		init(lc, l, mid, f + 1);
		init(rc, mid, r, f + 1);
		merge(flr[f + 1] + tree[lc].l, flr[f + 1] + tree[lc].r,
			flr[f + 1] + tree[rc].l, flr[f + 1] + tree[rc].r, flr[f] + l);
	}
	tree[k].l = l; tree[k].r = r;
}

int query(int k, int l, int r, int f) {
	if (l >= yy || r <= xx) return 0;
	else if (xx <= l && r <= yy)
		return upper_bound(flr[f] + tree[k].l, flr[f] + tree[k].r, cc) 
			- (flr[f] + tree[k].l);
	else {
		int lc = k << 1, rc = (k << 1) | 1, mid = (l + r) >> 1;
		return query(lc, l, mid, f + 1) + query(rc, mid, r, f + 1);
	}
}

int st_find(int begin, int end, int k) {//segment tree
	xx = begin; yy = end; kth = k;
	int l = -1, r = n - 1;
	while (r - l > 1) {
		int mid = (l + r) >> 1;
		cc = flr[1][mid];
		if (query(1, 0, n, 1) >= kth) r = mid;
		else l = mid;
	}
	return flr[1][r];
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	init(1, 0, n, 1);
	for (int i = 0; i < m; i++) {
		int x, y, k;
		scanf("%d %d %d", &x, &y, &k);
		printf("%d\n", st_find(x - 1, y, k));
	}
	return 0;
}
```

<div style="page-break-after: always;"></div>

## 排序
* 快速排序
 * 随机快速排序
 * 中点快速排序
* 归并排序
* 基数排序
* Kth number
 * 固定区间单次查询
 * 不固定区间多次查询

<div style="page-break-after: always;"></div>

### 快速排序

------

当待排序数据为随机序列时，中点快速排序的执行效率高于随机快速排序。

```cpp
void qsort(int *begin, int *end) {
    if (end - begin <= 1) return;
    int key = *(begin + rand() % (end - begin - 1));
    int *i = begin, *j = end - 1;
    while (i <= j) {
        while (*i < key) i++;
        while (*j > key) j--;
        if (i <= j) swap(*(i++), *(j--));
    }
    qsort(begin, i);
    qsort(i, end);
}

void mid_qsort(int* begin, int* end) {//faster when random
    if (end - begin <= 1) return;
    int key = *(begin + ((end - begin - 1) >> 1));
    int *i = begin, *j = end - 1;
	while (i <= j) {
        while (*i < key) i++;
        while (*j > key) j--;
        if (i <= j) swap(*(i++), *(j--));
	}
    mid_qsort(begin, i);
    mid_qsort(i, end);
 }
```

<div style="page-break-after: always;"></div>

### 归并排序

-----

```cpp
int temp[maxn];

void merge_sort(int* a, int low, int high) {
	if (low >= high) return;
	int mid = (low + high) >> 1;
	merge_sort(a, low, mid);
	merge_sort(a, mid + 1, high);
	
	int i = low, j = mid + 1, size = 0;
	for (; (i <= mid) && (j <= high); size++)
		if (a[i] < a[j]) temp[size] = a[i++];
		else temp[size] = a[j++];
	memcpy(temp + size, a + i, (mid - i + 1) << 2);
	memcpy(a + low, temp, (size + mid - i + 1) << 2);
}
```

<div style="page-break-after: always;"></div>

### 基数排序

-------

选取的基数为65536。

```cpp
#define BASE (1 << 16)
#define maxn 10000005
int tmp[maxn], bkt[BASE + 5];

void radix_sort(int* begin, int* end) {
	int n = end - begin;
	for (int k = BASE - 1, i = 0; i < 2; i++, k <<= 16) {
		for (int j = 0; j < n; j++) 
			bkt[((*(begin + j)) & k) >> (i * 16)]++;
		for (int j = 1; j < BASE; j++) 
			bkt[j] += bkt[j - 1];
		for (int j = n - 1; j >= 0; j--)
			tmp[--bkt[((*(begin + j)) & k) >> (i * 16)]] = 
				*(begin + j);
		memcpy(begin, tmp, n * sizeof(int));
		memset(bkt, 0, sizeof bkt);
	}
}
```

<div style="page-break-after: always;"></div>

### Kth number

-----

* 若固定区间单次查询，使用基于快速排序的Kth算法。

* 若不固定区间多次查询，使用基于线段树的Kth算法。

```cpp
//find Kth smallest element, k: [1, n]; [begin, end)
int n, m, a[maxn];
int tmp[maxn], xx, yy, cc, kth;//[x, y], <= c
vector<int> tree[(1 << 18)];

void init(int k, int l, int r) {
	if (r - l == 1)
		tree[k].push_back(a[l]);
	else {
		int lc = k << 1, rc = (k << 1) + 1;
		init(lc, l, (l + r) >> 1);
		init(rc, (l + r) >> 1, r);
		tree[k].resize(r - l);
		merge(tree[lc].begin(), tree[lc].end(), 
tree[rc].begin(), tree[rc].end(), tree[k].begin());
	}
	if (k == 1) {
		memcpy(tmp, a, sizeof(a));
		sort(tmp, tmp + n);
	}
}

int query(int k, int l, int r) {
	if (l >= yy || r <= xx) return 0;
	else if (xx <= l && r <= yy)
		return upper_bound(tree[k].begin(), 
			tree[k].end(), cc) - tree[k].begin();
	else {
		int lc = k << 1, rc = (k << 1) + 1;
		return query(lc, l, (l + r) >> 1) + 
			query(rc, (l + r) >> 1, r);
	}
}

//不固定区间多次查询，基于线段树：初始化O(logn); 查询O((logn)^3)
int st_find(int begin, int end, int k) {//segment tree
	xx = begin; yy = end; kth = k;
	int l = -1, r = n - 1;
	while (r - l > 1) {
		int mid = (l + r) >> 1;
		cc = tmp[mid];
		if (query(1, 0, n) >= kth) r = mid;
		else l = mid;
	}
	return tmp[r];
}

//固定区间单次查询，基于快速排序: O(NlogN)
int qfind(int *begin, int *end, int k) {
    if (end - begin <= 1) return *begin;
    int key = *(begin + rand() % (end - begin - 1));
    //中点：int key = *(begin + ((end - begin - 1) >> 1));
    int *i = begin, *j = end - 1;
    while (i <= j) {
        while (*i < key) i++;
        while (*j > key) j--;
        if (i <= j) swap(*(i++), *(j--));
    }
    if (k <= i - begin)
        return qfind(begin, i, k);
    else
        return qfind(i, end, k - (i - begin));
}
```

<div style="page-break-after: always;"></div>

## 字符串
* 字符串匹配
 * KMP
 * Trie
 * AC-Automata
* 字符串循环左移
* 后缀数组

<div style="page-break-after: always;"></div>

### 字符串匹配 - KMP

-------

求解`f`的过程有一个优化：`f[i] = p[i] == p[j] ? f[j] : j;`

在利用`f`数组计算字符串周期时，只能使用原始转移方式：`f[i] = j;`

`t`为文本串，`p`为模式串。时间复杂度：$O(N + M)$。

```cpp
int f[maxn];

void getf(char *p) {
	int len = strlen(p);
	memset(f, -1, sizeof f);
	for (int i = 0, j = -1; i < len;) {
		while (~j && p[i] != p[j]) j = f[j];
		i++; j++;
		f[i] = p[i] == p[j] ? f[j] : j;
	}
}

int kmp(char *t, char *p) {
	int len = strlen(t), lenp = strlen(p);
	getf(p);
	for (int i = 0, j = 0; i < len;) {
		while (~j && t[i] != p[j]) j = f[j];
		i++; j++;
		if (j == lenp) return i - j + 1;
	}
	return -1;
}
```

<div style="page-break-after: always;"></div>

### 字符串匹配 - Trie 树

------

插入、查找时间复杂度 $O(len(s))$

```
#define maxn 4005
#define maxl 105
#define maxw 300005

int size, idx, trie[maxn * maxl][30], cnt[maxw];
bool flag[maxn * maxl];
char w[maxw];

void insert(char* s) {
    int p = 0, len = strlen(s);
    for (int i = 0; i < len; i++) {
        int c = s[i] - 'a';
        if (!trie[p][c])
            trie[p][c] = ++size;
        p = trie[p][c];
    }
    flag[p] = 1;
}

int main() {
    while (scanf("%s", w) == 1) {
        size = 0;
        memset(trie, 0, sizeof trie);
        memset(cnt, 0, sizeof cnt);
        memset(flag, 0, sizeof flag);
        int n; char s[maxl]; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%s", s);
            insert(s);
        }
        int len = strlen(w); cnt[len] = 1;
        for (int i = len - 1; ~i; i--) {
            int p = 0;
            for (int j = i; j < len; j++) {
                int c = w[j] - 'a';
                if (!trie[p][c]) break;
                p = trie[p][c];
                if (flag[p])
                    cnt[i] = (cnt[i] + cnt[j + 1]) % 20071027;
            }
        }
        printf("Case %d: %d\n", ++idx, cnt[0]);
    }
    return 0;
}
```

<div style="page-break-after: always;"></div>

### 字符串匹配 - AC 自动机

------

* `flag`数组仍用来判断结点是否为单词结点，使用`vector<int>`而非`bool`是为了防止当模式串重复时，后一个会覆盖前一个的情况，否则在统计出现次数时前一个模式串的出现次数将为 0。这样，`vector`中的内容即为该单词结点对应的编模式串编号（可能有多个编号，对应的模式串均相同）。
* `num`数组即用来统计：编号为`i`的模式串出现次数为`num[i]`次。
* 在`getf()`函数中，下面这条语句可以将树中不存在的边全部补上，这样在匹配时就不再需要失配函数：

```
if (u == 0) { trie[now][c] = trie[f[now]][c]; continue; }
```

* 在`find()`函数中，由于某个单词结点可能对应多个模式串的结尾，发现匹配时需要沿着失配边继续匹配其他模式串。这里有一个`last`优化，可以跳过沿路上的非单词结点，效果显著。

```
#define maxn 1005
#define maxl 55
#define maxw 2000010
#define maxnode maxn * maxl
#define sigma_size 26

int size, trie[maxnode][sigma_size], f[maxnode], last[maxnode], num[maxnode];
vector<int> flag[maxnode];
char p[maxn][maxl], t[maxw];

inline void init() {
    memset(num, 0, sizeof num);
    memset(trie, 0, sizeof trie);
    memset(last, 0, sizeof last);
    for (int i = 0; i < maxnode; i++) flag[i].clear();
    size = 0;
}

inline int idx(char ch) { return ch - 'A'; }

inline void insert(char *s, int i) {
    int p = 0, len = strlen(s);
    for (int i = 0; i < len; i++) {
        int c = idx(s[i]);
        if (!trie[p][c]) trie[p][c] = ++size;
        p = trie[p][c];
    }
    flag[p].push_back(i);
}

inline void find(char* t) {
    int len = strlen(t);
    for (int i = 0, j = 0; i < len; i++) {
        int c = idx(t[i]);
        if (c >= sigma_size || c < 0) { j = 0; continue; }
        j = trie[j][c];
        for (int tmp = j; tmp; tmp = last[tmp])
            for (int t = 0; t < flag[tmp].size(); t++)
                num[flag[tmp][t]]++;
    }
}

void getf() {
    queue<int> q;
    memset(f, 0, sizeof f);
    for (int c = 0; c < sigma_size; c++)
        if (trie[0][c]) q.push(trie[0][c]);

    while (!q.empty()) {
        int now = q.front(); q.pop();
        for (int c = 0; c < sigma_size; c++) {
            int u = trie[now][c];
            if (u == 0) { trie[now][c] = trie[f[now]][c]; continue; }
            q.push(u);
            int j = f[now];
            while (j && trie[j][c] == 0) j = f[j];
            f[u] = trie[j][c];
			last[u] = flag[f[u]].size() ? f[u] : last[f[u]];
        }
    }
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        init();
        for (int i = 1; i <= n; i++) { 
			scanf("%s", p[i]);
            insert(p[i], i);
        }
        getf();
        scanf("%s", t); find(t);
        for (int i = 1; i <= n; i++)
            if (num[i] > 0) printf("%s: %d\n", p[i], num[i]);
    }
    return 0;
}
```

<div style="page-break-after: always;"></div>

### 字符串循环左移

------

时间复杂度：$O(N)$。

```cpp
void rev_str(string &s, int from, int to) {
	while (from < to) {
		char t = s[from];
		s[from++] = s[to];
		s[to--] = t;
	}
}

string left_rotate_str(string s, int m) {
	int n = s.size();
	m %= n;
	rev_str(s, 0, m - 1);
	rev_str(s, m, n - 1);
	rev_str(s, 0, n - 1);
	return s;
}

int main() {
	string a = "abcdef";
	cout << left_rotate_str(a, 21);
}
```

<div style="page-break-after: always;"></div>

### 字符串 - 后缀数组

------

```cpp
#define maxn 20005

char str[maxn];
int sa[maxn], rk[maxn], t2[maxn], c[maxn], lcp[maxn];

void build_sa(int m = maxn) {
	int i, *x = rk, *y = t2, n = strlen(str);
	memset(c, 0, sizeof c);
	for (int i = 0; i < n; i++) c[x[i] = str[i]]++;
	for (int i = 1; i < m; i++) c[i] += c[i - 1];
	for (int i = n - 1;~i; i--) sa[--c[x[i]]] = i;
	for (int k = 1; k <= n; k <<= 1) {
		int p = 0;
		for (int i = n - k; i < n; i++) y[p++] = i;
		for (int i = 0; i < n; i++)
			if (sa[i] >= k) y[p++] = sa[i] - k;
		memset(c, 0, sizeof c);
		for (int i = 0; i < n; i++) c[x[y[i]]]++;
		for (int i = 1; i < m; i++) c[i] += c[i - 1];
		for (int i = n - 1;~i; i--) sa[--c[x[y[i]]]] = y[i];
		swap(x, y); p = 1; x[sa[0]] = 0;
		for (int i = 1; i < n; i++)
			if (y[sa[i - 1]] == y[sa[i]] && y[sa[i - 1] + k] == y[sa[i] + k])
				x[sa[i]] = p - 1;
			else
				x[sa[i]] = p++;
		if (p >= n) break;
		m = p;
	}
}

void build_lcp() {
	int len = strlen(str);
	for (int i = 0; i < len; i++) rk[sa[i]] = i;
	int h = 0; memset(lcp, 0, sizeof lcp);
	for (int i = 0; i < len; i++) 
		if (rk[i] > 0) {
			int j = sa[rk[i] - 1];
			if (h > 0) h--;
			for (; j + h < len && i + h < len; h++)
				if (str[j + h] != str[i + h]) break;
			lcp[rk[i] - 1] = h;
		}
}
```

<div style="page-break-after: always;"></div>

## 数学

* 素数
 * 判断素数
 * 求所有因子
 * 质因数分解
 * 筛法求素数
* 扩展欧几里得与逆元
* 快速幂
 * 整数快速幂
 * 矩阵快速幂
* 全排列
 * 递归版
 * 非递归版
* FFT
* 原根
* 线性筛

<div style="page-break-after: always;"></div>

### 素数

-------

| 判断素数 | 求所有因子 | 质因数分解 | 筛法求素数 | 
|---------|----------|----------|-----------| 
| $O(\log{N})$| $O(\log{N})$| $O(\log{N})$| $O(N\log{\log{N}})$| 

```cpp
ll prime[maxn];
int pnum;
bool notp[maxn];//e.g. notp[2] = 0, notp[4] = 1
bool notp_big[maxn];//for segment sieve

bool judge(ll x) {//判断素数
	for (ll i = 2; i * i <= x; i++)
		if (x % i == 0) return 0;
	return (x != 1) && (x != 0);
}

vector<ll> divisor(ll x) {//求所有因子
	vector<ll> ans;
	for (ll i = 2; i * i <= x; i++)
		if (x % i == 0) {
			ans.push_back(i);
			if (i != x / i) ans.push_back(x / i);
		}
	return ans;
}

map<ll, int> factor(ll x) {//质因数分解
	map<ll, int> ans;
	for (ll i = 2; i * i <= x; i++)
		while (x % i == 0) {
			ans[i]++;
			x /= i;
		}
	if (x != 1) ans[x]++;
	return ans;
}

int sieve(int x) {//筛法求[2, x)中素数
	memset(notp, 0, sizeof notp);
	pnum = 0;
	notp[0] = notp[1] = 1;
	for (int i = 2; i < x; i++)
		if (!notp[i]) {
			prime[pnum++] = (ll)i;
			for (int j = 2; i * j < x; j++)
				notp[i * j] = 1;
		}
	return pnum;
}

int segment_sieve(ll a, ll b) {//筛法求[a, b)中素数
	pnum = 0;
	memset(notp, 0, sizeof notp);
	memset(notp_big, 0, sizeof notp_big);
	//notp_big[x - a] == 0 -> x is prime
	for (int i = 2; (ll)i * i < b; i++)
		if (!notp[i]) {
			//sieve [2, sqrt(b))
			for (int j = (i << 1); (ll)j * j < b; j += i)
				notp[j] = 1;
			//sieve [a, b)
			for (ll j = max(2LL, (a + i - 1) / i) * i; 
j < b; j += i)
				notp_big[j - a] = 1;
		}
	for (ll j = 0; j < b - a; j++)
		if (!notp_big[j])
			prime[pnum++] = a + j;
	return pnum;
}
```

<div style="page-break-after: always;"></div>

### 扩展欧几里得

------

* 辗转相除法时间复杂度$O(h)$，`h`为`b`在10进制下的位数。

* 扩展欧几里得算法，求满足 $ax + by = gcd(a, b)$ 的 $x$ 和 $y$ 。（求得结果必定满足 $x \leq b$ 和 $y \leq a$）

```cpp
//find x and y to satisfy ax + by = gcd(a, b)
//x will <= b, y will <= a
ll extgcd(ll a, ll b, ll& x, ll& y) {
	if (b == 0) {
		x = 1; y = 0; return a;
	} else {
		ll r = extgcd(b, a % b, y, x);
		y -= (a / b) * x;
		return r;
	}
}

ll inv(ll a, ll m) {
	ll x, y;
	extgcd(a, m, x, y);
	return (m + x % m) % m;
}
```

<div style="page-break-after: always;"></div>

### 快速幂

------

整数和矩阵快速幂，时间复杂度均为$O(\log{N})$。
`main`中为求斐波那契数列第$n$项的代码。

```cpp
#define MOD 10000007

struct matrix {
	int n, m;
	ll dat[maxn][maxn];//both start from 1
	matrix(int nn = 0, int mm = 0): n(nn), m(mm) {
		memset(dat, 0, sizeof dat);
		for (int i = 1; i <= n; i++) dat[i][i] = 1;
	}
	ll* operator[](const int i) { return dat[i]; }
};

inline matrix mat_mul(matrix &a, matrix &b) {
	matrix ans(a.n, a.n);
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= b.m; j++) {
			ll sum = 0;
			for (int k = 1; k <= a.m; k++)
				sum = (sum + a[i][k] * b[k][j]) % MOD;
			ans[i][j] = sum;
		}
	return ans;
}

matrix qpow_mat(matrix a, ll k) {
	matrix ans(a.n, a.n);
	for (; k; k >>= 1) {
		if (k & 1) ans = mat_mul(ans, a);
		a = mat_mul(a, a);
	}
	return ans;
}

ll qpow(ll a, ll k) {//(a^k)%MOD
	ll ans = 1;
	for (; k; k >>= 1) {
		if (k & 1) ans = (ans * a) % MOD;
		a = (a * a) % MOD;
	}
	return ans;
}

int main() {
	int n; scanf("%d", &n);
	matrix base(2, 2);
	base[1][1] = 1; base[1][2] = 1;
	base[2][1] = 1; base[2][2] = 2;
	int nn = (n - 1) / 2;
	matrix mm = qpow_mat(base, nn);
	if (n & 1) 
		printf("%lld\n", mm[1][1] + mm[1][2]);
	else 
		printf("%lld\n", mm[2][1] + mm[2][2]);
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 全排列 - 递归版

------

```cpp
#include <stdio.h>
int a[100], n = 12;

void permutation(int x) {
	if (x == n - 1) {
		for (int i = 0; i < n; i++) printf("%d ", a[i]);
		printf("\n"); return;
	}
	int dup[100] = {};
	for (int i = x; i < n; i++) {
		if (dup[a[i]]) continue;
		dup[a[i]] = 1;
		swap(a[i], a[x]);
		permutation(x + 1);
		swap(a[i], a[x]);
	}
}

int main() {
	for (int i = 0; i < n; i++) a[i] = i + 1;
	permutation(0);
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 全排列 - 非递归版

------

```cpp
#include <stdio.h>

inline void rev(int* from, int* to) {
	while (from < to) {
		swap(*from, *to);
		from++; to--;
	}
}

inline bool get_next_permutation(int* a, int size) {
	int i = size - 2;
	while ((i >= 0) && (a[i] >= a[i + 1])) i--;
	if (i < 0) return 0;
	int j = size - 1;
	while (a[j] <= a[i]) j--;
	swap(a[j], a[i]);
	rev(a + i + 1, a + size - 1);
	return 1;
}

int main() {
	int a[1005], size = 12;
	for (int i = 0; i < size; i++) a[i] = i + 1;
	do {
		for (int i = 0; i < size; i++) printf("%d ", a[i]);
		printf("\n");
	} while (get_next_permutation(a, size));
	return 0;
}
```

<div style="page-break-after: always;"></div>

### FFT

-------

快速傅里叶变换，包括多项式乘法、多项式求逆、多项式除法。

时间复杂度 $O(nlogn)$。

<pre>
#define pi M_PI

struct comp {
	double re, im;
	comp(double r = 0, double i = 0): re(r), im(i) {}
	comp operator + (comp x) { return comp(re + x.re, im + x.im); }
	comp operator - (comp x) { return comp(re - x.re, im - x.im); }
	comp operator * (comp x) { 
		return comp(re * x.re - im * x.im, re * x.im + im * x.re); 
	}
};

void FFT(comp* a, int* g, int n, int f) {
	for (int i = 0; i < n; i++)
		if (g[i] > i) swap(a[i], a[g[i]]);
	for (int i = 1; i < n; i <<= 1) {
		comp wn1(cos(pi / i), f * sin(pi / i));
		for (int j = 0; j < n; j += (i << 1)) {
			comp wnk(1, 0);
			for (int k = 0; k < i; k++, wnk = wnk * wn1) {
				comp x = a[j + k], y = wnk * a[j + k + i];
				a[j + k] = x + y; a[j + k + i] = x - y;
			}
		}
	}
	if (!(~f)) for (int i = 0; i < n; i++) a[i].re /= n;
}

void mul(comp* a, comp* b, comp* ans, int n) {
	int _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	FFT(a, g, _n, 1); FFT(b, g, _n, 1);
	for (int i = 0; i < _n; i++) { 
		ans[i] = a[i] * b[i]; 
		b[i] = comp(0, 0); 
	}
	FFT(ans, g, _n, -1);
	delete[] g;
}

void inv(comp* a, comp* b, int n) {
	if (n == 1) { b[0].re = 1 / a[0].re; return; }
	inv(a, b, (n + 1) >> 1);
	int _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	comp* tmp = new comp[_n];
	for (int i = 0; i < n; i++) tmp[i] = a[i];
	FFT(tmp, g, _n, 1); FFT(b, g, _n, 1);
	for (int i = 0; i < _n; i++) {
		tmp[i] = comp(2, 0) - tmp[i] * b[i];
		b[i] = tmp[i] * b[i];
	}
	FFT(b, g, _n, -1);
}

void div(comp* a, comp* b, comp* d, comp* r, int n, int m) {
	int _n = 1, t = -1;
	while (_n < (n - m + 1) << 1) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	for (int i = 0; i < (n >> 1); i++) swap(a[i], a[n - i - 1]);
	for (int i = 0; i < (m >> 1); i++) swap(b[i], b[m - i - 1]);
	comp* invb = new comp[_n];
	inv(b, invb, n - m + 2);
	FFT(a, g, _n, 1); FFT(invb, g, _n, 1);
	for (int i = 0; i < _n; i++) d[i] = a[i] * invb[i];
	FFT(d, g, _n, -1);
	for (int i = 0; i < ((n - m + 1) >> 1); i++) 
		swap(d[i], d[(n - m + 1) - i - 1]);
	for (int i = n - m; ~i; i--) {
		d[i - 1].re += (d[i].re - floor(d[i].re)) * 10;
		d[i].re = floor(d[i].re);
	}
}
</pre>

<div style="page-break-after: always;"></div>

### 原根

------


```cpp
#define maxn 1000005
#define ll long long

bool hasroot[maxn], notp[maxn];
int pnum, p[maxn], phi[maxn];

void init(int n) {
	pnum = 0;
	for (int i = 2; i < n; i++) {
		if (!notp[i]) {
			p[pnum++] = i;
			phi[i] = i - 1;
		}
		for (int j = 0; j < pnum && i * p[j] < n; j++) {
			int k = i * p[j]; notp[k] = 1;
			if (i % p[j] == 0) {
				phi[k] = phi[i] * p[j];
				break;
			}
			phi[k] = phi[i] * (p[j] - 1);
		}
	}
	hasroot[1] = hasroot[2] = hasroot[4] = 1;
	for (int i = 1; i < pnum; i++) {
		for (ll j = p[i]; j < maxn; j *= p[i])
			hasroot[j] = 1;
		for (ll j = p[i] << 1; j < maxn; j *= p[i])
			hasroot[j] = 1;
	}
}

inline ll qpow(ll a, ll k, ll MOD) {
	ll ans = 1;
	for (; k; k >>= 1) {
		if (k & 1) ans = (ans * a) % MOD;
		a = (a * a) % MOD;
	}
	return ans;
}

int minroot(int n) {
	if (n < 3) return 1;
	if (!hasroot[n]) return -1;
	int fac[100], fnum = 0, x = phi[n];
	for (int i = 2; i * i <= x; i++)
		if (x % i == 0) {
			fac[fnum++] = i;
			while (x % i == 0) x /= i;
		}
	if (x != 1) fac[fnum++] = x;
	for (int a = 1; a < n; a++) {
		if (gcd(a, n) != 1) continue;
		bool succ = 1;
		for (int k = 0; k < fnum; k++)
			if (qpow(a, phi[n] / fac[k], n) == 1) {
				succ = 0; break;
			}
		if (succ) return a;	
	}
	return -1;
}

void allroot(int n, int& rnum, int* r) {
	int minr = minroot(n); rnum = 0;
	r[rnum++] = minr;
	if (minr == -1) return;
	for (int d = 2; d < phi[n]; d++)
		if (gcd(d, phi[n]) == 1)
			r[rnum++] = qpow(minr, d, n);
}

int r[maxn], rnum;

int main() {
	int n; init(maxn);
	while (scanf("%d", &n) == 1) {
		allroot(n, rnum, r);
		sort(r, r + rnum);
		printf("%d", r[0]);
		for (int i = 1; i < rnum; i++)
			printf(" %d", r[i]);
		putchar('\n');
	}
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 线性筛

------

```cpp
#define maxn 1000005
#define ll long long

bool notp[maxn];
int pnum, p[maxn], phi[maxn], u[maxn];
int mindp[maxn], fac[maxn];
int mindv[maxn], sumd[maxn];

void sieve(int n) {
	memset(notp, 0, sizeof notp); pnum = 0;
	sumd[1] = fac[1] = u[1] = 1;
	for (int i = 2; i < n; i++) {
		if (!notp[i]) {
			p[pnum++] = i;
			phi[i] = i - 1;
			u[i] = -1;
			mindp[i] = 1;
			fac[i] = 2;
			mindv[i] = i;
			sumd[i] = i + 1;
		}
		for (int j = 0; j < pnum && i * p[j] < n; j++) {
			int k = i * p[j]; notp[k] = 1;
			if (i % p[j] == 0) {
				phi[k] = phi[i] * p[j];
				u[k] = 0;
				mindp[k] = mindp[i] + 1;
				fac[k] = fac[i] / (mindp[i] + 1) * (mindp[i] + 2);
				mindv[k] = mindv[i] * p[j];
				ll tmp = (ll)sumd[i] * (ll)(mindv[k] * p[j] - 1);
				sumd[k] = (int)(tmp / (ll)(mindv[k] - 1));
				break;
			}
			phi[k] = phi[i] * (p[j] - 1);
			u[k] = -u[i];
			mindp[k] = 1;
			fac[k] = fac[i] << 1;
			mindv[k] = p[j];
			sumd[k] = sumd[i] * (1 + p[j]);
		}
	}
}

int main() {
	sieve(maxn);
	return 0;
}
```

<div style="page-break-after: always;"></div>

## 哈希

* 整数哈希
* 字符串哈希

<div style="page-break-after: always;"></div>

### 整数哈希

------

**初始化**: 

```
memset(h, -1, sizeof(h));
```

```cpp
#define mod1 1000003
#define mod2 1009
#define hash1(x) ((x & 0x7fffffff) % mod1)
#define hash2(x) ((x & 0x7fffffff) % mod2)
int h[mod1], m, n, t;

inline void ins(int x) {
	int idx, h1 = hash1(x), h2 = hash2(x);
	for (int i = 0;; i++) {
		idx = (h1 + i * h2) % mod1;
		if (h[idx] == x || h[idx] == -1) break;
	}
	h[idx] = x;
}

inline bool query(int x) {
	int idx, h1 = hash1(x), h2 = hash2(x);
	for (int i = 1;; i++) {
		idx = (h1 + i * h2) % mod1;
		if (h[idx] == -1) return false;
		if (h[idx] == x) return true;
	}
}
```

<div style="page-break-after: always;"></div>

### 字符串哈希

------

```cpp
const int mod1 = 100003;
char h[mod1][1005];

inline int BKDRHash(const char* s) {
	int ans = 0, len = strlen(s);
	for (int i = 0; i < len; i++)
		ans = ans * 131 + s[i];
	return (ans & 0x7fffffff) % mod1;
}

inline void ins(const char *s) {
	for (int idx = BKDRHash(s);; idx = (idx + 1) % mod1) {
		if (h[idx][0] == 0) { strcpy(h[idx], s); return; }
		if (strcmp(h[idx], s) == 0) return;
	}
}

inline bool query(const char *s) {
	for (int idx = BKDRHash(s);; idx = (idx + 1) % mod1) {
		if (h[idx][0] == 0) return false;
		if (strcmp(h[idx], s) == 0) return true;
	}
}
```

<div style="page-break-after: always;"></div>

## 动态规划

* 最长不下降序列 - LIS
* 最长公共子序列 - LCS


<div style="page-break-after: always;"></div>

### 最长不下降序列 - LIS

------

数组`l[]`即为LIS之一，时间复杂度：$O(nlogn)$。

```cpp
int n, a[maxn], l[maxn], pre[maxn], sub[maxn] = {-1};

int LIS() {
	int len = 0;
	for (int i = 0; i < n; i++) {
		int l = 1, r = len;
		while (l <= r) {
			int mid = (l + r + 1) >> 1;
			if (a[sub[mid]] < a[i]) l = mid + 1;
			else r = mid - 1;
		}
		pre[i] = sub[l - 1]; 
		sub[l] = i;
		if (l > len) len = l;
	}
	return len;
}

void main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	int len = LIS();
	for (int p = sub[len], cnt = len; ~p; p = pre[p])
		l[--cnt] = a[p];
}
```

<div style="page-break-after: always;"></div>

### 最长公共子序列 - LCS

------

求两个字符串`a`和`b`的最长公共子序列，并统计最长序列的个数。

时间复杂度：$O(NM)$空间复杂度：$O(N + M)$

```cpp
char a[MAXLEN], b[MAXLEN];
int dp[2][MAXLEN], sum[2][MAXLEN];

int main() {
	scanf("%s %s", a + 1, b + 1);
	int len1 = strlen(a + 1), len2 = strlen(b + 1);
	for (int j = 0; j < len2; j++) sum[0][j] = 1;
	sum[1][0] = 1;
	int now = 1, pre = 0;
	for (int i = 1; i < len1; i++) {
		for (int j = 1; j < len2; j++)
			if (a[i] == b[j]) {
				dp[now][j] = dp[pre][j - 1] + 1;
				sum[now][j] = sum[pre][j - 1]
+ (dp[now][j] == dp[pre][j]) * sum[pre][j]
+ (dp[now][j] == dp[now][j - 1]) * sum[now][j - 1];
			} else {
				dp[now][j] = max(dp[pre][j], 
					dp[now][j - 1]);
				sum[now][j] = 0
+ (dp[now][j] == dp[pre][j]) * sum[pre][j]
+ (dp[now][j] == dp[now][j - 1]) * sum[now][j - 1]
- (dp[now][j] == dp[pre][j - 1]) * sum[pre][j - 1];
			}
		swap(now, pre);
	}
	cout << dp[pre][len2 - 1] << sum[pre][len2 - 1];
}
```

<div style="page-break-after: always;"></div>

## 其他

* IO 外挂

<div style="page-break-after: always;"></div>

### IO 外挂

------

```cpp
#define BUF_SIZE 100000
#define eps 1e-12
#define ll long long
ll pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
		1000000000, 10000000000LL, 100000000000LL, 1000000000000LL, 10000000000000LL,
		100000000000000LL, 1000000000000000LL, 10000000000000000LL, 100000000000000000LL };

char bufin[BUF_SIZE], *startin = bufin, *endin = bufin;
char bufout[BUF_SIZE], *startout = bufout;

inline char getch() {
	if (startin == endin)
		endin = (startin = bufin) + fread(bufin, 1, BUF_SIZE, stdin);
	return *startin++;
}

inline bool blank(char ch) {
	return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

inline int getint() {
	bool sign = 0; char ch = getch(); int x = 0;
	for (; blank(ch); ch = getch());
	if (ch == '-') { sign = 1; ch = getch(); }
	for (; ch >= '0' && ch <= '9'; ch = getch())
		x = x * 10 + ch - '0';
	return sign ? -x : x;
}

inline double getdb() {
	bool sign = 0; char ch = getch(); double x = 0;
	for (; blank(ch); ch = getch());
	if (ch == '-') { sign = 1; ch = getch(); }
	for (; ch >= '0' && ch <= '9'; ch = getch())
		x = x * 10 + ch - '0';
	if (ch == '.') {
		double tmp = 1; ch = getch();
		for (; ch >= '0' && ch <= '9'; ch = getch())
			tmp /= 10.0, x += tmp * (ch - '0');
	}
	return sign ? -x : x;
}

inline void getstr(char *s) {
	char ch = getch();
	for (; blank(ch); ch = getch());
	for (; !blank(ch); ch = getch()) *s++ = ch;
	*s = 0;
}

inline void putch(char ch) {
	if (startout == bufout + BUF_SIZE)
		fwrite(startout = bufout, 1, BUF_SIZE, stdout);
	*startout++ = ch;
}

inline void putint(int x) {
	if (x < 0) putch('-');
	if (x < 10) putch(x + '0');
	else {
		putint(x / 10);
		putch(x % 10 + '0');
	}
}

inline void putdb(double x, int y) {
	if (x < -eps) { putch('-'); x = -x; }
	x = x * pow10[y];
	long long x1 = (long long) floor(x);
	if (x - floor(x) >= 0.5) ++x1;
	long long x2 = x1 / pow10[y], x3 = x1 - x2 * pow10[y];
	putll(x2);
	if (y > 0) {
		putch('.');
		for (int i = 1; i < y && x3 * pow10[i] < pow10[y]; putch('0'), ++i);
		putll(x3);
	}
}

inline void putstr(char *s) { while (*s) putch(*s++); }

inline void flush() {
	if (startout != bufout) {
		fwrite(bufout, 1, startout - bufout, stdout);
		startout = bufout;
	}
}
```

<div style="page-break-after: always;"></div>

### 带符号大整数类

------

```
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#define max(x, y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) > (y) ? (y) : (x))
#define abs(x) ((x) < 0 ? (-x) : (x))
#define maxlen 40005
#define ll long long
#define pi (acos(-1))
using namespace std;

struct comp {
	double re, im;
	comp(double r = 0, double i = 0): re(r), im(i) {}
	comp operator + (comp x) { return comp(re + x.re, im + x.im); }
	comp operator - (comp x) { return comp(re - x.re, im - x.im); }
	comp operator * (comp x) { 
		return comp(re * x.re - im * x.im, re * x.im + im * x.re); 
	}
};

class bint {
public:
	int len, s[maxlen];
	void inc() { 
		int flag = neg(); s[0] += 1; 
		for (int i = 0; i < len && s[i] == 10; s[i++] = 0) s[i + 1]++;
		if (neg() > flag) s[len++] = 0;
	}
	void dec() {
		int flag = neg(); s[0] -= 1; 
		for (int i = 0; i < len && !(~s[i]); s[i++] = 9) s[i + 1]--;
		if (neg() < flag) s[len++] = 9;
	}
	void inv() { for (int i = 0; i < len; i++) s[i] = 9 - s[i]; inc(); }
	int neg() const { return s[len - 1] > 4 ? 9 : 0; }
	void clean() { 
		while (len > 2 && s[len - 1] == 0 && s[len - 2] < 5) len--; 
		while (len > 2 && s[len - 1] == 9 && s[len - 2] > 4) len--;
	}

public:
	bint(char* raw) {
		memset(s, 0, sizeof s);
		char *str = raw[0] == '-' ? raw + 1 : raw; len = (int)strlen(str) + 1;
		for (int i = 0; str[i]; i++) s[len - i - 2] = str[i] - '0';
		if (raw[0] == '-') inv();
	}
	bint(int v = 0) {
		memset(s, 0, sizeof s); 
		int val = abs(v); if (!val) { len = 2; return; }
		for (len = 0; val; val /= 10) s[len++] = val % 10; len++;
		if (v < 0) inv();
	}
	bint(const bint& a) { len = a.len; memcpy(s, a.s, sizeof(int) * len); }
	bint& operator = (const bint& a) { 
		len = a.len; memcpy(s, a.s, sizeof(int) * len); return *this; 
	}
	int to_int() {
		int ans = 0; bint tmp = *this; if (neg()) tmp.inv();
		for (int i = 0, b = 1; i < len; i++, b *= 10) ans += b * tmp.s[i];
		return neg() ? -ans : ans;
	}
	char* to_str() {
		char *ans = new char[len + 5]; int cnt = 0;
		bint tmp = *this; if (neg()) { tmp.inv(); ans[cnt++] = '-'; }
		for (int i = tmp.len - 1; ~i; i--) 
			if (tmp.s[i] || i == 0) {
				for (int j = i; ~j; j--) ans[cnt++] = tmp.s[j] + '0'; break;
		}
		ans[cnt++] = 0; return ans;
	}
	friend bint operator + (bint a, bint b);
	friend bint operator - (bint a, bint b);
	friend bint operator * (bint a, bint b);
	friend bint operator * (bint a, int b);
	friend bint operator / (bint a, bint b);
	friend bint operator / (bint a, int b);
	friend bint operator % (bint a, bint b);
	friend bint operator % (bint a, int b);
	friend bint operator << (bint a, int k);
	friend bint operator >> (bint a, int k);
	bint operator <<= (int k);
	bint operator >>= (int k);
	bint operator += (bint a) 	{ *this = *this + a; return *this; }
	bint operator -= (bint a) 	{ *this = *this - a; return *this; }
	bint operator *= (bint a)	{ *this = *this * a; return *this; }
	bint operator *= (int b);
	bint operator /= (bint a) 	{ *this = *this / a; return *this; }
	bool operator < (bint a)  	{ return (*this - a).neg() > 0; }
	bool operator > (bint a)  	{ return (a - *this).neg() > 0; }
	bool operator <= (bint a) 	{ return (a - *this).neg() <= 0; }
	bool operator >= (bint a) 	{ return (*this - a).neg() <= 0; }
	bool operator == (bint a) 	{ bint t = *this - a; return t.len < 10 && !t.to_int(); }
	bool operator != (bint a) 	{ return !(*this == a); }
	bint operator ++ (int)     	{ bint tmp(*this); inc(); return tmp; }
	bint operator ++ ()			{ inc(); return *this; }
	bint operator -- (int)		{ bint tmp(*this); dec(); return tmp; }
	bint operator -- ()			{ dec(); return *this; }
	bint operator - () 			{ bint tmp(*this); tmp.inv(); return tmp; }
	friend std::ostream& operator << (std::ostream&, const bint&);
	friend std::istream& operator >> (std::istream&, bint&);
};

bint bint::operator <<= (int k) {	
	for (int i = len - 1; ~i; i--) s[i + k] = s[i];
	len += k; clean(); return *this;	
}

bint bint::operator >>= (int k) {
	for (int i = 0; i < len - k; i++) s[i] = s[i + k];
	len -= k; if (len <= 0) *this = 0; clean(); return *this;
}

bint operator + (bint a, bint b) {
	bint ans; ans.len = max(a.len, b.len) + 1;
	for (int i = a.len, ta = a.neg(); i < ans.len; i++) a.s[i] = ta;
	for (int i = b.len, tb = b.neg(); i < ans.len; i++) b.s[i] = tb;
	for (int i = 0; i <= a.len || i <= b.len; i++) {
		ans.s[i] += a.s[i] + b.s[i];
		if (ans.s[i] > 9) { ans.s[i] -= 10; ans.s[i + 1]++; }
	}
	ans.clean(); return ans;
}

bint operator - (bint a, bint b) { b.inv(); return a + b; }

void FFT(comp* a, int* g, int n, int f) {
	for (int i = 0; i < n; i++)
		if (g[i] > i) swap(a[i], a[g[i]]);
	for (int i = 1; i < n; i <<= 1) {
		comp wn1(cos(pi / i), f * sin(pi / i));
		for (int j = 0; j < n; j += (i << 1)) {
			comp wnk(1, 0);
			for (int k = 0; k < i; k++, wnk = wnk * wn1) {
				comp x = a[j + k], y = wnk * a[j + k + i];
				a[j + k] = x + y; a[j + k + i] = x - y;
			}
		}
	}
	if (!(~f)) for (int i = 0; i < n; i++) a[i].re /= n;
}

bint operator * (bint a, bint b) {
	if (b.len < 9) return a * b.to_int();
	bool flag = (a.neg() > 0) ^ (b.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
	int n = max(a.len, b.len) << 1, _n = 1, t = -1;
	while (_n < n) { _n <<= 1; t++; }
	int* g = new int[_n]; g[0] = 0;
	comp *tmpa = new comp[_n], *tmpb = new comp[_n];
	for (int i = 1; i < _n; i++)
		g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
	for (int i = 0; i < a.len; i++) tmpa[i] = comp(a.s[i], 0);
	for (int i = 0; i < b.len; i++) tmpb[i] = comp(b.s[i], 0);
	FFT(tmpa, g, _n, 1); FFT(tmpb, g, _n, 1);
	for (int i = 0; i < _n; i++) tmpa[i] = tmpa[i] * tmpb[i];
	FFT(tmpa, g, _n, -1);
	bint ans; ans.len = a.len + b.len + 1;
	for (int i = 0; i < min(_n, ans.len); i++) ans.s[i] = tmpa[i].re + 0.01;
	for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10) 
		ans.s[i + 1] += ans.s[i] / 10;
	ans.clean(); return flag ? -ans : ans;
}

bint operator * (bint a, int b) {
	if (b > 99999999) return a * bint(b);
	bool flag = (a.neg() > 0) ^ (b < 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len + 10;
	for (int i = 0; i < a.len; i++)
		ans.s[i] += a.s[i] * b;
	for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
		ans.s[i + 1] += ans.s[i] / 10;
	ans.clean(); return flag ? -ans : ans;
}

bint bint::operator *= (int b) {
	if (b > 99999999) return (*this) * bint(b);
	bool flag = (neg() > 0) ^ (b < 0);
	if (neg()) inv(); if (b < 0) b = -b;
	for (int i = 0; i < len; i++) s[i] *= b;
	len += 10;
	for (int i = 0; i < len - 2; s[i++] %= 10)
		s[i + 1] += s[i] / 10;
	clean(); return flag ? -(*this) : *this;
}

bint operator / (bint a, bint b) {
	if (b.len < 10) return a / b.to_int();
	bool flag = (a.neg() > 0) ^ (b.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
    bint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        bint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    bint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    bint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
	ans.clean(); return flag ? -ans : ans;
}

bint operator / (bint a, int b) {
	bool flag = (a.neg() > 0) ^ (b < 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len;
	long long mod = 0;
	for (int i = a.len - 1; ~i; i--) {
		mod *= 10; mod += a.s[i];
		ans.s[i] = (int)(mod / (ll)b); mod %= b;
	}
	ans.clean(); return flag ? -ans : ans;
}

bint operator % (bint a, bint b) {
	if (b.len < 10) return a % b.to_int();
	bool flag = (a.neg() > 0);
	if (a.neg()) a.inv(); if (b.neg()) b.inv();
    bint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        bint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    bint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    bint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
	mod.clean(); return flag ? -mod : mod;
}

bint operator % (bint a, int b) {
	bool flag = (a.neg() > 0);
	if (a.neg()) a.inv(); if (b < 0) b = -b;
	bint ans; ans.len = a.len;
	long long mod = 0;
	for (int i = a.len - 1; ~i; i--) {
		mod *= 10; mod += a.s[i];
		ans.s[i] = (int)(mod / (ll)b); mod %= b;
	}
	return flag ? -int(mod) : int(mod);
}

bint operator << (bint a, int k) {
	a.len += k;
	for (int i = a.len - 1; ~i; i--) a.s[i + k] = a.s[i];
	a.clean(); return a;
}

bint operator >> (bint a, int k) {
	a.len -= k;
	for (int i = 0; i < a.len; i++) a.s[i] = a.s[i + k];
	if (a.len <= 0) a = 0; a.clean(); return a;
}

std::ostream& operator << (std::ostream& out, const bint& a) {
	bint tmp = a; if (a.neg()) { putchar('-'); tmp.inv(); }
	for (int i = tmp.len - 1; ~i; i--) 
		if (tmp.s[i] || i == 0) {
			for (int j = i; ~j; j--) putchar(tmp.s[j] + '0'); break;
		}
	return out;
}

std::istream& operator >> (std::istream& cin, bint& a) {
	char s[maxlen]; scanf("%s", s); a = bint(s);
	return cin;
}

int main() {
	bint a, b;
	cin >> a >> b;
	cout << a / b << endl;
	return 0;
}
```

<div style="page-break-after: always;"></div>

### 无符号大整数类

------

```
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#define max(x, y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) > (y) ? (y) : (x))
#define maxlen 40005
#define ll long long
#define pi (acos(-1))
using namespace std;

struct comp {
    double re, im;
    comp(double r = 0, double i = 0): re(r), im(i) {}
    comp operator + (comp x) { return comp(re + x.re, im + x.im); }
    comp operator - (comp x) { return comp(re - x.re, im - x.im); }
    comp operator * (comp x) {
        return comp(re * x.re - im * x.im, re * x.im + im * x.re);
    }
};

class ubint {
public:
    int len, s[maxlen];
    void clean() { while (len > 1 && s[len - 1] == 0) len--; }
    void inc() {
        s[0]++; for (int i = 0; i < len && s[i] == 10; s[i++] = 0) s[i + 1]++;
        if (s[len]) len++;
    }
    void dec() {
        s[0]--; for (int i = 0; i < len && !(~s[i]); s[i++] = 9) s[i + 1]--;
        if (s[len - 1] == 0) len--;
    }
    int cmp(const ubint& a) {
        if (len != a.len) return len - a.len;
        for (int i = len - 1; ~i; i--)
            if (s[i] != a.s[i]) return s[i] - a.s[i];
        return 0;
    }
    
public:
    ubint(char* str) {
        memset(s, 0, sizeof s); len = (int)strlen(str);
        for (int i = 0; str[i]; i++) s[len - i - 1] = str[i] - '0';
    }
    ubint(int v = 0) {
        memset(s, 0, sizeof s);
        if (!v) { len = 1; s[0] = 0; return; }
        for (len = 0; v; v /= 10) s[len++] = v % 10;
    }
    ubint(const ubint& a) {
        len = a.len; memcpy(s, a.s, sizeof s); clean();
    }
    ubint& operator = (const ubint& a) {
        len = a.len; memcpy(s, a.s, sizeof s); clean(); return *this;
    }
    int to_int() {
        int ans = 0;
        for (int i = 0, b = 1; i < len; i++, b *= 10) ans += b * s[i];
        return ans;
    }
    char* to_str() {
        char *ans = new char[len + 3]; int cnt = 0; clean();
        for (int i = len - 1; ~i; i--) ans[cnt++] = s[i] + '0';
        ans[cnt++] = 0; return ans;
    }
    friend ubint operator + (ubint a, ubint b);
    friend ubint operator - (ubint a, ubint b);
    friend ubint operator * (ubint a, ubint b);
    friend ubint operator * (ubint a, int b);
    friend ubint operator / (ubint a, ubint b);
    friend ubint operator / (ubint a, int b);
    friend ubint operator % (ubint a, ubint b);
    friend ubint operator % (ubint a, int b);
    ubint operator += (ubint a) { *this = *this + a; return *this; }
    ubint operator -= (ubint a) { *this = *this - a; return *this; }
    ubint operator *= (ubint a)	{ *this = *this * a; return *this; }
    ubint operator *= (int b);
    ubint operator /= (ubint a) { *this = *this / a; return *this; }
    bool operator < (ubint a)  	{ return cmp(a) < 0; }
    bool operator > (ubint a)  	{ return cmp(a) > 0; }
    bool operator <= (ubint a) 	{ return cmp(a) <= 0; }
    bool operator >= (ubint a) 	{ return cmp(a) >= 0; }
    bool operator == (ubint a) 	{ return cmp(a) == 0; }
    bool operator != (ubint a) 	{ return cmp(a) != 0; }
    ubint operator ++ (int)     { ubint tmp(*this); inc(); return tmp; }
    ubint operator ++ ()		{ inc(); return *this; }
    ubint operator -- (int)		{ ubint tmp(*this); dec(); return tmp; }
    ubint operator -- ()		{ dec(); return *this; }
    friend std::ostream& operator << (std::ostream&, const ubint&);
    friend std::istream& operator >> (std::istream&, ubint&);
};

ubint operator + (ubint a, ubint b) {
    ubint ans; ans.len = max(a.len, b.len) + 1;
    for (int i = 0; i < a.len || i < b.len; i++) {
        ans.s[i] += a.s[i] + b.s[i];
        if (ans.s[i] > 9) { ans.s[i] -= 10; ans.s[i + 1]++; }
    }
    ans.clean(); return ans;
}

ubint operator - (ubint a, ubint b) {
    ubint ans; ans.len = a.len + 1;
    for (int i = 0; i < a.len; i++) {
        ans.s[i] += a.s[i] - b.s[i];
        if (ans.s[i] < 0) { ans.s[i] += 10; ans.s[i + 1]--; }
    }
    ans.clean(); return ans;
}

void FFT(comp* a, int* g, int n, int f) {
    for (int i = 0; i < n; i++)
        if (g[i] > i) swap(a[i], a[g[i]]);
    for (int i = 1; i < n; i <<= 1) {
        comp wn1(cos(pi / i), f * sin(pi / i));
        for (int j = 0; j < n; j += (i << 1)) {
            comp wnk(1, 0);
            for (int k = 0; k < i; k++, wnk = wnk * wn1) {
                comp x = a[j + k], y = wnk * a[j + k + i];
                a[j + k] = x + y; a[j + k + i] = x - y;
            }
        }
    }
    if (!(~f)) for (int i = 0; i < n; i++) a[i].re /= n;
}

ubint operator * (ubint a, ubint b) {
    if (b.len < 9) return a * b.to_int();
    int n = max(a.len, b.len) << 1, _n = 1, t = -1;
    while (_n < n) { _n <<= 1; t++; }
    int* g = new int[_n]; g[0] = 0;
    comp *tmpa = new comp[_n], *tmpb = new comp[_n];
    for (int i = 1; i < _n; i++)
        g[i] = (g[i >> 1] >> 1) | ((i & 1) << t);
    for (int i = 0; i < a.len; i++) tmpa[i] = comp(a.s[i], 0);
    for (int i = 0; i < b.len; i++) tmpb[i] = comp(b.s[i], 0);
    FFT(tmpa, g, _n, 1); FFT(tmpb, g, _n, 1);
    for (int i = 0; i < _n; i++) tmpa[i] = tmpa[i] * tmpb[i];
    FFT(tmpa, g, _n, -1);
    ubint ans; ans.len = a.len + b.len + 1;
    for (int i = 0; i < min(_n, ans.len); i++) ans.s[i] = tmpa[i].re + 0.01;
    for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
        ans.s[i + 1] += ans.s[i] / 10;
    ans.clean(); return ans;
}

ubint operator * (ubint a, int b) {
    if (b > 99999999) return a * ubint(b);
    ubint ans; ans.len = a.len + 10;
    for (int i = 0; i < a.len; i++)
        ans.s[i] += a.s[i] * b;
    for (int i = 0; i < ans.len - 2; ans.s[i++] %= 10)
        ans.s[i + 1] += ans.s[i] / 10;
    ans.clean(); return ans;
}

ubint ubint::operator *= (int b) {
    if (b > 99999999) return (*this) * ubint(b);
    for (int i = 0; i < len; i++) s[i] *= b;
    len += 10;
    for (int i = 0; i < len - 2; s[i++] %= 10)
        s[i + 1] += s[i] / 10;
    clean(); return *this;
}

ubint operator / (ubint a, ubint b) {
    if (b.len < 10) return a / b.to_int();
    ubint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        ubint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    ubint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    ubint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
    ans.clean(); return ans;
}

ubint operator / (ubint a, int b) {
    ubint ans; ans.len = a.len; ll mod = 0;
    for (int i = a.len - 1; ~i; i--) {
        mod *= 10; mod += a.s[i];
        ans.s[i] = (int)(mod / (ll)b); mod %= b;
    }
    ans.clean(); return ans;
}

ubint operator % (ubint a, ubint b) {
    if (b.len < 10) return a % b.to_int();
    ubint invb = 1; int lim = a.len + 5, fac = b.len;
    for (int i = 0; i < 100; i++) {
        ubint two(0), last = invb; two.len = fac + 1; two.s[fac] = 2;
        invb = invb * (two - b * invb); fac <<= 1;
        if (invb.len > lim) {
            fac -= invb.len - lim;
            for (int i = 0; i < lim; i++)
                invb.s[i] = invb.s[i + invb.len - lim];
            for (int i = lim; i < invb.len; i++)
                invb.s[i] = 0;
            invb.len = lim;
        }
        if (last == invb) break;
    }
    ubint ans = a * invb; if (ans.len < fac) return 0;
    for (int i = 0; i < ans.len - fac; i++)
        ans.s[i] = ans.s[i + fac];
    for (int i = ans.len - fac; i < ans.len; i++)
        ans.s[i] = 0;
    ans.len -= fac; if (ans.len == 0) ans = 0;
    ubint mod = a - b * ans;
    while (mod >= b) { ans++; mod -= b;}
    ans.clean(); return mod;
}

ubint operator % (ubint a, int b) {
    ubint ans; ans.len = a.len; ll mod = 0;
    for (int i = a.len - 1; ~i; i--) {
        mod *= 10; mod += a.s[i];
        ans.s[i] = (int)(mod / (ll)b); mod %= b;
    }
    return int(mod);
}

std::ostream& operator << (std::ostream& out, const ubint& a) {
    for (int i = a.len - 1; ~i; i--) putchar(a.s[i] + '0');
    return out;
}

std::istream& operator >> (std::istream& cin, ubint& a) {
    char s[maxlen]; scanf("%s", s); a = ubint(s);
    return cin;
}

int main() {
    ubint a, b; cin >> a >> b;
    cout << a % b << endl;
    return 0;
}
```

<div style="page-break-after: always;"></div>

### 分数类

------

```
#define sign(x) ((x) > 0 ? 1 : -1)

inline int gcd(int a, int b) {
	while (b) { int t = a % b; a = b; b = t; }
	return a;
}

inline int lcm(int a, int b) { return (a * b) / gcd(a, b); }

struct frac {
	int x, y; // x / y
	frac(int val = 0) { x = val; y = 1; }
	frac(int a, int b) {
		int t = gcd(abs(a), abs(b));
		x = (a / t) * sign(b);
		y = (x == 0 ? 1 : (abs(b) / t));
	}
	frac(char* s) {
		int len = strlen(s), a, b, t;
		for (int i = 0; i < len; i++)
			if (s[i] == '/') {
				sscanf(s, "%d/%d", &a, &b); *this = frac(a, b); return;
			} else if (s[i] == '.') {
				sscanf(s, "%d.%d", &a, &t);
				for (int j = 0; j < len - i - 1; j++) b *= 10;
				a *= b; a += t; *this = frac(a, b); return;
			}
		sscanf(s, "%d", &a); x = a; y = 1;
	}
	frac operator + (const frac& f) { return frac(x * f.y + f.x * y, y * f.y); }
	frac operator - (const frac& f) { return frac(x * f.y - f.x * y, y * f.y); }
	frac operator * (const frac& f) { return frac(x * f.x, y * f.y); }
	frac operator / (const frac& f) { return frac(x * f.y, y * f.x); }
};

inline void print(const frac& f) {
	printf("%d", f.x); if (f.y != 1) printf("/%d", f.y);
}
```

<div style="page-break-after: always;"></div>

### 矩阵类

------

```
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#define MAXN 100
#define MAXM 200
//N * M
using namespace std;

template <class T>
struct matrix {
	int n, m;
	T dat[MAXN][MAXM];//both start from 1
	matrix(int nn = 0, int mm = 0): n(nn), m(mm) {}
	T* operator[](int i) { return dat[i]; }
};

template <class T>
void matcpy(matrix<T>& a, matrix<T>& b) {
	b.n = a.n; b.m = a.m;
	memcpy(b.dat, a.dat, sizeof(T) * MAXM * MAXN);
}

template <class T>
void matadd(matrix<T>& a, matrix<T>& b, matrix<T>& ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] + b[i][j];
}

template <class T>
void matsub(matrix<T> &a, matrix<T> &b, matrix<T>& ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] - b[i][j];
}

template <class T>
void matmul(matrix<T> &a, matrix<T> &b, matrix<T> &ans) {
	ans.n = a.n; ans.m = b.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= b.m; j++) {
			T sum = 0;
			for (int k = 1; k <= a.m; k++)
				sum += a[i][k] * b[k][j];
			ans[i][j] = sum;
		}
}

template <class T>
void matmul(matrix<T> &a, int k, matrix<T> &ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] * k;
}

template <class T>
void matdiv(matrix<T> &a, int k, matrix<T> &ans) {
	ans.n = a.n; ans.m = a.m;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.m; j++)
			ans[i][j] = a[i][j] / k;
}

template <class T>
T det(matrix<T> &a) {
	if (a.m - a.n) exit(1);//error
	T ans = 1;
	for (int px = 1; px <= a.n; px++) {
		if (a[px][px] == 0) 
			for (int i = px + 1; i <= a.n; i++)
				if (a[i][px] != 0) {
					for (int j = px; j <= a.n; j++) swap(a[i][j], a[px][j]);
					ans = -ans; break;
				}
		if (a[px][px] == 0) return 0;
		ans *= a[px][px];
		for (int i = px + 1; i <= a.n; i++) {
			T k = a[i][px] / a[px][px];
			for (int j = px; j <= a.n; j++) a[i][j] -= a[px][j] * k;
		}
	}
	return ans;
}

template <class T>
void inv(matrix<T> &a, matrix<T> &ans) {
	if (a.n - a.m) exit(1);
	a.m <<= 1;
	for (int i = 1; i <= a.n; i++) {
		for (int j = a.n + 1; j <= a.m; j++) a[i][j] = 0;
		a[i][i + a.n] = 1;
	}
	for (int px = 1; px <= a.n; px++) {
		if (a[px][px] == 0)
			for (int i = px + 1; i <= a.n; i++)
				if (a[i][px] != 0) {
					for (int j = px; j <= a.m; j++) swap(a[i][j], a[px][j]);
					break;
				}
		if (a[px][px] == 0) { ans.n = -1; return; }
		for (int j = px + 1; j <= a.m; j++) a[px][j] /= a[px][px];
		a[px][px] = 1;
		for (int i = px + 1; i <= a.n; i++) {
			T k = a[i][px];
			for (int j = px; j <= a.m; j++) a[i][j] -= a[px][j] * k;
		}
	}
	for (int px = a.n; px >= 1; px--)
		for (int i = px - 1; i >= 1; i--)
			if (a[i][px] != 0) {
				T k = a[i][px];
				for (int j = px; j <= a.m; j++) a[i][j] -= a[px][j] * k;
			}
	ans.n = ans.m = a.n;
	for (int i = 1; i <= a.n; i++)
		for (int j = 1; j <= a.n; j++)
			ans[i][j] = a[i][j + a.n];
}
```

<div style="page-break-after: always;"></div>

### 计算几何

------

```
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <vector>

/////////////////////////////////////////////////////
//L = line, C = circle, S = segment, P = point, F = flat
/////////////////////////////////////////////////////

using namespace std;
const double eps = 1e-8;
const double pi = acos(-1.0);

/////////////////////////////////////////////////////
//点和向量的定义
/////////////////////////////////////////////////////

struct point {
	double x, y;
	point(double x = 0, double y = 0):x(x), y(y) {}
};
typedef point vec;//实现上，vector不过是point的别名
vec operator + (vec a, vec b) { return vec(a.x + b.x, a.y + b.y); }
vec operator - (point a, point b) { return vec(a.x - b.x, a.y - b.y); }
vec operator * (vec a, double p) { return vec(a.x * p, a.y * p); }
vec operator / (vec a, double p) { return vec(a.x / p, a.y / p); }
bool operator < (const point &a, const point &b) {//不要使用>,而使用<
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}
int dcmp(double x) {//三态函数
	if (fabs(x) < eps)
		return 0;
	else
		return x < 0 ? -1 : 1;
}
bool operator == (const point &a, const point &b) {
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y == 0);
}

/////////////////////////////////////////////////////
//点和向量的基本运算
/////////////////////////////////////////////////////

double dot(vec a, vec b) { return a.x*b.x + a.y*b.y; };//点积
double length(vec a) { return sqrt(dot(a, a)); }
double angle(vec a, vec b) {//俩向量夹角
	double ans = dot(a, b) / length(a) / length(b);
	if (dcmp(ans - 1) == 0) ans = 1;
	if (dcmp(ans + 1) == 0) ans = -1;
	return acos(ans);
}
double angle(vec a) { return atan2(a.y, a.x); }//向量的极角
double cross(vec a, vec b) { return a.x * b.y - a.y * b.x; }//叉积
double area(point a, point b, point c) {//以a,b,c为顶点的三角形的面积
	return cross(b - a, c - a) / 2;
}
vec rotate(vec a, double rad) {//将向量a绕起点逆时针旋转rad角
	return vec(a.x*cos(rad) - a.y*sin(rad), a.x*sin(rad) + a.y*cos(rad));
}
vec normal(vec a) {//a的单位法线(即左转90度,再长度归一)
	double l = length(a);
	return vec(-a.y / l, a.x / l);
}

/////////////////////////////////////////////////////
//点和直线
/////////////////////////////////////////////////////

struct line {
	point p;
	vec v;
	double rad;//极角
	line() {}
	line(point p, vec v):p(p), v(v) { rad = angle(v); }
	bool operator < (const line& l) const {
		return rad < l.rad;
	}
};
point get_LL_intersection(point p, vec v, point q, vec w) {//俩直线交点
	vec u = p - q;//直线1:p+t*v,直线2:q+t*w
	double t = cross(w, u) / cross(v, w);
	return p + v * t;
}
double dis_of_PL(point p, point a, point b) {//点p到直线AB的距离
	vec v1 = b - a, v2 = p - a;
	return fabs(cross(v1, v2)) / length(v1);
}
double dis_of_PS(point p, point a, point b) {//点p到线段AB的距离
	if (a == b) return length(p - a);
	vec v1 = b - a, v2 = p - a, v3 = p - b;
	if (dcmp(dot(v1, v2)) < 0)
		return length(v2);//返回AP长度
	else if (dcmp(dot(v1, v3)) > 0)
		return length(v3);//返回BP长度
	else
		return fabs(cross(v1, v2)) / length(v1);//计算方法同直线方法
}
bool P_on_L_left(point a, point p, vec v) {//判断点a是否在线段p+tv左侧
	return cross(v, a - p) > 0;
}
point get_PL_projection(point p, point a, point b) {//p点在直线AB上的投影点
	vec v = b - a;
	return a + v*(dot(v, p - a) / dot(v, v));
}
bool SS_intersection(point a1, point a2, point b1, point b2) {//线段相交判定
	double c1 = cross(a2 - a1, b1 - a1), c2 = cross(a2 - a1, b2 - a1),
		c3 = cross(b2 - b1, a1 - b1), c4 = cross(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}
bool P_on_S(point p, point a1, point a2) {//判断点是否在线段上
	return dcmp(cross(a1 - p, a2 - p)) == 0 && dcmp(dot(a1 - p, a2 - p)) < 0;
}

/////////////////////////////////////////////////////
//多边形
/////////////////////////////////////////////////////

double polygon_area(point *p, int n) {
	double area = 0;
	for (int i = 1; i < n - 1; i++)
		area += cross(p[i] - p[0], p[i + 1] - p[0]);
	return area / 2;
}

/////////////////////////////////////////////////////
//圆的相关计算
/////////////////////////////////////////////////////

struct circle {
	point c;
	double r;
	circle(point c, double r):c(c), r(r) {}
	point get_point(double rad) {//获得圆上一点的坐标
		return point(c.x + cos(rad)*r, c.y + sin(rad)*r);
	}
};
int get_LC_intersection(point p, vec v, circle cir, //p+tv为直线参数方程
	double &t1, double &t2, vector<point> &sol) {//t1,t2存放交点在直线上的参数,sol存放交点
	double a = v.x, b = p.x - cir.c.x, c = v.y, d = p.y - cir.c.y;
	double e = a*a + c*c, f = 2 * (a*b + c*d), g = b*b + d*d - cir.r*cir.r;
	double delta = f*f - 4 * e*g;
	if (dcmp(delta) < 0)return 0;//返回交点的个数
	if (dcmp(delta) == 0) {
		t1 = t2 = -f / (2 * e);
		sol.push_back(p + v*t1);
		return 1;
	}
	t1 = (-f - sqrt(delta)) / (2 * e); sol.push_back(p + v*t1);//相交,两个交点
	t2 = (-f + sqrt(delta)) / (2 * e); sol.push_back(p + v*t2);
	return 2;
}
int get_CC_intersection(circle cir1, circle cir2, vector<point> &sol) {//两圆相交
	double d = length(cir1.c - cir2.c);//圆心距
	if (dcmp(d) == 0) {
		if (dcmp(cir1.r - cir2.r) == 0) return -1;//两圆重合
		else return 0;//两圆内含
	}
	if (dcmp(cir1.r + cir2.r - d) < 0) return 0;//两圆相离
	if (dcmp(fabs(cir1.r - cir2.r) - d) > 0) return 0;//两圆内含
	double a = angle(cir2.c - cir1.c);//C1C2的极角
	double da = acos((cir1.r*cir1.r + d*d - cir2.r*cir2.r) / (2 * cir1.r*d));
	point p1 = cir1.get_point(a - da), p2 = cir1.get_point(a + da);
	sol.push_back(p1);
	if (p1 == p2) return 1;
	sol.push_back(p2); return 2;
}
int get_PC_tangents(point p, circle cir, vec *v) {//过点p到圆cir的切线,v[i]是第i条切线的向量
	vec u = cir.c - p;
	double dis = length(u);
	if (dis < cir.r) return 0;
	else if (dcmp(dis - cir.r) == 0) {
		v[0] = rotate(u, pi / 2);
		return 1;
	} else {
		double rad = asin(cir.r / dis);
		v[0] = rotate(u, -rad);
		v[1] = rotate(u, rad);
		return 2;
	}
}
int get_CC_tangents(circle cir1, circle cir2,//返回切线的条数,-1表示无穷 
	point *p1, point *p2) {//p1[i]和p2[i]分别表示第i条切线在圆1与圆2上的切点
	int cnt = 0;
	if (cir1.r < cir2.r) {
		swap(cir1, cir2);
		swap(p1, p2);
	}
	double d2 = (cir1.c.x - cir2.c.x)*(cir1.c.x - cir2.c.x)
		+ (cir1.c.y - cir2.c.y)*(cir1.c.y - cir2.c.y);//d^2为圆心距的平方
	double rdiff = cir1.r - cir2.r;//半径之差
	double rsum = cir1.r + cir2.r;//半径之和
	if (d2 < rdiff*rdiff) return 0;//两圆内含
	double base = angle(cir2.c - cir1.c);
	if (d2 == 0 && cir1.r == cir2.r) return -1;//无限条切线
	if (d2 == rdiff*rdiff) {//内切,1条切线
		p1[cnt] = cir1.get_point(base);
		p2[cnt] = cir2.get_point(base);
		cnt++; return 1;
	}
	double rad = acos((cir1.r - cir2.r) / sqrt(d2));//有外公切线,一定为2条
	p1[cnt] = cir1.get_point(base + rad);
	p2[cnt] = cir2.get_point(base + rad);
	cnt++;
	p1[cnt] = cir1.get_point(base - rad);
	p2[cnt] = cir2.get_point(base - rad);
	cnt++;
	if (d2 == rsum*rsum) {//1条内公切线
		p1[cnt] = cir1.get_point(base);
		p2[cnt] = cir2.get_point(base + pi);
		cnt++;
	} else if (d2>rsum*rsum) {//2条内公切线
		double rad = acos((cir1.r + cir2.r) / sqrt(d2));
		p1[cnt] = cir1.get_point(base + rad);
		p2[cnt] = cir2.get_point(base + rad + pi);
		cnt++;
		p1[cnt] = cir1.get_point(base - rad);
		p2[cnt] = cir2.get_point(base - rad + pi);
		cnt++;
	}
	return cnt;
}

/////////////////////////////////////////////////////
//二维几何常用算法
/////////////////////////////////////////////////////

int P_in_polygon(point p, point *poly, int size) {//判断点是否在多边形中
	int wn = 0;
	for (int i = 0; i < size; i++) {
		if (P_on_S(p, poly[i], poly[(i + 1) % size])) return -1;
		int k = dcmp(cross(poly[(i + 1) % size] - poly[i], p - poly[i]));
		int d1 = dcmp(poly[i].y - p.y);
		int d2 = dcmp(poly[(i + 1) % size].y - p.y);
		if (k > 0 && d1 <= 0 && d2 > 0)wn++;
		if (k < 0 && d2 <= 0 && d1 > 0)wn--;
	}
	if (wn) return 1;//点在内部
	else return 0;//点在外部
}
int convex_hull(point *p, int n, point *ch) {//n个点组成的p[]的凸包,存放在ch[]中
	sort(p, p + n);
	int m = 0;
	for (int i = 0; i < n; i++) {
		while (m > 1 && cross(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) m--;
		ch[m++] = p[i];
	}
	int k = m;
	for (int i = n - 2; i >= 0; i--) {
		while (m > k && cross(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) m--;
		ch[m++] = p[i];
	}
	if (n > 1) m--;
	return m;
}
int half_plane_intersaction(line *l, int n, point *poly) {
	sort(l, l + n);
	int first, last;//构造双端队列
	point *p = new point[n];//p[i]是q[i] & q[i+1]的交点
	line *q = new line[n];//双端队列
	q[first = last = 0] = l[0];
	for (int i = 1; i < n; i++) {
		while (first < last && !P_on_L_left(p[last - 1], l[i].p, l[i].v)) last--;
		while (first < last && !P_on_L_left(p[first], l[i].p, l[i].v)) first++;
		q[++last] = l[i];
		if (fabs(cross(q[last].v, q[last - 1].v)) < eps) {
			last--;
			if (P_on_L_left(l[i].p, q[last].p, q[last].v)) q[last] = l[i];
		}
		if (first < last) p[last - 1] = get_LL_intersection(q[last].p, q[last].v, q[last - 1].p, q[last - 1].v);
	}
	while (first < last && !P_on_L_left(p[last - 1], q[first].p, q[first].v)) last--;//删除无用平面
	if (last - first <= 1) return 0;
	p[last] = get_LL_intersection(q[last].p, q[last].v, q[first].p, q[first].v);//计算首尾交点
	int m = 0;
	for (int i = first; i <= last; i++) poly[m++] = p[i];//拷贝结果
	return m;

}

/////////////////////////////////////////////////////
//三维点的定义
/////////////////////////////////////////////////////

struct point3 {
	double x, y, z;
	point3(double x = 0, double y = 0, double z = 0):x(x), y(y) {}
};
typedef point3 vec3;
vec3 operator + (vec3 a, vec3 b) {
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
vec3 operator - (vec3 a, vec3 b) {
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
vec3 operator * (vec3 a, double p) {
	return vec3(a.x * p, a.y * p, a.z * p);
}
vec3 operator / (vec3 a, double p) {
	return vec3(a.x / p, a.y / p, a.z / p);
}

/////////////////////////////////////////////////////
//三维点的运算
/////////////////////////////////////////////////////

double dot(vec3 a, vec3 b) { return a.x*b.x + a.y*b.y + a.z * b.z; }//三维点积
double length(vec3 a) { return sqrt(dot(a, a)); }
double angle(vec3 a, vec3 b) {//俩向量夹角
	double ans = dot(a, b) / length(a) / length(b);
	if (dcmp(ans - 1) == 0)ans = 1;
	if (dcmp(ans + 1) == 0)ans = -1;
	return acos(ans);
}
double dis_of_PF(const point3 &p, const point3 &p0, const point3 &n) {
	return fabs(dot(p - p0, n));
}
point3 get_PF_projection(const point3 &p, const point3 &p0, const point3 &n) {
	return p - n*dot(p - p0, n);
}
point3 get_LF_intersaction(point3 &p1, point3 &p2, point3 &p0, vec3 n) {
	vec3 v = p2 - p1;
	double t = (dot(n, p0 - p1) / dot(n, p2 - p1));
	return p1 + v*t;
}
vec3 cross(vec3 a, vec3 b) {
	return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
double area2(point3 a, point3 b, point3 c) {
	return length(cross(b - a, c - a));
}
```