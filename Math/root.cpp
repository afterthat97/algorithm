#include <iostream>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#define gcd __gcd
#define maxn 1000005
#define ll long long
using namespace std;

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
