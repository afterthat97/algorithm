#include <iostream>
#include <stdio.h>
#include <algorithm>
#define maxn 1005
#define maxm 5005
using namespace std;

struct edge {
	int x, y, w;
}g[maxm];
int np, ne, par[maxn], rk[maxn];

bool cmp(const edge& a, const edge& b) { return a.w < b.w; }

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
		if (fx != fy) {//Add edge (xp, yp)
			combine(fx, fy);
			cnt++;
			all_cost += g[i].w;
		}
	}
	printf("%d\n", all_cost);
	return 0;
}
