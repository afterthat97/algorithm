#include <iostream>
#include <queue>
#include <stdio.h>
#include <cstring>
#include <vector>
#define maxn 1005
using namespace std;

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
