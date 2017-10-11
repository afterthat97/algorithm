#include <stdio.h>
#include <cstring>
#define ll long long
#define MAXN 10 //max_matrix 10 * 10
#define MOD 10000007

struct matrix {
	int n, m;
	ll dat[MAXN][MAXN];//both start from 1
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
