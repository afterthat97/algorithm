#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cmath>
#define maxn 1000005
#define ll long long
using namespace std;

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
