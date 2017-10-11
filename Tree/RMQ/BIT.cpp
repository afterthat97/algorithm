#include <iostream>
#include <stdio.h>
#define MAXN 100005
using namespace std;

int tree[MAXN], n;

inline int sum(int x) {
	int ans = 0;
	for (; x > 0; x -= (x & -x))
		ans += tree[x];
	return ans;
}

inline void add(int x, int val) {
	for (; x <= n; x += (x & -x))
		tree[x] += val;
}

int main() {

	return 0;
}
