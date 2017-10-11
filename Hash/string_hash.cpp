#include <stdio.h>
#include <cstring>

const int mod1 = 100003;
char h[mod1][1005];

inline int BKDRHash(const char* s) {
	int ans = 0, len = strlen(s);
	for (int i = 0; i < len; i++)
		ans = ans * 131 + s[i];
	return (ans & 0x7fffffff) % mod1;
}

inline void ins(const char *s) {
	for (int idx = BKDRHash(s);; idx = (idx + 1) % mod1) {
		if (h[idx][0] == 0) { strcpy(h[idx], s); return; }
		if (strcmp(h[idx], s) == 0) return;
	}
}

inline bool query(const char *s) {
	for (int idx = BKDRHash(s);; idx = (idx + 1) % mod1) {
		if (h[idx][0] == 0) return false;
		if (strcmp(h[idx], s) == 0) return true;
	}
}

int n, m;
char s[1005];

int main() {
	scanf("%d\n", &n);
	for (int i = 0; i < n; i++) {
		scanf("%s", s);
		ins(s);
	}
	scanf("%d\n", &m);
	for (int i = 0; i < m; i++) {
		scanf("%s", s);
		puts(query(s) ? "Yes" : "No");
	}
	return 0;
}
