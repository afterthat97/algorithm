#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#define MAXN 1005
#define min(a, b) ((a) > (b) ? (b) : (a))
using namespace std;

stack<int> s;
vector<int> g[MAXN], gnew[MAXN];
bool visited[MAXN], instack[MAXN];
int DFN[MAXN], low[MAXN], belong[MAXN];
int ne, np, idx, cnt;

void tarjan(int p) {
	idx++;
	DFN[p] = low[p] = idx;
	s.push(p);
	visited[p] = instack[p] = 1;

	for (int i = 0; i < g[p].size(); i++)
			if (!visited[g[p][i]]) {
				tarjan(g[p][i]);
				low[p] = min(low[p], low[g[p][i]]);
			} else if (instack[g[p][i]])
				low[p] = min(low[p], DFN[g[p][i]]);

	if (DFN[p] == low[p]) {
		cnt++;
		int j;
		do {
			j = s.top();
			s.pop();
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
