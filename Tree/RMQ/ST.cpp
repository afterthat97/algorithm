#include <iostream>
#include <stdio.h>
#define maxn 10005
#define MAX_LGN 15
using namespace std;

int st[MAX_LGN][1 << MAX_LGN], a[maxn];

void init(int* a, int n) {
	for (int j = 0; j < n; j++) st[0][j] = a[j];
	for (int i = 1; (1 << i) <= n; i++)
		for (int j = 0; j <= n - (1 << i); j++)
			st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
}

inline int query(int l, int r) {
	int i = 0;
	while ((1 << (i + 1)) <= r - l + 1) i++;
	return min(st[i][l], st[i][r - (1 << i) + 1]);
}

int main() {
	int n; scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	init(a, n);
	
	int m; scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		int l, r;
		scanf("%d %d", &l, &r);
		printf("%d\n", query(l, r));
	}
	return 0;
}
