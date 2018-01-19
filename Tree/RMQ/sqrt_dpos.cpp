#include <stdio.h>
#include <cmath>
#define ll long long

ll sum[450], addv[450], a[200005];
int size, n, m;

inline void maintain(int idx, int k, int v) {
	a[k] += v;
	sum[idx] += v;
}

inline void add(int l, int r, int v) {//0 <= l, r < n
    int idx1 = l / size, idx2 = r / size, k;
    if (idx1 == idx2) {
        for (k = l; k <= r; ++k) maintain(idx1, k, v);
        return;
    }
    for (k = idx1 + 1; k < idx2; ++k) addv[k] += v;
    for (k = l; k < (idx1 + 1) * size; ++k) maintain(idx1, k, v);
    for (k = idx2 * size; k <= r; ++k) maintain(idx2, k, v);
}

inline ll query(int l, int r) {//0 <= l, r < n
    int idx1 = l / size, idx2 = r / size, k;
	ll ans = 0;
    if (idx1 == idx2) {
        for (k = l; k <= r; ++k) ans += a[k] + addv[idx1];
		return ans;
    }
    for (k = idx1 + 1; k < idx2; ++k) ans += addv[k] * size + sum[k];
    for (k = l; k < (idx1 + 1) * size; ++k) ans += a[k] + addv[idx1];	
	for (k = idx2 * size; k <= r; ++k) ans += a[k] + addv[idx2];
	return ans;
}
