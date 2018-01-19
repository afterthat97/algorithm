#include <iostream>
#include <stdio.h>
#include <cstring>
#define maxn 20005
#define mod 1000000007
using namespace std;

int tree[maxn], n, k;
int a[maxn], pre[maxn], cnt[maxn];

inline int sum(int x) {
	int ans = 0;
	for (; x > 0; x -= (x & -x))
		ans = (ans + tree[x]) % mod;
	return ans;
}

inline void add(int x, int val) {
	for (; x <= n; x += (x & -x))
		tree[x] = (tree[x] + val) % mod;
}

int main() {
	int T; cin >> T;
	for (int idx = 0; idx < T; idx++) {
		scanf("%d %d", &n, &k);
		for (int i = 0; i < n; i++) {
			scanf("%d", &a[i]);
			cnt[i] = 1;
		}
		for (int j = 0; j < k - 1; j++) {//des: cnt
			memset(tree, 0, sizeof tree);
			memcpy(pre, cnt, sizeof cnt);
			for (int i = n - 1; ~i; i--) {
				cnt[i] = sum(a[i]);
				add(a[i], pre[i]);
			}
		}
		int ans = 0;
		for (int i = 0; i < n; i++) 
			ans = (ans + cnt[i]) % mod;
		printf("%d\n", ans);
	}
	return 0;
}
