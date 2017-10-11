#include <cstring>
#include <stdio.h>
#define ll long long
#define mod1 1000003
#define mod2 1009
#define hash1(x) ((x & 0x7fffffff) % mod1)
#define hash2(x) ((x & 0x7fffffff) % mod2)
int h[mod1], m, n, t;

inline void ins(int x) {
	int idx, h1 = hash1(x), h2 = hash2(x);
	for (int i = 0;; i++) {
		idx = (h1 + i * h2) % mod1;
		if (h[idx] == x || h[idx] == -1) break;
	}
	h[idx] = x;
}

inline bool query(int x) {
	int idx, h1 = hash1(x), h2 = hash2(x);
	for (int i = 1;; i++) {
		idx = (h1 + i * h2) % mod1;
		if (h[idx] == -1) return false;
		if (h[idx] == x) return true;
	}
}

int main() {
	memset(h, -1, sizeof(h));
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) { scanf("%d", &t); ins(t); }
	for (int i = 0; i < m; i++) { scanf("%d", &t); puts(query(t) ? "YES" : "NO"); }
	return 0;
}
