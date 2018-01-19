#include <stdio.h>
#include <cstring>
#define MAXN 1005 

int pre[MAXN], mid[MAXN], suc[MAXN];
int tree[MAXN], l[MAXN], r[MAXN];
int cnt, n, temp;

void build1(int a, int b, int &idx) {
	tree[++idx] = pre[++cnt];
	int now = idx, t = -1;
	if (a == b) {
		if (mid[a] != tree[idx]) {/*Build failed*/}
		return;
	}
	for (int i = a; i <= b; i++)
		if (mid[i] == tree[idx]) { t = i; break; }
	if (t == -1) {/*Build failed*/}
	if (t > a) { l[now] = idx + 1; build1(a, t - 1, idx); }
	if (t < b) { r[now] = idx + 1; build1(t + 1, b, idx); }
}

void build2(int a, int b, int &idx) {
	tree[++idx] = suc[--cnt];
	int now = idx, t = -1;
	if (a == b) {
		if (mid[a] != tree[idx]) {/*Build failed*/}
		return;
	}
	for (int i = a; i <= b; i++)
		if (mid[i] == tree[idx]) { t = i; break; }
	if (t == -1) {/*Build failed*/}
	if (t < b) { r[now] = idx + 1; build2(t + 1, b, idx); }
	if (t > a) { l[now] = idx + 1; build2(a, t - 1, idx); }
}

int main() {
	memset(l, -1, sizeof l);
	memset(r, -1, sizeof r);
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &suc[i]);
	for (int i = 1; i <= n; i++) scanf("%d", &mid[i]);

	cnt = 0;
	build1(1, n, temp = 0);
	cnt = n + 1;
	build2(1, n, temp = 0);
	
	return 0;
}
