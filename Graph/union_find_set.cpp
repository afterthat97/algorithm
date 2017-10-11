#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 10005
using namespace std;

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

int main() {


	return 0;
}

