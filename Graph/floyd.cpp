#include <stdio.h>
#define maxn 1005
int np, g[maxn][maxn];

void floyd() {
	for (int k = 1; k <= np; k++)
	for (int i = 1; i <= np; i++)
	for (int j = 1; j <= np; j++)
		if (g[i][j] > g[i][k] + g[k][j])
			g[i][j] = g[i][k] + g[k][j];
}

int main() {
	return 0;
}
