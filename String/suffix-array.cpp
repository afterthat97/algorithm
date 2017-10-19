#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 20005
using namespace std;

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

int main() {

	return 0;
}
