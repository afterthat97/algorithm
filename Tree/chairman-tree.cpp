#include <iostream>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#define fst first
#define snd second
#define MAXN 100005
#define MAXM 50005
using namespace std;

struct node {
	int l, r, num;
	node() {}
	node(int L, int R, int NUM) {
		l = L; r = R; num = NUM;
	}
}tree[MAXN << 5];

pair<int, int> raw[MAXN];
int root[MAXN], a[MAXN], n, m, cnt;

int ins(int pre, int pos) {
	int now = ++cnt;
	int l = 1, r = n, ret = now;
	while (l < r) {
		int mid = (l + r) >> 1;
		if (pos <= mid) {
			tree[now] = node(++cnt, tree[pre].r, tree[pre].num + 1);
			pre = tree[pre].l; r = mid;
		} else {
			tree[now] = node(tree[pre].l, ++cnt, tree[pre].num + 1);
			pre = tree[pre].r; l = mid + 1;
		}
		now = cnt;
	}
	tree[now] = node(0, 0, 1);
	return ret;
}

int query(int ql, int qr, int k) {
	int l = 1, r = n;
	while (l < r) {
		int mid = (l + r) >> 1;
		int num = tree[tree[qr].l].num - tree[tree[ql].l].num;
		if (k <= num) {
			ql = tree[ql].l; qr = tree[qr].l;
			r = mid;
		} else {
			ql = tree[ql].r; qr = tree[qr].r;
			l = mid + 1; k -= num;
		}
	}
	return l;
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &raw[i].fst);
		raw[i].snd = i;
	}
	sort(raw + 1, raw + n + 1);
	for (int i = 1; i <= n; i++)
		a[raw[i].snd] = i;
	for (int i = 1; i <= n; i++)
		root[i] = ins(root[i - 1], a[i]);
	for (int i = 0; i < m; i++) {
		int l, r, k;
		scanf("%d %d %d", &l, &r, &k);
		printf("%d\n", raw[query(root[l - 1], root[r], k)].fst);
	}
	return 0;
}
